#include "UTInstrument.h"
#include "UTChannel.h"
#include "UTSetup.h"
#include "UltrasonicManager.h"
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <QLoggingCategory>
#include <QPlainTextEdit>
#include <Ultrasonic/InstrumentSetup.h>
#include <allrising/Util.h>

static Q_LOGGING_CATEGORY(debug, "ultrasonic.instrument");

UTInstrument::UTInstrument(QObject *parent) :
    QObject(parent),
    m_messageLog(new QTextDocument(this))
{
    m_messageLog->setDocumentLayout(new QPlainTextDocumentLayout(m_messageLog));

    m_instrumentStatus.state = InstrumentStatus::Disconnected;
}

UTInstrument::~UTInstrument()
{
    destroyDriver();
}

QString UTInstrument::name() const
{
    return m_config.name;
}

void UTInstrument::setConfig(const InstrumentConfig &config)
{
    if (m_config == config) {
        return;
    }

    m_config = config;
    destroyDriver();
    initDriver();
    emit configChanged(m_config);
}

void UTInstrument::setSetup(const InstrumentSetup &setup)
{
    if (m_setup == setup) {
        return;
    }

    m_setup = setup;
    emit setupChanged(m_setup);
}

void UTInstrument::Connect()                                                    /* 连接到远程仪器。检查是否存在Driver，如果不存在，先InitDriver()，然后连接 */
{
    if (driver == nullptr) {
        initDriver();
    }
    if (driver != nullptr) {
        QMetaObject::invokeMethod(driver, "Connect");
    }
}

void UTInstrument::Disconnect()
{
    if (!driver) {
        return;
    }
    QMetaObject::invokeMethod(driver, "Disconnect");
}

void UTInstrument::initDriver()
{
    if (driver) {
        return;
    }
    const QString deviceType = m_config.deviceType;
    driver = UltrasonicManager::createDriver(deviceType);

    if (!driver) {
        qCCritical(debug) << "Invalid device type " << deviceType;
        return;
    }

    driver->setObjectName(QString("InstrumentDriver%1").arg(m_index));
    driver->setHostAddr(m_config.address);

    connect(driver, &IInstrumentDriver::instrumentStatusChanged, this, &UTInstrument::setInstrumentStatus);
    connect(driver, &IInstrumentDriver::messageReceived, this, &UTInstrument::onMessageReceived);
    connect(driver, &IInstrumentDriver::monitorDataArrived, this, &UTInstrument::onMonitorDataArrived);
    connect(driver, &IInstrumentDriver::dataArrived, this, &UTInstrument::onDataArrived);
    connect(driver, &IInstrumentDriver::rawMessageArrived, this, &UTInstrument::rawMessageArrived);
    connect(driver, &IInstrumentDriver::instrumentInfoChanged, this, &UTInstrument::setInstrumentInfo);
    while (m_channels.size() < channelCount()) {
        m_channels << nullptr;
    }
    setInstrumentInfo(driver->instrumentInfo());

    driver->setParent(nullptr);
    driver->moveToThread(&driverThread);
    driverThread.start();
}

void UTInstrument::destroyDriver()
{
    setInstrumentStatus(InstrumentStatus());
    setInstrumentInfo(InstrumentInfo());
    qCDebug(debug) << "Stopping driver" << m_index;
    QMetaObject::invokeMethod(driver, "deleteLater", Qt::BlockingQueuedConnection);
    driverThread.quit();
    driverThread.wait();
    driver = nullptr;
}

UTChannel *UTInstrument::channel(int index) const
{
    return m_channels.value(index);
}

QList<UTChannel *> UTInstrument::channels() const
{
    return m_channels;
}

InstrumentConfig UTInstrument::config() const
{
    return m_config;
}

InstrumentStatus UTInstrument::instrumentStatus() const
{
    return m_instrumentStatus;
}

InstrumentInfo UTInstrument::instrumentInfo() const
{
    return m_info;
}

InstrumentSetup UTInstrument::setup() const
{
    return m_setup;
}

QTextDocument *UTInstrument::messageLog() const
{
    return m_messageLog;
}

void UTInstrument::loadJson(const QJsonObject &json)
{
    InstrumentConfig config;
    if (json.contains("config")) {
        config = convertFromJson<InstrumentConfig>(json["config"]);
    } else {
        config.name = json["Name"].toString();
        config.deviceType = json["Device"].toString();
        config.address = json["IP"].toString();
        config.channelCount = json["NumChannels"].toInt();
    }
    setConfig(config);
    setSetup(convertFromJson<InstrumentSetup>(json["setup"]));
}

QJsonObject UTInstrument::toJson() const
{
    QJsonObject json;

    json["config"] = convertToJson(m_config);
    json["setup"] = convertToJson(m_setup);

    return json;
}

int UTInstrument::channelCount() const
{
    return m_config.channelCount;
}

void UTInstrument::setInstrumentStatus(const InstrumentStatus &status)
{
    if (m_instrumentStatus != status) {
        m_instrumentStatus = status;
        emit instrumentStatusChanged(m_instrumentStatus);
    }
}

void UTInstrument::setInstrumentInfo(const InstrumentInfo &info)
{
    if (m_info == info) {
        return;
    }
    m_info = info;
    emit instrumentInfoChanged(m_info);
}

void UTInstrument::onMonitorDataArrived(UTDataFrame frame)
{
    m_lastFrame = frame;

    int channelIndex = frame.Context.GetChannelID();
    UTChannel *channel = m_channels.value(channelIndex);
    if (channel) {
        frame.Context.SetChannelID(channel->channelId());
        emit channel->MonitorDataArrived(frame);
    }
}

void UTInstrument::onDataArrived(UTDataSegment segment)
{
    if (segment.isEmpty()) {
        return;
    }
    int channelIndex = segment.first().Context.GetChannelID();
    UTChannel *channel = m_channels.value(channelIndex);
    if (channel) {
        int channelId = channel->channelId();
        for (auto &frame : segment) {
            frame.Context.SetChannelID(channelId);
        }
        emit channel->dataArrived(segment);
        emit dataArrived(segment);
    }
}

void UTInstrument::setChannelSetup(int channel, const UTSetup &setup)
{
    QMetaObject::invokeMethod(driver, "setChannelSetup", Q_ARG(int, channel), Q_ARG(UTSetup, setup));
}

void UTInstrument::onMessageReceived(const QString &text)
{
    QTextCursor cursor(m_messageLog);
    cursor.movePosition(QTextCursor::End);
    if (!cursor.atBlockStart()) {
        cursor.insertBlock();
    }
    cursor.insertText(text);
}

void UTInstrument::setActiveChannel(int channel)
{
    // 当前通道改变，需要初始化最新当前通道参数，使之上发数据
    if (driver && (m_channels.count() > channel)) {
        QMetaObject::invokeMethod(driver, "setActiveChannel", Q_ARG(int, channel));
    }
}

void UTInstrument::resetPose()
{
    ARS_CHECK(driver);
    QMetaObject::invokeMethod(driver, "resetPose");
}

void UTInstrument::sendCommand(const QString &command)
{
    QMetaObject::invokeMethod(driver, "sendCommand", Q_ARG(QString, command));
}

void UTInstrument::sendRawMessage(int type, const QVariant &message)
{
    QMetaObject::invokeMethod(driver, "sendRawMessage", Q_ARG(int, type), Q_ARG(QVariant, message));
}

/*! 设置标准的运动控制器位置 */
void UTInstrument::SetStandardPos(const UTPose &pos)
{
    ARS_CHECK(driver);
    if (m_lastFrame.isEmpty()) {
        return;
    }
    UTPose offset(pos.count());
    UTPose currentPose = m_lastFrame.Pose;
    m_lastFrame = {};
    for (int index = 0; index < pos.count(); ++index) {
        Axis *axis = MotionManager::axis(index);
        if (!axis || axis->config().datumInput >= 0) {                     // 不可被复位的轴不需要添加偏移
            offset[index] = pos[index] - currentPose.value(index);
        }
    }
    QMetaObject::invokeMethod(driver, "setPoseOffset", Q_ARG(UTPose, offset));
}

/*! 获取仪器驱动 */
IInstrumentDriver *UTInstrument::instrumentDriver() const
{
    return driver;
}
