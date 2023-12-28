#include "Mdo3000.h"
#include <allrising/Util.h>
#include <QLoggingCategory>
#include <QUrl>
#include <QtEndian>

Q_LOGGING_CATEGORY(debug, "ut.mdo3000", QtInfoMsg)

static const int A_SCAN_NUM_SAMPLES_PER_FRAME = 512;                            ///< 每帧A扫数据的幅值样本数
static const int NUM_GATES = 4;                                                 ///< 仪器的闸门数
static const int NUM_CHANNELS = 1;                                              ///< 仪器的通道数

using ADataValueRaw = qint16;
using CDataValueRaw = qint16;
using PoseValueRaw = qint32;
static const int BYTES_FRAME_ASCAN = sizeof(ADataValueRaw) * A_SCAN_NUM_SAMPLES_PER_FRAME;

static const char model[] = "MDO3000";
static const char manufacturer[] = "Tektronix";
static const char softwareVersion[] = "1.0";
static const UTDataHeader Mdo3000DataHeader = Mdo3000::InitDataHeader();
static const int Mdo3000DataHeaderId = UTDataFrame::RegisterHeader(Mdo3000DataHeader, HeaderMdo);

UTDataHeader Mdo3000::InitDataHeader()
{
    UTDataHeader header("Mdo3000");
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
}

enum BenchmarkKeys {
    COL_BYTE = 0,
    COL_FRAME = 1,

    GLOBAL_INVALID = -1000,
    GLOBAL_SENT = -14,
    GLOBAL_RAW = -13,
    GLOBAL_TEXT = -12,

    LEVEL_ALL = -1,
};

Mdo3000::Mdo3000(QObject *parent) :
    IInstrumentDriver(parent),
    socket(this),
    benchmark(this)
{
    m_info.model = model;
    m_info.manufacturer = manufacturer;
    m_info.softwareVersion = softwareVersion;
    m_info.channelCount = 1;

    while (localParameter.size() < NUM_CHANNELS) {
        localParameter << UTSetup();
    }
    qRegisterMetaType<QAbstractSocket::SocketState>();
    connect(&socket, &QTcpSocket::connected, this, &Mdo3000::OnConnected);
    connect(&socket, &QTcpSocket::readyRead, this, &Mdo3000::OnDataArrived);
    connect(&socket, &QTcpSocket::stateChanged, this, &Mdo3000::updateInstrumentStatus);

    benchmark.SetColName(COL_BYTE, tr("字节"));
    benchmark.SetColName(COL_FRAME, tr("帧"));
    benchmark.SetRowName(GLOBAL_SENT, GLOBAL_INVALID, tr("发送"));
    benchmark.SetRowName(GLOBAL_RAW, GLOBAL_INVALID, tr("接收"));
    benchmark.SetRowName(GLOBAL_TEXT, GLOBAL_INVALID, tr("接收二进制"));
    benchmark.SetRowName(LEVEL_ALL, GLOBAL_INVALID, tr("波形数据"));

    socket.setSocketOption(QTcpSocket::LowDelayOption, 1);
    socket.setSocketOption(QTcpSocket::KeepAliveOption, 1);
}

Mdo3000::~Mdo3000()
{
    socket.disconnectFromHost();
}

void Mdo3000::setHostAddr(const QString &ip)
{
    hostIp = ip;
}

void Mdo3000::Connect()
{
    if (socket.state() != QTcpSocket::UnconnectedState) {
        return;
    }
    QUrl url = QUrl::fromUserInput(hostIp);
    socket.connectToHost(url.host(), url.port(4000));
    if (socket.thread() != thread()) {
        qCCritical(debug) << "Socket not in driver thread";
    }
}

void Mdo3000::Disconnect()
{
    if (socket.state() == QTcpSocket::ClosingState) {
        socket.abort();
    } else {
        socket.disconnectFromHost();
    }
    qCInfo(debug) << "Disconnected:" << socket.errorString();
}

void Mdo3000::setChannelSetup(int channel, const UTSetup &setup)
{
    if (uint(channel) < uint(localParameter.size())) {
        localParameter[channel] = setup;
    }
}

void Mdo3000::OnConnected()
{
    qCDebug(debug) << "Socket connected";
    context = Context();
    for (int channel = 0; channel < localParameter.size(); ++channel) {
        setChannelSetup(channel, localParameter.at(channel));
    }

    context.idle = false;

    sendCommand("!t 200");                  /* 指令超时时间200ms */
    sendCommand(":header 1");               /* 返回数据时发送字段名 */
    sendCommand(":data:source ch1");        /* 数据源 ch1 */
    sendCommand(":acquire:mode sample");    /* 采集模式 取样 */
    sendCommand(":data:start 1");           /* 采集起点 1 */
    sendCommand(":data:stop 100000");       /* 采集终点 100k */
    sendCommand(":wfmoutpre:encdg binary"); /* 数据编码 二进制 */
    sendCommand(":wfmoutpre:byt_nr 1");     /* 字长 1字节 */
    sendCommand(":wfmoutpre:bn_fmt RP");    /* 二进制格式 unsigned */
//    SendCommand(":busy?");
}

void Mdo3000::OnDataArrived()
{
    qint64 bytes = socket.bytesAvailable();
    qCDebug(debug) << "Socket available" << bytes;

    while (!socket.atEnd() && eatData()) {}
    benchmark.Add(GLOBAL_RAW, LEVEL_ALL, bytes - socket.bytesAvailable());
}

void Mdo3000::updateInstrumentStatus()
{
    QTcpSocket::SocketState generalState = socket.state();
    InstrumentStatus status;
    if (generalState == QTcpSocket::ConnectedState) {
        status.state = InstrumentStatus::Connected;
    } else if (generalState == QTcpSocket::HostLookupState || generalState == QTcpSocket::ConnectingState) {
        status.state = InstrumentStatus::Initializing;
    } else {
        status.state = InstrumentStatus::Disconnected;
    }
    setInstrumentStatus(status);
}

void Mdo3000::setInstrumentStatus(const InstrumentStatus &status)
{
    if (m_status != status) {
        m_status = status;
        emit instrumentStatusChanged(m_status);
    }
}

InstrumentStatus Mdo3000::instrumentStatus() const
{
    return m_status;
}

Benchmark *Mdo3000::GetBenchmark()
{
    return &benchmark;
}

void Mdo3000::sendCommand(const QString &command)
{
    if (command.isEmpty() || socket.state() != QTcpSocket::ConnectedState) {
        return;
    }

    if (context.idle) {
        writeCommand(command);
    } else {
        context.commandQueue << command;
    }
}

void Mdo3000::setDataLevel(int level, int channel)
{
    Q_UNUSED(level)
    Q_UNUSED(channel)
}

void Mdo3000::setActiveChannel(quint32 channelIndex)
{
    Q_UNUSED(channelIndex)
}

void Mdo3000::writeCommand(const QString &command)
{
    qint64 bytesWritten = socket.write(command.toLocal8Bit());
    bytesWritten += socket.write("\r\n");
    socket.flush();
    benchmark.Add(COL_BYTE, GLOBAL_SENT, bytesWritten);
    emit messageReceived("> " + command);
    context.idle = false;
}

inline bool isTerminator(char c)
{
    return QChar(c).isSpace() || c == ';' || c == '>';
}

inline bool isNewLine(char c)
{
    return c == '\n' || c == '\r';
}

/*! 尝试从socket读取下一块数据.
 * socket必须有至少1字节可读数据
 * 返回true时可继续读取, 返回false无法继续读取, 需要等待更多数据.
 */
bool Mdo3000::eatData()
{
    char c = '\0';
    switch (context.state) {
    case WaitingLineStart: {
        socket.getChar(&c);
        if (isNewLine(c)) {
            return true;
        } else if (c == ':') {
            context.commandRoot.clear();
            setState(WaitingCommand);
        } else if (c == '>') {
            setState(WaitingSpace);
        } else {
            context.command.append(c);
            setState(WaitingText);
        }
        return true;
    }
    case WaitingSpace: {
        socket.getChar(&c);
        setIdle(true);
        setState(WaitingLineStart);
        if (c != ' ') {
            qCWarning(debug) << "Got unexpected char" << c << "at" << context.state;
        }
        return true;
    }
    case WaitingText: {
        socket.getChar(&c);
        if (isNewLine(c)) {
            setState(WaitingLineStart);
            eatText();
        } else {
            context.command.append(c);
        }
        return true;
    }
    case WaitingCommand: {
        socket.getChar(&c);
        if (isNewLine(c)) {
            setState(WaitingLineStart);
            if (!context.command.isEmpty()) {
                qCWarning(debug) << "Unexpected end char" << int(c) << "after command" << context.command;
            }
        } else if (isTerminator(c)) {
            setState(WaitingValue);
        } else {
            context.command.append(c);
        }
        return true;
    }
    case WaitingValue: {
        socket.getChar(&c);
        if (isNewLine(c)) {
            setState(WaitingLineStart);
            eatCommand();
        } else if (c == '#') {
            setState(WaitingDigitLength);
            if (!context.value.isEmpty()) {
                qCWarning(debug) << "Unexpected value" << context.value << "before binary";
            }
        } else if (c == '"') {
            setState(WaitingQuote);
            context.value.append(c);
        } else if (isTerminator(c)) {
            qCDebug(debug) << "Terminated command" << context.command << "value" << context.value << "with" << int(c);
            setState(WaitingCommand);
            eatCommand();
        } else {
            context.value.append(c);
        }
        return true;
    }
    case WaitingQuote: {
        socket.getChar(&c);
        context.value.append(c);
        if (c == '"') {
            setState(WaitingValue);
        }
        return true;
    }
    case WaitingDigitLength: {
        socket.getChar(&c);
        context.expectLength = c - '0';
        if (context.expectLength == 0) {
            setState(WaitingData);
            context.expectLength = 1;
        } else {
            setState(WaitingLength);
        }
        return true;
    }
    case WaitingLength: {
        if (socket.bytesAvailable() < context.expectLength) {
            return false;
        }
        QByteArray length = socket.read(context.expectLength);
        context.expectLength = length.toLongLong();
        setState(WaitingData);
        return true;
    }
    case WaitingData: {
        if (socket.bytesAvailable() < context.expectLength) {
            return false;
        }
        context.value = socket.read(context.expectLength);
        setState(WaitingCommand);
        eatCommand();
        return true;
    }
    default: {
        socket.getChar(&c);
        qCDebug(debug) << "Unexpected char" << c << "at state" << context.state << context.command;
        return true;
    }
    }
}

/*! 更新context中的状态 */
void Mdo3000::setState(Mdo3000::State state)
{
    if (context.state != state) {
        qCDebug(debug) << "State" << context.state << "=>" << state << context.command << context.value << context.expectLength;
    }
    context.state = state;
}

void Mdo3000::setIdle(bool idle)
{
    bool becomesIdle = (!context.idle && idle);
    context.idle = idle;
    if (becomesIdle) {
        context.idle = idle;
        if (!context.commandQueue.isEmpty()) {
            writeCommand(context.commandQueue.takeFirst());
        } else {
            writeCommand(":acquire:numacq?;:horizontal:delay:time?;:curve?");
        }
    }
}

/*! 处理接收到的命令, 即command-value对 */
void Mdo3000::eatCommand()
{
    if (context.command.isEmpty()) {
        return;
    }
    if (context.command.startsWith(':')) {
        context.commandRoot.clear();
        context.command.remove(0, 1);
    }
    if (context.command.contains(':')) {
        QByteArrayList domains = context.command.split(':');
        context.command = domains.takeLast();
        context.commandRoot += ':' + domains.join(':');
    }
    QByteArray fullCommand = context.commandRoot + ':' + context.command;
    if (fullCommand == ":CURVE") {
        parseCurve(context.value);
        qCDebug(debug) << "Curve:" << context.value.size() << context.xzero;
    } else {
        if (context.value.size() > 64) {
            qCWarning(debug) << "Unexpected long command:" << fullCommand << context.value.left(20) << context.value.size();
        }
        if (fullCommand == ":HORIZONTAL:DELAY:TIME") {
            context.xzero = context.value.toDouble() * 1e6;
        } else if (fullCommand == ":ACQUIRE:NUMACQ") {
            uint newCount = context.value.toUInt();
            context.acquireNew = (newCount != context.acquireCount);
            context.acquireCount = newCount;
        }
        emit messageReceived(fullCommand + " " + context.value);
        qCDebug(debug) << "Command:" << fullCommand << context.value;
    }
    context.command.clear();
    context.value.clear();
}

/*! 处理接收到的文本消息 */
void Mdo3000::eatText()
{
    emit messageReceived(context.command);
    context.command.clear();
}

/*! 处理接收到的二进制波形数据 */
void Mdo3000::parseCurve(const QByteArray &data)
{
    if (!context.acquireNew) {
        return;
    }
    ADataFrame waveform(data.size());
    for (int index = 0; index < waveform.size(); ++index) {
        waveform[index] = data[index] << 8;
    }

    const UTSetup setup = localParameter.value(0);
    UTDataFrame frame;
    frame.AData = waveform;
    frame.headerId = Mdo3000DataHeaderId;
    frame.CData.resize(frame.CData.count());
    frame.Context.SetChannelID(0);
    frame.setContext(UTDataContext::ContextSyncOffset, context.xzero);
    setup.gateFeature(waveform, frame.CData.data());

    benchmark.Add(COL_BYTE, LEVEL_ALL, data.size());
    benchmark.Add(COL_FRAME, LEVEL_ALL, 1);
    emit monitorDataArrived(frame);
    emit messageReceived(QString(":CURVE <%1 bytes>").arg(data.size()));
}
