#include "UTChannel.h"
#include "UTInstrument.h"
#include "UTSetup.h"
#include "UltrasonicManager.h"
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <QLoggingCategory>
#include <QRegularExpression>

static Q_LOGGING_CATEGORY(debug, "ultrasonic.channel")

UTChannel::UTChannel(QObject *parent) :
    QObject(parent)
{
}

QString UTChannel::name() const
{
    if (m_config.name.isEmpty()) {
        auto *i = instrument();
        if (i) {
            return tr("通道 %1 (%2)").arg(channelId() + 1).arg(i->name());
        } else {
            return tr("通道 %1").arg(channelId() + 1);
        }
    }
    return m_config.name;
}

QString UTChannel::description() const
{
    return m_description;
}

UTInstrument *UTChannel::instrument() const
{
    return m_instrument;
}

void UTChannel::setSetup(const UTSetup &setup)
{
    if (m_setup == setup) {
        return;
    }
    m_setup = setup;
    // 绑定完成需要发送绑定改变信号（绑定改变需要重新下发通道参数）
    if (m_instrument != nullptr) {
        m_instrument->setChannelSetup(instrumentIndex(), setup);
    }
    emit setupChanged(setup);
}

Probe UTChannel::probe() const
{
    return m_probe;
}

void UTChannel::addTransform(QMatrix4x4 &matrix, const UTPose &pose, qreal waterPath)
{
    m_probe.addTransform(matrix, pose, waterPath);
}

QList<QMatrix4x4> UTChannel::transformChain(const UTPose &pose, qreal waterPath) const
{
    return m_probe.transformChain(pose, waterPath);
}

QUuid UTChannel::probeUuid() const
{
    return m_probe.uuid;
}

UTSetup UTChannel::setup() const
{
    return m_setup;
}

int UTChannel::instrumentIndex() const
{
    return m_config.identifier.channel;
}

void UTChannel::refreshInstrumentBinding()
{
    UTInstrument *instrument = UltrasonicManager::instrument(m_config.identifier.instrument);
    if (instrument != m_instrument) {
        if (m_instrument) {
            std::replace(m_instrument->m_channels.begin(), m_instrument->m_channels.end(), this, static_cast<UTChannel *>(nullptr));
        }
        if (instrument) {
            int id = m_config.identifier.channel;
            if (uint(id) < uint(instrument->m_channels.count())) {
                instrument->m_channels[id] = this;
            }
        }
        m_instrument = instrument;
    }
}

ChannelConfig UTChannel::config() const
{
    return m_config;
}

int UTChannel::channelId() const
{
    return m_config.channelId;
}

UTDataFrame UTChannel::lastFrame() const
{
    return m_lastFrame;
}

void UTChannel::setName(const QString &name)
{
    m_config.name = name;
}

/*! 设置数据保存模式 */
void UTChannel::setDataLevel(int level)
{
    if (m_instrument == nullptr) {
        return;
    }
    IInstrumentDriver *driver = m_instrument->instrumentDriver();
    if (driver == nullptr) {
        return;
    }
    driver->setDataLevel(level, instrumentIndex());
}

void UTChannel::fromJson(const QJsonObject &json)
{
    auto setup = convertFromJson<UTSetup>(json["Setup"]);
    setSetup(setup);
    auto probe = convertFromJson<Probe>(json["Probe"]);
    setProbe(probe);
}

QJsonValue UTChannel::toJson() const
{
    QJsonObject json;
    json["ChannelConfig"] = convertToJson(m_config);
    json["Setup"] = convertToJson(m_setup);
    json["Probe"] = convertToJson(m_probe);
    return json;
}

void UTChannel::setChannelId(int id)
{
    if (m_config.channelId == id) {
        return;
    }
    if (UltrasonicManager::channel(id) != nullptr) {
        qCWarning(debug) << "Failed setting channel id" << id << ": already exists";
        return;
    }

    m_config.channelId = id;
    UltrasonicManager::instance()->refreshChannels();
}

void UTChannel::setConfig(const ChannelConfig &config)
{
    if (m_config == config) {
        return;
    }
    m_config = config;

    refreshInstrumentBinding();
    emit configChanged(m_config);
}

void UTChannel::setDescription(const QString &description)
{
    m_description = description;
}

void UTChannel::setProbe(const Probe &probe)
{
    if (m_probe == probe) {
        return;
    }
    m_probe = probe;
    emit probeChanged(m_probe);
}
