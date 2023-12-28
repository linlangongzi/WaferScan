#include "Ars3000.h"
#include <Ars1000/PipeInstrumentDriver.h>
#include <Ars500/ARS500.h>
#include <Motion/MotionManager.h>
#include <QApplication>
#include <QElapsedTimer>
#include <QLoggingCategory>
#include <QRegularExpression>
#include <QTimer>
#include <Ultrasonic/InstrumentSetup.h>
#include <Ultrasonic/UTDataHeader.h>
#include <Ultrasonic/UTStreamFile.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/Util.h>
#include <utils/algorithm.h>

using namespace Ars500;

#define FromInstrumentEndian qFromLittleEndian

static const int AD_Frequency = 400;
static const QString hardwareModel = "Allrising ARS500 V3";
static const QString deviceType = "ARS3000";
static const QVersionNumber minSoftwareVersion(1);
static const int fpgaMinVersion = 0x509;
static const int fpgaMaxVersion = 0x600;
static const int fpgaSurfaceTracingVersion = 0x522;
static const int channelCount = 2;
static const int encoderCount = 7;
static const int hardwareGateCount = 4;
static const UTDataHeader ars3000DataHeader = ARS3000::InitDataHeader();
static const int ars3000DataHeaderId = UTDataFrame::RegisterHeader(ars3000DataHeader, HeaderArs3000);

static Q_LOGGING_CATEGORY(debug, "ars3000", QtWarningMsg)

UTDataHeader ARS3000::InitDataHeader()
{
#ifdef PIPE_DETECT
    return PipeDetect::PipeInstrumentDriver::InitDataHeader();
#else
    UTDataHeader header("ARS3000");
    header.SetType(0, 0, GateTrigger,   DataTime,   tr("触发时间"), -1);
    header.SetType(1, 0, GateMaxPeak,   DataTime,   tr("最大波峰时间"), 0);
    header.SetType(2, 0, GateMaxPeak,   DataAmp,    tr("最大波峰高度"), -1);
    for (int gate = 1; gate < NUM_GATES; ++gate) {
        header.SetType(gate * 5 - 2, gate, GateMaxPeak,     DataTime,   tr("最大波峰时间"), 0);
        header.SetType(gate * 5 - 1, gate, GateMaxPeak,     DataAmp,    tr("最大波峰高度"), -1);
        header.SetType(gate * 5 + 0, gate, GateTrigger,     DataTime,   tr("触发时间"), 0);
        header.SetType(gate * 5 + 1, gate, GateFirstPeak,   DataTime,   tr("首次波峰时间"), 0);
        header.SetType(gate * 5 + 2, gate, GateFirstPeak,   DataAmp,    tr("首次波峰高度"), -1);
    }
    return header;
#endif
}

enum BenchmarkKeys {
    COL_BYTE = 0,
    COL_FRAME = 1,
    COL_SEGMENT = 2,

    GLOBAL_INVALID = -1000,
    GLOBAL_SENT = -14,
    GLOBAL_RAW = -13,
    GLOBAL_TEXT = -12,
    GLOBAL_BINARY = -11,
    GLOBAL_BAD_DATA = -10,

    CHANNEL_SHIFT = 8,
    LEVEL_ALL = -1,
    LEVEL_MONITOR = 0,
    LEVEL_FEATURE = 1,
    LEVEL_WAVE = 2,
};

static QString ToLuaValue(const QJsonValue &v)
{
    switch (v.type()) {
    case QJsonValue::Bool:      return QString::number(v.toBool() ? 1 : 0);
    case QJsonValue::Double:    return QString::number(v.toDouble());
    case QJsonValue::String:    return '\"' + v.toString() + '\"';
    case QJsonValue::Array: {
        const QJsonArray array = v.toArray();
        QStringList list;
        list.reserve(array.size());
        for (auto it = array.constBegin(); it != array.constEnd(); ++it) {
            list << ToLuaValue(*it);
        }
        return list.join(", ").prepend('{').append('}');
    }
    case QJsonValue::Object: {
        const QJsonObject obj = v.toObject();
        QStringList list;
        list.reserve(obj.size());
        for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
            list << QString("%1 = %2").arg(it.key(), ToLuaValue(it.value()));
        }
        return list.join(", ").prepend('{').append('}');
    }
    case QJsonValue::Null:
    case QJsonValue::Undefined:
    default:                    return "nil";
    }
}

ARS3000::ARS3000(QObject *parent) :
    IInstrumentDriver(parent),
    socket(this),
    m_benchmark(this)
{
    qRegisterMetaType<QAbstractSocket::SocketState>();
    m_setups.resize(NUM_CHANNELS);

    connect(&socket, &QTcpSocket::connected, this, &ARS3000::onConnected);
    connect(&socket, &QTcpSocket::readyRead, this, &ARS3000::onDataArrived);
    connect(&socket, &QTcpSocket::stateChanged, this, &ARS3000::onSocketStateChanged);
    connect(&socket, QOverload<QTcpSocket::SocketError>::of(&QTcpSocket::error), this, [this] () {
        qCDebug(debug) << "Socket error:  Status" << instrumentStatus() << ", socket state" << socket.state() << ", error" << socket.errorString();
    });

    for (int channel = 0; channel < NUM_CHANNELS; ++channel) {
        setDataLevel(ASCAN_ONLY, channel);
    }

    m_benchmark.SetColName(COL_BYTE, tr("字节"));
    m_benchmark.SetColName(COL_FRAME, tr("帧"));
    m_benchmark.SetColName(COL_SEGMENT, tr("块"));
    m_benchmark.SetRowName(GLOBAL_SENT, GLOBAL_INVALID, tr("发送"));
    m_benchmark.SetRowName(GLOBAL_RAW, GLOBAL_INVALID, tr("接收"));
    m_benchmark.SetRowName(GLOBAL_TEXT, GLOBAL_INVALID, tr("接收文本"));
    m_benchmark.SetRowName(GLOBAL_BINARY, GLOBAL_INVALID, tr("接收二进制"));
    m_benchmark.SetRowName(GLOBAL_BAD_DATA, GLOBAL_INVALID, tr("接收坏包"));
    for (int channel = 0; channel < NUM_CHANNELS; ++channel) {
        int p = m_benchmark.SetRowName((channel << CHANNEL_SHIFT) + LEVEL_ALL, GLOBAL_INVALID, tr("通道 %1").arg(channel));
        m_benchmark.SetRowName((channel << CHANNEL_SHIFT) + LEVEL_MONITOR, p, tr("监视"));
        m_benchmark.SetRowName((channel << CHANNEL_SHIFT) + LEVEL_FEATURE, p, tr("特征值"));
        m_benchmark.SetRowName((channel << CHANNEL_SHIFT) + LEVEL_WAVE, p, tr("波形"));
    }
    m_info.manufacturer = tr("Allrising Technology");
    m_info.voltage = {100, 400};
    m_info.supportAlterSource = true;
    m_info.supportPulseWidth = true;
}

void ARS3000::setHostAddr(const QString &ip)
{
    hostIp = ip;
}

void ARS3000::Connect()
{
    socket.connectToHost(hostIp, 6666);
    if (socket.thread() != thread()) {
        qCCritical(debug) << "Socket not in driver thread";
    }
}

void ARS3000::Disconnect()
{
    if (socket.state() == QTcpSocket::ConnectedState) {
        socket.disconnectFromHost();
    } else {
        socket.abort();
    }
}

void ARS3000::onConnected()
{
    qCDebug(debug) << "General socket connected";
    QString startupScript = ReadFileAsBinary(":/Ars3000.lua");
    if (startupScript.isEmpty()) {
        qCCritical(debug) << "Read lua file failed";
    }
    sendCommand(startupScript);
#ifdef PIPE_DETECT
    sendCommand("print('Configuring instrument as PipeDetect') SetMemoryInt16(0x5E, 0x3)");
#endif
    for (int channel = 0; channel < NUM_CHANNELS; ++channel) {
        setDataLevel(dataLevel[channel], channel);
    }
    for (int channel = 0; channel < m_setups.size(); ++channel) {
        UTSetup setup = m_setups.value(channel);
        setChannelSetup(channel, setup);
    }
    sendCommand("SetInstrument(Ars3000)");
}

void ARS3000::onDataArrived()
{
    static const QByteArray binarySignature(PACKET_SIGNATURE, sizeof(PACKET_SIGNATURE) - 1);
    while (!socket.atEnd()) {
        qint64 bytesAvailable = socket.bytesAvailable();
        QByteArray signature = socket.peek(binarySignature.size());      /* peek搜索二进制包签名 */
        if (binarySignature.startsWith(signature)) {
            if (bytesAvailable < binarySignature.size() + qint64(sizeof(quint32))) {
                break;
            }

            char buffer[GENERAL_MIN_PACKET_SIZE];
            socket.peek(buffer, sizeof(buffer));
            auto *packetSize = reinterpret_cast<quint32 *>(&buffer[binarySignature.size()]);
            if (bytesAvailable < qint64(GENERAL_MIN_PACKET_SIZE + *packetSize)) {
                break;
            }
            socket.read(buffer, sizeof(buffer));                                /* 将签名与长度从流中弹出 */

            qint64 bytesRead = parseBinaryData(*packetSize);
            m_benchmark.Add(COL_BYTE, GLOBAL_BINARY, bytesRead + sizeof(buffer));

            qint64 bytesBad = bytesRead - *packetSize;                          /* 未被 ParseBinaryData 读取的剩余数据，将其弹出 */
            if (bytesBad > 0) {
                qCDebug(debug) << "Bad data " << bytesBad << "bytes";
                socket.read(bytesBad);
                m_benchmark.Add(COL_BYTE, GLOBAL_BAD_DATA, bytesBad);
            }
        } else {
            const QByteArray lineData = socket.readLine();
            m_benchmark.Add(COL_BYTE, GLOBAL_TEXT, lineData.size());
            const QByteArray line = lineData.trimmed();
            if (!line.isEmpty()) {
                if (line.startsWith("REPORT_CONFIG")) {
                    QVariantMap var = ByteArrayToJson(line.split('\t').value(1)).toObject().toVariantMap();
                    if (!var.isEmpty()) {
                        variables = var;
                        updateVariables();
                    }
                } else if (line.startsWith("REPORT_INFO")) {
                    QJsonObject var = ByteArrayToJson(line.split('\t').value(1)).toObject();
                    if (!var.isEmpty()) {
                        updateInstrumentInfo(var);
                    }
                } else {
                    emit messageReceived(line);
                }
//                qCDebug() << "ARS3000:" << line;
            }
        }
        m_benchmark.Add(COL_BYTE, GLOBAL_RAW, bytesAvailable - socket.bytesAvailable());
    }
}

void ARS3000::onSocketStateChanged()
{
    QTcpSocket::SocketState socketState = socket.state();
    if (socketState == QTcpSocket::ConnectedState) {
        return setInstrumentStatus(InstrumentStatus::Connected, tr("连接成功"));
    } else if (socketState == QTcpSocket::HostLookupState || socketState == QTcpSocket::ConnectingState) {
        return setInstrumentStatus(InstrumentStatus::Initializing, tr("正在初始化"));
    } else {
        if (instrumentStatus().state == InstrumentStatus::Connected && socket.error() != QAbstractSocket::UnknownSocketError) {
             return setInstrumentStatus(InstrumentStatus::Disconnected, tr("连接中断: %1").arg(socket.errorString()));
        } else if (instrumentStatus().state == InstrumentStatus::Initializing && socket.error() != QAbstractSocket::UnknownSocketError) {
            return setInstrumentStatus(InstrumentStatus::Disconnected, tr("连接失败: %1").arg(socket.errorString()));
        }
        return setInstrumentStatus(InstrumentStatus::Disconnected, tr("断开连接"));
    }
}

void ARS3000::updateVariables()
{
    const int waveformSize = 1024;
    rawAmp.fill(0, waveformSize);
    int waveformBase = variables["WaveformBase"].toInt();
    if (waveformBase >= 0) {
        const qreal scale = waveformSize / qreal(waveformSize - waveformBase);
        for (int index = waveformBase; index < waveformSize; ++index) {
            rawAmp[index] = (index - waveformBase) * scale;
        }
    }
}

void ARS3000::updateInstrumentInfo(const QJsonObject &obj)
{
    for (const QString &key: obj.keys()) {
        MetaType::writeGadgetProperty(m_info, key, obj[key]);
    }
    m_info.channelCount = channelCount;
    m_info.encoderCount = encoderCount;
    m_info.gateCount = hardwareGateCount;
    if (m_info.hardwareVersion == hardwareModel) {
        m_info.supportPulseType = true;
        m_info.voltage.clear();
        for (int step = 50; step <= 400; step += 50) {
            m_info.voltage.append(step);
        }
    } else {
        m_info.supportPulseType = false;
        m_info.voltage = {100, 400};
    }
    if (QVersionNumber::fromString(m_info.softwareVersion) < minSoftwareVersion) {
        socket.disconnectFromHost();
        return setInstrumentStatus(InstrumentStatus::Disconnected, tr("连接失败: 软件版本 %1 不匹配").arg(QVersionNumber::fromString(m_info.softwareVersion).toString()));
    }
    int fpga = m_info.fpgaVersion.toInt(nullptr, 16);
    if (fpga < fpgaMinVersion || fpga >= fpgaMaxVersion) {
        socket.disconnectFromHost();
        return setInstrumentStatus(InstrumentStatus::Disconnected, tr("连接失败: FPGA版本 %1 不匹配").arg(m_info.fpgaVersion));
    }
    m_info.supportSurfaceTracking = (fpga >= fpgaSurfaceTracingVersion);
    emit instrumentInfoChanged(m_info);
}

struct FpgaBufferHeader
{
    quint32 sectionSize = 0;
    quint32 sectionCount = 0;
    quint16 channel = -1;
    quint16 levelOfService = -1;
    quint16 timeMultiplier = 1;
};

qint64 ARS3000::parseBinaryData(qint64 size)
{
    if (size < qint64(sizeof(qint32))) {
        return 0;
    }
    qint64 bytesAvailable = size;
    qint32 type;
    bytesAvailable -= socket.read(reinterpret_cast<char *>(&type), sizeof(type));
//    qCDebug(debug) << "Got binary data len=" << size << "type=" << type;

    FpgaBufferHeader header;
    if (type == 0x10000) {
        if (bytesAvailable < qint64(sizeof(FpgaBufferHeader))) {
            qCDebug(debug) << "Bad binary data - invalid header";
            return size - bytesAvailable;
        }
        bytesAvailable -= socket.read(reinterpret_cast<char *>(&header), sizeof(header));
        if (header.channel >= NUM_CHANNELS || bytesAvailable < header.sectionCount * header.sectionSize) {
            qCDebug(debug) << "Bad binary data - wrong channel or section size";
            return size - bytesAvailable;
        }

//        if (header.sectionCount > 1) {
//            qCDebug(debug) << "Got" << header.sectionCount << "sections, " << header.sectionSize << "bytes each.";
//        }
        UTDataSegment segment;
        segment.reserve(header.sectionCount);

        UTSetup parameter = m_setups.value(header.channel);
        if (header.channel == 1 && m_setups.value(1).alterSource()) {
            parameter = m_setups.value(0);
        }
        CDataValue range = parameter.range();
        CDataValue delay = parameter.delay();
        bool surfaceTracing = parameter.alterSource();
        if (surfaceTracing) {
            delay = 0;
        }

        qint64 bytesAvailableStart = bytesAvailable;
        for (; header.sectionCount; --header.sectionCount) {
            UTDataFrame frame = UTDataFrame();
            if (header.sectionSize == 64) {
                bytesAvailable -= readCDataFrame(frame, header.timeMultiplier);
            } else if (header.sectionSize == 64 + 512) {
                bytesAvailable -= readCompressedADataFrame(frame);
                bytesAvailable -= readCDataFrame(frame, header.timeMultiplier);
            } else if (header.sectionSize == 64 + 1024) {
                bytesAvailable -= readADataFrame(frame);
                bytesAvailable -= readCDataFrame(frame, header.timeMultiplier);
            }
            frame.Context.SetChannelID(header.channel);
            frame.Context.setDelay(delay);
            frame.Context.SetRange(range);
            frame.headerId = ars3000DataHeaderId;
            if (surfaceTracing) {
                frame.CData[0] = frame.CData[0] / header.timeMultiplier * 4;
            }
            segment.append(frame);
        }

        m_benchmark.Add(COL_SEGMENT,  (header.channel << CHANNEL_SHIFT) + header.levelOfService, 1);
        m_benchmark.Add(COL_FRAME,    (header.channel << CHANNEL_SHIFT) + header.levelOfService, segment.count());
        m_benchmark.Add(COL_BYTE,     (header.channel << CHANNEL_SHIFT) + header.levelOfService, bytesAvailableStart - bytesAvailable);
        m_benchmark.Add(COL_SEGMENT,  (header.channel << CHANNEL_SHIFT) + LEVEL_ALL, 1);
        m_benchmark.Add(COL_FRAME,    (header.channel << CHANNEL_SHIFT) + LEVEL_ALL, segment.count());
        m_benchmark.Add(COL_BYTE,     (header.channel << CHANNEL_SHIFT) + LEVEL_ALL, bytesAvailableStart - bytesAvailable);

        if (segment.count() == 1) {
            emit monitorDataArrived(segment[0]);
        } else if (segment.count() > 1) {
            emit dataArrived(segment);
#ifdef ARS_DUMP_CSV
            if (qApp->property("DumpCsv").toBool()) {
                DumpSegment(QString("%1-%2.csv").arg(objectName()).arg(header.channel), segment);
            }
#endif
        }
    }
//    qCDebug(debug) << "Size is" << size << ", bytesStillAvailable" << bytesAvailable;
    return size - bytesAvailable;
}

Benchmark *ARS3000::benchmark()
{
    return &m_benchmark;
}

void ARS3000::sendParameters()
{
    QString command;
    QTextStream s(&command);

    const UTSetup setup = m_setups.value(0);;
    s << "Ars3000 = {" << endl;
    s << "ProbeMultiplex"       << " = " << setup.probeMultiplex()  << "," << endl;
    s << "PRF"                  << " = " << setup.prf()             << "," << endl;
    s << "TransmitVoltage"      << " = " << setup.transmitVoltage() << "," << endl;
    for (const UTSetup &setup: m_setups) {
        s << "{" << endl;
        s << "Delay"            << " = " << setup.delay()           << "," << endl;
        s << "Range"            << " = " << setup.range()           << "," << endl;
        s << "PulserEnable"     << " = " << setup.pulserEnable()    << "," << endl;
        s << "TransmitMode"     << " = " << setup.transmitMode()    << "," << endl;
        s << "TransmitEnergy"   << " = " << setup.transmitEnergy()  << "," << endl;
        s << "PulseWidth"       << " = " << setup.pulseWidth()      << "," << endl;
        s << "Impedance"        << " = " << setup.impedance()       << "," << endl;
        s << "RectifierMode"    << " = " << setup.rectifierMode()   << "," << endl;
        s << "Gain"             << " = " << setup.gain()            << "," << endl;
        s << "GainRef"          << " = " << setup.gainRef()         << "," << endl;
        s << "ManualGain"       << " = " << setup.manualGain()      << "," << endl;
        s << "PreAmplify"       << " = " << setup.preAmplify()      << "," << endl;
        s << "Amplify1"         << " = " << setup.amplify1()        << "," << endl;
        s << "Amplify2"         << " = " << setup.amplify2()        << "," << endl;
        s << "ProbeBand"        << " = " << setup.probeBand()       << "," << endl;
        s << "SampleAverage"    << " = " << setup.sampleAverage()   << "," << endl;
        s << "Suppress"         << " = " << setup.suppress()        << "," << endl;
        s << "AlterSource"      << " = " << setup.alterSource()     << "," << endl;
        s << "TcgEnable"        << " = " << (setup.dacMode() == UTSetup::DacTcg) << "," << endl;
        s << "SurfaceTracing"   << " = " << setup.surfaceTracing().enable        << "," << endl;
        s << "SurfaceOffset"    << " = " << setup.surfaceTracing().offset        << "," << endl;
        s << "SurfaceThreshold" << " = " << setup.surfaceTracing().threshold     << "," << endl;
        s << "SurfaceStart"     << " = " << setup.surfaceTracing().earlist       << ","  << endl;
        s << "SurfaceEnd"       << " = " << setup.surfaceTracing().latest        << ","  << endl;
        s << "TCG = {" << endl;
        const auto dac = setup.dac();
        for (auto it = dac.cbegin(); it != dac.cend(); ++it) {
            qreal targetGain = UltrasonicManager::ampToDecibel(setup.dacTarget() / 100);
            qreal gain = -(it.value() + setup.gain()) + (qIsFinite(targetGain) ? targetGain : 200);
            s << "{Time = " << it.key() << ", Gain = " << gain << "}," << endl;
        }
        s << "}," << endl;

        for (const auto &gate: setup.gates()) {
            s << "{" << endl;
            s << "Enable"       << " = " << gate.isEnabled()        << "," << endl;
            s << "Start"        << " = " << gate.start()            << "," << endl;
            s << "Width"        << " = " << gate.width()            << "," << endl;
            s << "Amp"          << " = " << gate.threshold()        << "," << endl;
//            s << SyncMode        = {address = 0x02},
//            SyncThreshold   = {address = 0x04},
            s << "}," << endl;
        }
        s << "}," << endl;
    }
    s << "} c()";

    sendCommand(command);
}

void ARS3000::setDataLevel(int level, int channel)
{
    Q_ASSERT(channel < NUM_CHANNELS && channel >= 0);
    dataLevel[channel] = level;
    for (int maskLevel = 0; maskLevel < NUM_LEVELS; ++maskLevel) {
        sendCommand(QString("SetDataLevel(%1, %2, %3)").arg(channel).arg(maskLevel).arg(((1 << maskLevel) & level) ? "true" : "false"));
    }
    emit DataLevelChanged(level, channel);
}

void ARS3000::resetPose()
{
    sendCommand("ResetEncoders()");
    poseOffset.fill(0);
}

QVariantMap ARS3000::getVariables() const
{
    return variables;
}

void ARS3000::setVariables(const QVariantMap &var)
{
    sendCommand(QString("SetConfig(%1)").arg(ToLuaValue(QJsonObject::fromVariantMap(var))));
}

InstrumentInfo ARS3000::instrumentInfo() const
{
    return m_info;
}

void ARS3000::setIdentifier(const QHash<int, ChannelIdentifier> &identifier)
{
    identifierFront = identifier;
    identifierBack.fill(-1, NUM_CHANNELS);
    for (auto it = identifier.begin(); it != identifier.end(); ++it) {
        int channel = it.value().channel;
        if (channel < identifierBack.size()) {
            identifierBack[channel] = it.key();
        }
    }
}

void ARS3000::setChannelSetup(int channel, const UTSetup &setup)
{
    if ( (channel < 0) || ( channel >= m_setups.count() ) ) {
        qCWarning(debug) << "setChannelSetup input channel is out of limit";
        return;
    }
    m_setups.replace(channel, setup);
    sendParameters();
}

/*! 更新编码器偏移 */
void ARS3000::setPoseOffset(const UTPose &offset)
{
    poseOffset.resize(qMax(poseOffset.size(), offset.size()));
    for (int index = 0; index < offset.size(); ++index) {
        poseOffset[index] += offset[index];
    }
}

void ARS3000::sendCommand(const QString &command)
{
    if (!command.isEmpty() && socket.state() == QTcpSocket::ConnectedState) {
        qCDebug(debug) << "Sending command" << command;
        qint64 bytesWritten = socket.write(command.toLocal8Bit().replace("\r\n", "\r").replace('\n', '\r'));
        bytesWritten += socket.write("\n");
        socket.flush();
        m_benchmark.Add(COL_BYTE, GLOBAL_SENT, bytesWritten);
//        emit MessageReceived(command);
    }
}

void ARS3000::sendRawMessage(int type, const QVariant &message)
{
    if (type == MessageEncoder) {
        alternativeCoder = bool(message.toInt());
    }
}

qint64 ARS3000::readADataFrame(UTDataFrame &frame)
{
    Q_ASSERT(BYTES_FRAME_ASCAN == sizeof(ADataValue) * A_SCAN_NUM_SAMPLES_PER_FRAME);
    frame.AData.resize(A_SCAN_NUM_SAMPLES_PER_FRAME);
    char dummy[sizeof(ADataValue)];
    qint64 bytesRead = socket.read(dummy, sizeof(dummy));
    bytesRead += socket.read(reinterpret_cast<char *>(frame.AData.data()), BYTES_FRAME_ASCAN - sizeof(dummy));
    frame.AData.squeeze();
    for (auto &value: frame.AData) {
        value = mapRawAmplitude(value);
    }
    Q_ASSERT(bytesRead == 1024);
    return bytesRead;
}

qint64 ARS3000::readCompressedADataFrame(UTDataFrame &frame)
{
    QVarLengthArray<uchar, 512> buffer(A_SCAN_NUM_SAMPLES_PER_FRAME);
    qint64 bytesRead = socket.read(reinterpret_cast<char *>(buffer.data()), A_SCAN_NUM_SAMPLES_PER_FRAME);
    ADataFrame &AData = frame.AData;
    AData.resize(A_SCAN_NUM_SAMPLES_PER_FRAME);
    AData.squeeze();
    for (int index = A_SCAN_NUM_SAMPLES_PER_FRAME - 1; index; --index) {        /* 将读出来的512个点映射到A扫数据中的0-510 */
        AData[index - 1] = mapRawAmplitude(ADataValue(buffer[index]) << 2);
    }
    return bytesRead;
}

qint64 ARS3000::readCDataFrame(UTDataFrame &frame, int timeMultiplier)
{
    Q_ASSERT(BYTES_FRAME_FEATURE + BYTES_FRAME_POSE == BYTES_FRAME_CDATA);
    /* 读取位姿 */
    qint32 buf[NUM_AXES];
    qint64 bytesRead = socket.read(reinterpret_cast<char *>(buf), BYTES_FRAME_POSE);
#ifdef PIPE_DETECT
    if (alternativeCoder) {
        qSwap(buf[1], buf[6]);
    } else {
        qSwap(buf[1], buf[3]);
    }
#endif
    quint32 direction = 0;
    frame.Pose = MotionManager::fillPose(buf, NUM_AXES, poseOffset, &direction);
    frame.Context.SetDirections(direction);

    /* 读取特征值 */
    /* 由于QIODevice::read本质是memcpy，涉及指针传递和检查，用于逐个处理数据并不理想，所以使
     * 用一个固定数量特征值缓冲区，此缓冲区在栈上分配更高效。 */
    CDataValueRaw buffer[NUM_FEATURES];
    Q_ASSERT_X(sizeof(buffer) == BYTES_FRAME_FEATURE, "ARS3000::ReadCDataFrame", "Feature data length mismatch");
    bytesRead += socket.read(reinterpret_cast<char *>(&buffer), BYTES_FRAME_FEATURE);       /* 此处使用预先算好的包长度，避免重复运算 */
    CDataFrame &data = frame.CData;
#ifdef PIPE_DETECT
    data.resize(8);
    data[0] = float(buffer[0]) * (timeMultiplier) / AD_Frequency;               /* 闸门1, 触发时间 */
    data[1] = float(buffer[2]) / 1023.0 * 100;

    data[2] = float(buffer[3]) * (timeMultiplier) / AD_Frequency;
    data[3] = float(buffer[4]) / 1023.0 * 100;

    data[4] = float(buffer[8]) * (timeMultiplier) / AD_Frequency;
    data[5] = float(buffer[9]) / 1023.0 * 100;

    data[6] = float(buffer[13]) * (timeMultiplier) / AD_Frequency;
    data[7] = float(buffer[14]) / 1023.0 * 100;
#else
    data.resize(NUM_FEATURES);
    /* 因为需要同时访问DataType表，下标比迭代器更快 */
    for (int index = NUM_FEATURES - 1; index >= 0; --index) {
        switch (CDataType[index]) {
        case DATA_TYPE_TIME:
            data[index] = float(buffer[index]) * (timeMultiplier) / AD_Frequency; break;
        case DATA_TYPE_AMP:
        default:
            data[index] = float(mapRawAmplitude(buffer[index])) / 1023.0 * 100;
        }
    }
#endif
    data.squeeze();

    return bytesRead;
}

ADataValue ARS3000::mapRawAmplitude(ADataValue amp)
{
    if (rawAmp.isEmpty()) {
        return amp;
    }
    return rawAmp.value(amp, amp);
}
