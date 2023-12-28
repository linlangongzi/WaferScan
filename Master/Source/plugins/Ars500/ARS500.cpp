#include "ARS500.h"
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <QRegularExpression>
#include <QSet>
#include <QtEndian>
#include <Ultrasonic/InstrumentSetup.h>
#include <Ultrasonic/UTDataHeader.h>
#include <Ultrasonic/UTSetup.h>
#include <allrising/Util.h>

using namespace Ars500;

const QString UTDriverARS500::deviceType = "ARS500";
const UTDataHeader UTDriverARS500::header = UTDriverARS500::InitDataHeader();

UTDataHeader UTDriverARS500::InitDataHeader()
{
    UTDataHeader header;
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

UTDriverARS500::UTDriverARS500(const QJsonArray &config, QObject *parent) :
    IInstrumentDriver(parent),
    currentChannel(0),
    remoteChannel(-1),
    currentWorkMode(-1)
{

    m_info.channelCount = NUM_CHANNELS;
    m_info.supportAlterSource = true;
    for (int i = 0; i < m_info.channelCount; i++) {
        parsers.push_back( new Parser(config[i].toObject(), this) );
    }
    ASocket = new AScanSocket(parsers, this);
    monitorSocket = new MonitorSocket(parsers, this);
    CSocket = new CScanSocket(parsers, this);
    parameterSocket = new ParameterSocket(parsers, this);

    sockets << ASocket << CSocket << parameterSocket << monitorSocket;

    InitSockets();
    for (int channel = 0; channel < m_info.channelCount; ++channel) {
        isTcgChanged[channel] = false;
        isParameterChanged[channel] = false;
        setDataLevel(MONITOR_ONLY, channel);
    }
    startTimer(platform()->Get("ParameterInterval").toInt(600));
}

void UTDriverARS500::setHostAddr(const QString &ip)
{
    IPAddr = ip;
}

void UTDriverARS500::Connect()
{
    ASocket->connectToHost(IPAddr, ASCAN_PORT);
    monitorSocket->connectToHost(IPAddr, MONITOR_PORT);
    CSocket->connectToHost(IPAddr, CSCAN_PORT);
    parameterSocket->connectToHost(IPAddr, PARAM_PORT);
}

void UTDriverARS500::Disconnect()
{
    foreach (QTcpSocket *socket, sockets) {
        if (socket->state() == QTcpSocket::ClosingState) {
            socket->abort();
        } else {
            socket->disconnectFromHost();
        }
    }
}

void UTDriverARS500::InitSockets()
{
    connect( ASocket, &AScanSocket::dataArrived, this, &UTDriverARS500::OnAScanReceived );
    connect( monitorSocket, &MonitorSocket::dataArrived, this, &UTDriverARS500::OnMonitorReceived );
    connect( CSocket, &CScanSocket::dataArrived, this, &UTDriverARS500::OnCScanReceived );
    connect( parameterSocket, &ParameterSocket::dataArrived, this, &UTDriverARS500::OnParamReceived );

    connect( parameterSocket, &ParameterSocket::VersionUpdate, this, &UTDriverARS500::OnVersionUpdate );

    foreach (QTcpSocket *socket, sockets) {
        connect(socket, &QTcpSocket::stateChanged, this, &UTDriverARS500::onSocketStateChanged);
    }
}

void UTDriverARS500::OnConnected()
{
    qDebug() << "connect successed";
    parameterSocket->RequestVersion();
    // 连接成功之后，将所有通道的标志位置位， 让定时器自动初始化每个通道
    for (int channel = 0; channel < m_info.channelCount; ++channel) {
        isParameterChanged[channel] = true;
//        bTCGChanged[channel] = true;
    }
    // 连接成功之后需要立即发送A扫与C扫的数据请求,以及A扫存储请求
    monitorSocket->RequestData();
    CSocket->RequestData();
    ASocket->RequestData();
    currentWorkMode = -1;
    UpdateWorkMode();
}

/*!
 * \brief UTDriverARS500::OnVersionUpdate 接受版本更新信号
 * \param vers
 */
void UTDriverARS500::OnVersionUpdate(const QString &newVersion)
{
    version = newVersion;
}

void UTDriverARS500::onSocketStateChanged()
{
    QVector<QTcpSocket::SocketState> states;
    states << ASocket->state();
    states << CSocket->state();
    states << monitorSocket->state();
    states << parameterSocket->state();
    QVector<QTcpSocket::SocketState> allConnected(states.size(), QTcpSocket::ConnectedState);
//    qDebug() << "ARS500:" << states;
    if (states == allConnected) {
        OnConnected();
        return setInstrumentStatus(InstrumentStatus::Connected, tr("连接成功"));
    } else if (states.contains(QTcpSocket::UnconnectedState)) {
        return setInstrumentStatus(InstrumentStatus::Disconnected, tr("断开连接"));
    } else if (states.contains(QTcpSocket::HostLookupState) || states.contains(QTcpSocket::ConnectingState)) {
        return setInstrumentStatus(InstrumentStatus::Initializing, tr("正在初始化"));
    }
}

void UTDriverARS500::UpdateWorkMode()
{
    int newWorkMode = 0;
    for (int channel = 0; channel < m_info.channelCount; ++channel) {
        if (localParameter.value(channel).pulserEnable()) {
            newWorkMode |= 1 << channel;
        }
    }
    Q_STATIC_ASSERT_X(NUM_CHANNELS == 2, "WorkMode is designed for 2 channels.");
    if (localParameter[1].alterSource()) {
        newWorkMode = 6;
        currentChannel = 0;
    }
    if (newWorkMode != currentWorkMode) {
        qDebug() << "Sending workmode" << newWorkMode;
        Parser::SendCommand(parameterSocket, CMD_WORK_MODE_SET, 0, 0, qint16(newWorkMode));
        currentWorkMode = newWorkMode;
        if (newWorkMode == 6) {
            remoteChannel = -1;
        }
    }
}

void UTDriverARS500::OnAScanReceived(UTDataSegment dataSegment)
{
//    qDebug() << "UTDriverARS500::OnAScanReceived";
    emit dataArrived(dataSegment);
}

void UTDriverARS500::OnMonitorReceived(UTDataFrame dataFrame)
{
//    qDebug() << "ARS500:MonitorData at channel" << dataframe.Context.GetChannelID();
    emit monitorDataArrived(dataFrame);
}

void UTDriverARS500::OnCScanReceived(UTDataSegment dataSegment)
{
//    qDebug() << "UTDriverARS500::OnCScanReceived";
    emit dataArrived(dataSegment);
}

void UTDriverARS500::OnParamReceived(UTDataSegment dataSegment)
{
    emit dataArrived(dataSegment);
}

void UTDriverARS500::setChannelSetup(int channel, const UTSetup &setup)
{
//    qDebug() << "ARS500::SetChannelParameter" << channel << key << value;
    ARS_CHECK_BOUND_WARNING(0, channel, m_info.channelCount - 1, "UTDriverARS500::SetChannelParameter input channel is out of limit");
    UTSetup localSetup = localParameter.value(channel);
    if (localSetup == setup) {
        return;
    }
    if ( (channel < 0) || ( channel >= localParameter.count() ) ) {
        qWarning() << "UTDriverARS500::setChannelSetup input channel is out of limit";
        return;
    }
    localParameter.replace(channel, setup);
    if (setup.pulserEnable() || setup.alterSource()) {                  /* 更改通道 */
        isParameterChanged[1] = true;                                           /* 关闭关联检测时，需要重新发送2通道配置 */
        return UpdateWorkMode();
    }
    isTcgChanged[channel] = true;
    isParameterChanged[channel] = true;
}

void UTDriverARS500::setDataLevel(int level, int channel)
{
    Q_ASSERT(channel < m_info.channelCount && channel >= 0);
    dataLevel[channel] = level;
    ASocket->SetDataLevel(level, channel);
    monitorSocket->SetDataLevel(level, channel);
    CSocket->SetDataLevel(level, channel);
    emit DataLevelChanged(level, channel);
}

void UTDriverARS500::resetPose()
{
    parameterSocket->ResetPose();
}

/*!
 * \brief UTDriverARS500::InitChannel
 * \param channel
 */
void UTDriverARS500::setActiveChannel(int channel)
{
    ARS_CHECK_BOUND(0, channel, m_info.channelCount - 1);
    currentChannel = channel;                                                   // 设置当前通道， 并且修改下发标志位
    isParameterChanged[channel] = true;
}

InstrumentInfo UTDriverARS500::instrumentInfo() const
{
    return m_info;
}

/*!
 * \brief UTDriverARS500::timerEvent 下发参数信号节流函数
 * \param event 定时器事件
 */
void UTDriverARS500::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    ARS_CHECK(parameterSocket->state() == QAbstractSocket::ConnectedState);
    for (int channel = 0; channel < m_info.channelCount; ++channel) {              /* 更新非当前通道 */
        if (channel == currentChannel) {
            continue;
        }
        ARS_CHECK( UpdateParameters(channel) && UpdateTCG(channel) );
    }
    ARS_CHECK( UpdateParameters(currentChannel, currentChannel != remoteChannel) ); /* 更新当前通道，如果仪器通道不是当前通道，则强制更新 */
    UpdateTCG(currentChannel);
}

/*!
 * \brief UTDriverARS500::UpdateTCG 尝试发送TCG更新数据
 * \param channel 通道号
 * \return true: 发送成功
 *          false: 发送失败
 */
bool UTDriverARS500::UpdateTCG(int channel)
{
    ARS_CHECK_BOUND_RETURN(0, channel, m_info.channelCount - 1, true);
    ARS_CHECK_RETURN(isTcgChanged[channel], true);
    parameterSocket->setTCG( localParameter.value(channel), channel, localParameter.value(channel).dacMode() == UTSetup::DacTcg);
    isTcgChanged[channel] = false;
    return false;
}

/*!
 * \brief UTDriverARS500::UpdateParameters 尝试发送更新参数消息
 * \param channel 通道号
 * \return  true: 更新成功
 *          false: 更新失败
 */
bool UTDriverARS500::UpdateParameters(int channel, bool forceUpdate)
{
    ARS_CHECK_BOUND_RETURN(0, channel, m_info.channelCount - 1, true);
    ARS_CHECK_RETURN(isParameterChanged[channel] || forceUpdate, true);          /* 条件检查：参数有改变，或者需要切换通道 */
    parameterSocket->SendParameter(localParameter, channel);
    remoteChannel = channel;
    isParameterChanged[channel] = false;
//    qDebug() << "UTDriverARS500::UpdateParamerets Channel" << channel;
    return false;
}

/*! 设置仪器编码器偏移 */
void UTDriverARS500::setPoseOffset(const UTPose &offset)
{
    foreach (Parser *parser, parsers) {
        parser->UpdateChannelOffset(offset);
    }
}
