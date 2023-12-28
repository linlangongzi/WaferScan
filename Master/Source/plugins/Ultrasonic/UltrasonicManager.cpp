#include "UTSetup.h"
#include "UltrasonicManager.h"
#include <Core/ICore.h>
#include <Core/Platform.h>
#include <QDir>
#include <QFileInfo>
#include <QLoggingCategory>
#include <QtMath>
#include <Ultrasonic/UltrasonicConstants.h>
#include <allrising/Util.h>
#include <extensionsystem/pluginmanager.h>
#include <utils/algorithm.h>

static UltrasonicManager *m_instance = nullptr;
static Q_LOGGING_CATEGORY(debug, "ut.manager")

using namespace Core;
using ExtensionSystem::PluginManager;
using namespace Ultrasonic;

UltrasonicManager::UltrasonicManager(QObject *parent) :
    QObject(parent),
    m_materials(new MaterialModel(this)),
    m_probes(new ProbeModel(this))
{
    m_instance = this;
    connect(m_probes, &ProbeModel::probesChanged, this, &UltrasonicManager::reloadChannelProbe);
}

UltrasonicManager::~UltrasonicManager()
{
    QList<UTInstrument *> instruments;
    qSwap(m_instruments, instruments);
//    refreshChannels();
    emit instrumentsChanged();

    qDeleteAll(instruments);

    m_instance = nullptr;
}

UltrasonicManager *UltrasonicManager::instance()
{
    return m_instance;
}

QMap<int, UTSetup> UltrasonicManager::setups()
{
    QMap<int, UTSetup> setups;
    const auto &channels = m_instance->m_channels;
    for (auto it = channels.cbegin(); it != channels.cend(); ++it) {
        setups[it.key()] = it.value()->setup();
    }
    return setups;
}

IInstrumentDriver *UltrasonicManager::createDriver(const QString &name)
{
    auto *factory = Utils::findOrDefault(IInstrumentDriverFactory::allFactories(), [name] (auto *f) {
        return f->name() == name;
    });
    if (factory != nullptr) {
        return factory->create();
    }
    return nullptr;
}

QStringList UltrasonicManager::availableDriverNames()
{
    auto factories = IInstrumentDriverFactory::allFactories();
    return Utils::transform(factories, [] (auto *factory) {return factory->name();});
}

QList<UTInstrument *> UltrasonicManager::instruments()
{
    return m_instance->m_instruments;
}

UTInstrument *UltrasonicManager::instrument(int index)
{
    return m_instance->m_instruments.value(index);
}

UTInstrument *UltrasonicManager::instrument(const QString &name)
{
    return Utils::findOrDefault(m_instance->m_instruments, [&] (auto *instrument) {
        return instrument->name() == name;
    });
}

UTInstrument *UltrasonicManager::createInstrument()
{
    auto *instrument = new UTInstrument(m_instance);
    m_instance->m_instruments << instrument;
    m_instance->refreshInstruments();

    return instrument;
}

void UltrasonicManager::removeInstrument(UTInstrument *instrument)
{
    if (m_instance->m_instruments.contains(instrument)) {
        m_instance->m_instruments.removeAll(instrument);
        m_instance->refreshInstruments();
        instrument->deleteLater();
    }
}

int UltrasonicManager::channelCount()
{
    return m_instance->m_channels.count();
}

QList<UTChannel *> UltrasonicManager::channels()
{
    return m_instance->m_channels.values();
}

UTChannel *UltrasonicManager::channel(int channelId)
{
    return m_instance->m_channels.value(channelId);
}

UTChannel *UltrasonicManager::channel(const ChannelIdentifier &id)
{
    auto end = std::end(m_instance->m_channels);
    auto it = std::find_if(std::begin(m_instance->m_channels), end, [&] (UTChannel *channel) {
        return channel->config().identifier == id;
    });
    return (it == end) ? nullptr : *it;
}

UTSetup UltrasonicManager::channelSetup(int channelId)
{
    UTChannel *c = channel(channelId);
    if (c != nullptr) {
        return c->setup();
    }
    return {};
}

QString UltrasonicManager::channelName(int channelId)
{
    QString name = tr("通道 %1").arg(channelId + 1);
    if (!isValidId(channelId)) {
        return name;
    }

    UTChannel *c = channel(channelId);
    if (c != nullptr) {
        return c->name();
    }
    return name;
}

MaterialModel *UltrasonicManager::materialModel()
{
    return m_instance->m_materials;
}

ProbeModel *UltrasonicManager::probeModel()
{
    return m_instance->m_probes;
}

QList<MathExpression> UltrasonicManager::mathExpressions()
{
    return m_instance->m_mathExpressions;
}

qreal UltrasonicManager::ampToDecibel(qreal amp, qreal base)
{
    return 20 * qLn(amp / base) / qLn(10);
}

qreal UltrasonicManager::decibelToAmp(qreal decibel, qreal base)
{
    return qPow(10, decibel / 20) * base;
}

QString UltrasonicManager::ampToDecibelText(qreal amp, qreal base)
{
    return QString("%1 dB").arg(ampToDecibel(amp, base), 0, 'f', m_instance->m_ampPrecision);
}

QString UltrasonicManager::ampText(qreal amp, bool useDecibel, qreal base)
{
    if (useDecibel) {
        return ampToDecibelText(amp, base);
    }
    return QString("%1 %").arg(amp, 0, 'f', m_instance->m_ampPrecision);
}

bool UltrasonicManager::removeChannel(int channelId)
{
    if (!isValidId(channelId)) {
        return false;
    }
    UTChannel *channel = m_channels.take(channelId);
    if (channel != nullptr) {
        channel->deleteLater();
    }

    emit channelsChanged();

    return true;
}

UTChannel *UltrasonicManager::createChannel(const ChannelConfig &config)
{
    if (isValidId(config.channelId)) {
        return nullptr;
    }
    auto *channel = new UTChannel(this);
    m_channels[config.channelId] = channel;
    channel->setConfig(config);
    int dataLevel = platform()->Get("DataLevel").toInt();
    channel->setDataLevel(dataLevel == 0 ? IInstrumentDriver::ASCAN_ONLY : dataLevel);
    emit channelsChanged();

    return channel;
}

void UltrasonicManager::loadConfig()
{
    QString errorString;

    QList<Material> materials = ReadFileAsJsonArray<Material>(ICore::settingsPath() + P_MATERIAL, &errorString);
    if (materials.isEmpty()) {
        QFileInfo legacy(QString(".") + P_MATERIAL);
        if (legacy.exists()) {
            qCInfo(debug) << "Loading legacy materials";
            materials = ReadFileAsJsonArray<Material>(QString(".") + P_MATERIAL);
            QDir().remove(legacy.absoluteFilePath());
        }
    }
    if (materials.isEmpty()) {
        qCInfo(debug) << "Loading default material list";
        materials = ReadFileAsJsonArray<Material>(":/Ultrasonic/Material.json");
    }
    m_materials->load(materials);

    QList<Probe> probes = ReadFileAsJsonArray<Probe>(ICore::settingsPath() + P_PROBE, &errorString);
    if (probes.isEmpty()) {
        QFileInfo legacy(QString(".") + P_PROBE);
        if (legacy.exists()) {
            qCInfo(debug) << "Loading legacy probe";
            probes = ReadFileAsJsonArray<Probe>(QString(".") + P_PROBE);
            QDir().remove(legacy.absoluteFilePath());
        }
    }
    if (probes.isEmpty()) {
        qCInfo(debug) << "Loading default probe list";
        probes = ReadFileAsJsonArray<Probe>(":/Ultrasonic/Probe.json");
    }
    m_probes->setProbes(probes);

    if (QFileInfo::exists(ICore::settingsPath() + P_INSTRUMENT)) {
        QList<UTInstrument *> instruments;
        qSwap(m_instruments, instruments);
        emit instrumentsChanged();
        qDeleteAll(instruments);

        QJsonArray array = ReadFileAsJson(ICore::settingsPath() + P_INSTRUMENT, &errorString).toArray();
        for (const auto &value: array) {
            UTInstrument *instrument = createInstrument();
            instrument->loadJson(value.toObject());
//            instrument->LoadCompleted();
//            instrument->setConfig(convertFromJson<InstrumentConfig>(value));
        }

        QSettings *settings = ICore::settings();
        settings->beginGroup(G_ULTRASONIC);
        if (settings->contains(K_TIMEBASE_PRECISION)) {
            UTSetupTimebase::setTimebasePrecision(settings->value(K_TIMEBASE_PRECISION).toInt());
        }
        if (settings->contains(K_AMPLITUDE_PRECISION)) {
            m_ampPrecision = settings->value(K_AMPLITUDE_PRECISION).toInt();
        }
        settings->endGroup();
    } else {
        loadLegacyConfig();
    }

    QJsonArray channelConfigs = ReadFileAsJson(ICore::settingsPath() + P_CHANNEL_CONFIG, &errorString).toArray();
    if (!channelConfigs.isEmpty()) {
        m_channels.clear();
        for (auto config: channelConfigs) {
            auto channelConfig = convertFromJson<ChannelConfig>(config.toObject()["ChannelConfig"]);
            UTChannel *channel = createChannel(channelConfig);
            channel->fromJson(config.toObject());
//            channel->setSetup(channel->channelId() + 1);
        }
    }

    if (m_instruments.isEmpty() && ICore::isInternalVersion()) {
        qCInfo(debug) << "Adding default instrument";
        UTInstrument *instrument = createInstrument();
        InstrumentConfig config;
        config.name = "仪器 1";
        config.address = "192.168.1.80";
        config.deviceType = "ARS3000";
        config.channelCount = 2;
        instrument->setConfig(config);
        createLegacyChannels();
    }

    refreshChannels();
    emit channelsChanged();
    emit instrumentsChanged();
}

void UltrasonicManager::saveConfig()
{
    QSettings *settings = ICore::settings();
    settings->beginGroup(G_ULTRASONIC);
    settings->setValue(K_TIMEBASE_PRECISION, UTSetupTimebase::timebasePrecision());
    settings->endGroup();

    QString file = ICore::settingsPath() + P_INSTRUMENT;
//    QList<InstrumentConfig> configs = Utils::transform(m_instruments, &UTInstrument::config);
    QJsonArray array;
    for (UTInstrument *instrument: m_instruments) {
        array << instrument->toJson();
    }
    qCDebug(debug) << "Writing config to" << file;
//    WriteFileAsJson(file, convertToJson(configs));
    WriteFileAsJson(file, array);
    qCInfo(debug) << "Finished writing config";

    file = ICore::settingsPath() + P_MATERIAL;
    qCDebug(debug) << "Writing config to" << file;
    WriteFileAsJsonArray<Material>(file, m_materials->save());
    qCInfo(debug) << "Finished writing config";

    file = ICore::settingsPath() + P_PROBE;
    qCDebug(debug) << "Writing config to" << file;
    WriteFileAsJsonArray<Probe>(file, m_probes->probes());
    qCInfo(debug) << "Finished writing config";

    {
        QJsonArray array;
        for (auto channel: m_channels) {
            array << channel->toJson();
        }
        file = ICore::settingsPath() + P_CHANNEL_CONFIG;
        qCDebug(debug) << "Writing config to" << file;
        WriteFileAsJson(file, array);
        qCInfo(debug) << "Finished writing config";
    }
}

void UltrasonicManager::loadLegacyConfig()
{
    QString errorString;

    QJsonArray instruments = ReadFileAsJson("Instrument.json", &errorString).toArray();
    for (const auto &value: instruments) {
        auto *instrument = createInstrument();
        instrument->loadJson(value.toObject());
    }

    createLegacyChannels();

    QDir().remove("Instrument.json");
    saveConfig();
}

void UltrasonicManager::createLegacyChannels()
{
    qDeleteAll(m_channels);
    m_channels.clear();
    emit channelsChanged();

    auto setups = ReadFileAsJsonArray<UTSetup>(P_SETUP);

    int channelId = 0;
    for (auto *instrument: m_instruments) {
        for (int index = 0; index < instrument->channelCount(); ++index) {
            ChannelConfig config;
            config.identifier = ChannelIdentifier(instrument->name(), index);
            config.channelId = channelId;

            auto *channel = createChannel(config);
            channel->setSetup(setups.value(channelId));

            m_channels[channelId] = channel;
            channelId++;
        }
    }
    refreshChannels();
}

void UltrasonicManager::refreshInstruments()
{
    for (int index = 0; index < m_instruments.count(); ++index) {
        m_instruments[index]->m_index = index;
    }
    emit instrumentsChanged();
}

void UltrasonicManager::refreshChannels()
{
    QMap<int, UTChannel *> channels;
    for (auto *channel: m_channels) {
        channels[channel->channelId()] = channel;
    }
    if (channels == m_channels) {
        return;
    }
    emit channelsChanged();
}

void UltrasonicManager::dispatchFrame(const UTDataFrame &frame)
{
    const int channelId = frame.channelId();

    auto *channel = m_channels.value(channelId);
    if (channel->instrument()) {
        emit channel->MonitorDataArrived(frame);
    }
}

void UltrasonicManager::dispatchSegment(const UTDataSegment &segment)
{
    if (segment.isEmpty()) {
        return;
    }

    const int channelId = segment.first().channelId();

    auto *channel = m_channels.value(channelId);
    if (channel->instrument()) {
        emit channel->dataArrived(segment);
    }
}

void UltrasonicManager::reloadChannelProbe()
{
    if (m_probes->probes().isEmpty() || channels().isEmpty()) {
        return;
    }
    foreach (UTChannel *item, channels()) {
        if (item->probeUuid().isNull()) {
            return;
        }
        item->setProbe(m_probes->probe(item->probeUuid()));
    }
}

bool UltrasonicManager::isValidId(int channelId)
{
    return m_instance->m_channels.value(channelId) != nullptr;
}

bool UltrasonicManager::isValidChannel(const QString &instrumentName, int channel, int cycle)
{
    return UltrasonicManager::channel(ChannelIdentifier(instrumentName, channel, cycle)) != nullptr;
}
