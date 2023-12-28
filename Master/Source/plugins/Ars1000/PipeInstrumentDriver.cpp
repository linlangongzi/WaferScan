#include "PipeInstrumentDriver.h"
#include <QTime>
#include <QJsonValue>
#include <Ultrasonic/InstrumentSetup.h>

using namespace PipeDetect;

const QString PipeInstrumentDriver::version = "1.0";
const QString PipeInstrumentDriver::deviceType = "PipeDetect";
const UTDataHeader pipeDetectDataHeader = PipeInstrumentDriver::InitDataHeader();
const int pipeDetectDataHeaderId = UTDataFrame::RegisterHeader(pipeDetectDataHeader, HeaderPipe);

PipeInstrumentDriver::PipeInstrumentDriver(const QJsonArray &config, QObject *parent):
    IInstrumentDriver(parent),
    status(InstrumentStatus()),
    selChannelMark(0xFF)
{
    m_info.maxGain = 80;
    m_info.maxPrf = 4000;
    m_info.supportAdjustTransmitMode = false;
    m_info.supportImpedance = false;
    for (int i = 0; i < NUM_CHANNELS; i++) {
        parsers.push_back(new PipeParser(config[i].toObject(), this));
    }
    m_AScanSocket = new PipeAScanSocket(parsers, this);
    m_MonitorSocket = new PipeMonitorSocket(parsers, this);
    m_CScanSocket = new PipeCScanSocket(parsers, this);
    m_ParameterSocket = new PipeParameterSocket(parsers, this);
    nConnectedStatus = 0;
    dataLevel = CSCAN_ONLY;
    InitSockets();
    startTimer(200);
    ResetChannelCount(NUM_CHANNELS);
}

void PipeInstrumentDriver::setHostAddr(const QString &ip)
{
    IPAddr = ip;
}

void PipeInstrumentDriver::Connect()
{
    status.state = InstrumentStatus::Initializing;
//    emit ConnectionChanged(status);
    m_AScanSocket->abort();
    m_MonitorSocket->abort();
    m_CScanSocket->abort();
    m_ParameterSocket->abort();
    m_AScanSocket->connectToHost(IPAddr, ASCAN_PORT);
    m_MonitorSocket->connectToHost(IPAddr, MONITOR_PORT);
    m_CScanSocket->connectToHost(IPAddr, CSCAN_PORT);
    m_ParameterSocket->connectToHost(IPAddr, PARAM_PORT);
}

void PipeInstrumentDriver::Disconnect()
{
    m_AScanSocket->disconnectFromHost();
    m_MonitorSocket->disconnectFromHost();
    m_CScanSocket->disconnectFromHost();
    m_ParameterSocket->disconnectFromHost();
}

void PipeInstrumentDriver::InitSockets()
{
    connect(m_AScanSocket, &QAbstractSocket::connected, this, &PipeInstrumentDriver::OnAScanConnected);
    connect(m_MonitorSocket, &QAbstractSocket::connected, this, &PipeInstrumentDriver::OnMonitorConnected);
    connect(m_CScanSocket, &QAbstractSocket::connected, this, &PipeInstrumentDriver::OnCScanConnected);
    connect(m_ParameterSocket, &QAbstractSocket::connected, this, &PipeInstrumentDriver::OnParamConnected);

    connect(m_AScanSocket, &PipeAScanSocket::dataArrived, this, &PipeInstrumentDriver::OnAScanReceived);
    connect(m_MonitorSocket, &PipeMonitorSocket::dataArrived, this, &PipeInstrumentDriver::OnMonitorReceived);
    connect(m_CScanSocket, &PipeCScanSocket::dataArrived, this, &PipeInstrumentDriver::OnCScanReceived);
    connect(m_ParameterSocket, &PipeParameterSocket::dataArrived, this, &PipeInstrumentDriver::OnParamReceived);
    connect(m_ParameterSocket, &PipeParameterSocket::StartFlagArrived, this, [this] (quint16 flag) {
        emit rawMessageArrived(MessagePipeStartFlag, flag);
    });

    connect(m_AScanSocket, &QAbstractSocket::disconnected, this, &PipeInstrumentDriver::OnAScanDisConnected);
    connect(m_MonitorSocket, &QAbstractSocket::disconnected, this, &PipeInstrumentDriver::OnMonitorDisConnected);
    connect(m_CScanSocket, &QAbstractSocket::disconnected, this, &PipeInstrumentDriver::OnCScanDisConnected);
    connect(m_ParameterSocket, &QAbstractSocket::disconnected, this, &PipeInstrumentDriver::OnParamDisConnected);
}

void PipeInstrumentDriver::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if (hasNewTCG) {
        bool enable = localParameter.value(tcgChannel).dacMode() == UTSetup::DacTcg;
        m_ParameterSocket->setTCG(localParameter.value(tcgChannel).dac().toJson().toArray(), tcgChannel, enable);
    }
}

void PipeInstrumentDriver::OnAScanConnected()
{
    m_AScanSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    OnConnected(0x0001);
}

void PipeInstrumentDriver::OnMonitorConnected()
{
    m_MonitorSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    OnConnected(0x0002);
}

void PipeInstrumentDriver::OnCScanConnected()
{
    m_CScanSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    OnConnected(0x0004);
}

void PipeInstrumentDriver::OnParamConnected()
{
    m_ParameterSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    OnConnected(0x0008);
}

void PipeInstrumentDriver::OnConnected(int mask)
{
    nConnectedStatus |= mask;
    if (nConnectedStatus != 0x000F) {
        return;
    }
    m_ParameterSocket->SetWaveTypeMark(0xFF);

    // 设置全部通道参数
    for (int index = 0; index < NUM_CHANNELS; ++index) {
        setChannelSetup(index, UTSetup());
    }

    //连接成功之后需要立即发送A扫与C扫的数据请求,以及A扫存储请求
    m_MonitorSocket->RequestData();
    m_CScanSocket->RequestData();
    m_AScanSocket->RequestData();
    setInstrumentStatus(InstrumentStatus::Connected);
}

void PipeInstrumentDriver::OnAScanDisConnected()
{
    OnDisconnected(0xfffe);
}

void PipeInstrumentDriver::OnMonitorDisConnected()
{
    OnDisconnected(0xfffd);
}

void PipeInstrumentDriver::OnCScanDisConnected()
{
    OnDisconnected(0xfffb);
}

void PipeInstrumentDriver::OnParamDisConnected()
{
    OnDisconnected(0xfff7);
}

void PipeInstrumentDriver::OnDisconnected(int mask)
{
    nConnectedStatus &= mask;
    if (nConnectedStatus != 0) {
        return;
    }
    setInstrumentStatus(InstrumentStatus::Disconnected);
}

void PipeInstrumentDriver::OnAScanReceived(UTDataSegment dataSegment)
{
    emit dataArrived(dataSegment);
}

void PipeInstrumentDriver::OnMonitorReceived(UTDataFrame frame)
{
    frame.Context.setDelay(localParameter.value(frame.Context.GetChannelID()).delay());
    frame.headerId = pipeDetectDataHeaderId;
    emit monitorDataArrived(frame);
}

void PipeInstrumentDriver::OnCScanReceived(UTDataSegment dataSegment)
{
    UTDataSegment segment;
    int channelIndex = dataSegment.first().Context.GetChannelID();
    const int averageSize = averageCount.value(channelIndex, -1);

    if (averageSize > 1 && uint(channelIndex) >= uint(sum.size())) {
        return;
    }

    for (auto &frame: dataSegment) {
        frame.headerId = pipeDetectDataHeaderId;
        /* 滤波数量大于1 && 通道有效 */
        if (averageSize <= 1) {
            segment << frame;
            continue;
        }

        /* I1和I2闸门未触发, 数据不参与平均 */
        if (qFuzzyIsNull(frame.gateTime(0)) || qFuzzyIsNull(frame.gateTime(3))) {
            continue;
        }

        CDataFrame &sumFrame = sum[channelIndex];
        for (int index = 0; index < NUM_FEATURES; ++index) {
            sumFrame[index] += frame.CData[index];
        }
        sumCount[channelIndex]++;

        int count = sumCount[channelIndex];
        if (count >= averageSize) {
            for (int index = 0; index < NUM_FEATURES; ++index) {
                frame.CData[index] = sumFrame[index] / count;
            }
            segment << frame;

            sumFrame.fill(0, NUM_FEATURES);
            sumCount[channelIndex] = 0;
        }
    }

    emit dataArrived(segment);
}

void PipeInstrumentDriver::OnParamReceived(UTDataSegment dataSegment)
{
    emit dataArrived(dataSegment);
}

void PipeInstrumentDriver::setChannelSetup(int channel, const UTSetup &setup)
{
    if (status.state != InstrumentStatus::Connected || channel >= localParameter.size()) {
        return;
    }

    qint32 index = channel;
    if (index < localParameter.size()) {
       localParameter.insert(index, setup);
    }

    if (index > 24) {
        m_ParameterSocket->SetParameter(localParameter.value(index), localParameter.value(index - 5), channel, index);
    } else if (index > 19 && index < 25) {
        m_ParameterSocket->SetParameter(localParameter.value(index + 5), localParameter.value(index), channel, index);
    } else {
        m_ParameterSocket->SetParameter(localParameter.value(index), localParameter.value(index), channel, index);
    }

    if (channel < averageCount.count()) {
        averageCount[channel] = localParameter.value(index).sampleAverage();
    }

    QTime curTime = QTime::currentTime();
    if (lastTime.msecsTo(curTime) > 50) {
        hasNewTCG = false;
        bool enable = localParameter.value(index).dacMode() == UTSetup::DacTcg;
        m_ParameterSocket->setTCG(localParameter.value(index).dac().toJson().toArray(), channel, enable);
    } else {
        hasNewTCG = true;
        tcgChannel = channel;
        tcgArray = localParameter.value(index).dac().toJson().toArray();
    }
    m_ParameterSocket->SetPRF(localParameter.value(index).prf());
}

void PipeInstrumentDriver::setDataLevel(int level, int channel)
{
    m_AScanSocket->SetDataLevel(level);
    m_MonitorSocket->SetDataLevel(level);
    m_CScanSocket->SetDataLevel(level);

    dataLevel = level;
    emit DataLevelChanged(level, channel);
}

void PipeInstrumentDriver::resetPose()
{
}

void PipeInstrumentDriver::setActiveChannel(int channelIndex)
{
    if (channelIndex < 20 || channelIndex > 31) {
        return;
    }

    bool isFlaw = true;
    if (channelIndex > 24) {
        channelIndex -= 5;
        isFlaw = false;
    }

    quint16 hardChannel = 1 + (channelIndex / 4);   // 由于屏蔽了一个硬通道，要加上1

    if (isFlaw) {
        selChannelMark = (0x1 << hardChannel) | selChannelMark;
    } else {
        selChannelMark = (~(0x1 << hardChannel)) & selChannelMark;
    }

    if (nConnectedStatus == 0x000F) {
        m_ParameterSocket->SetWaveTypeMark(selChannelMark);
    }
}

void PipeInstrumentDriver::setPRF(quint16 prf)
{
    if (m_ParameterSocket) {
        m_ParameterSocket->SetPRF(prf);
    }
}

void PipeInstrumentDriver::sendRawMessage(int type, const QVariant &message)
{
    if (type == MessageEncoder) {
        m_ParameterSocket->SetCoderMark(message.toInt());
    }
}
/*!
 * \brief PipeInstrumentDriver::InitFilter
 * \param channelCount:仪器通道总数量
 * 根据通道数量初始化滤波功能的相关容器
 */
void PipeInstrumentDriver::ResetChannelCount(int channelCount)
{
    averageCount.fill(1, channelCount);

    sumCount.fill(0, channelCount);
    sum.fill(CDataFrame(NUM_FEATURES, 0), channelCount);
}

InstrumentInfo PipeInstrumentDriver::instrumentInfo() const
{
    return m_info;
}

CDataTypeList PipeInstrumentDriver::GetCDataType()
{
    return CDataType;
}

UTDataHeader PipeInstrumentDriver::InitDataHeader()
{
    UTDataHeader header("PipeDetect");
    for (int gate = 0; gate < NUM_GATES; ++gate) {
        header.SetType(gate * 2,     gate, GateMaxPeak,  DataTime, tr("最大波峰时间"),  0);
        header.SetType(gate * 2 + 1, gate, GateMaxPeak,  DataAmp,  tr("最大波峰高度"), -1);
    }
    return header;
}
