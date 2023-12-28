#include "TemperatureManager.h"

#include <QRegularExpression>
#include <lua.hpp>

//Q_LOGGING_CATEGORY(debug, "temp.manager")

static TemperatureManager *m_instance = Q_NULLPTR;

DataSourcePath::DataSourcePath(const QString &path)
{
    QStringList id = path.split(':');
    source = KeyToEnum(id.value(0), None);
    channel = id.value(1).toInt();
    feature = id.value(2).toInt();
}

QString DataSourcePath::toString() const
{
    if (source == Ultrasonic) {
        return QString("%1:%2:%3").arg(EnumToKey(source)).arg(channel).arg(feature);
    } else if (source == ThermalCoupler) {
        return QString("%1:%2").arg(EnumToKey(source)).arg(channel);
    } else {
        return EnumToKey(source);
    }
}

void TemperatureSource::setValue(qreal value)
{
    m_value = value;
    updateColumn(ColValue);
}

void TemperatureSource::onUltrasonicData(const UTSharedDataFrame &frame)
{
    qreal time = frame->gateTime(m_path.feature);
    setValue(m_instance->evaluate(time, m_func));
}

void TemperatureSource::onThermalData(const QList<qreal> &data)
{
    qreal time = data.value(m_path.channel, Q_SNAN);
    setValue(m_instance->evaluate(time, m_func));
}

QVariant TemperatureSource::data(int column, int role) const
{
    if (column == ColPath) {
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return m_path.toString();
        }
    } else if (column == ColValue) {
        if (role == Qt::DisplayRole) {
            return m_value;
        }
    } else if (column == ColFunc) {
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return m_func;
        }
    }
    return QVariant();
}

bool TemperatureSource::setData(int column, const QVariant &data, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }
    if (column == ColPath) {
        setPath(data.toString());
    } else if (column == ColFunc) {
        m_func = data.toString().toLatin1();
    }
    return false;
}

Qt::ItemFlags TemperatureSource::flags(int column) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (column == ColPath || column == ColFunc) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

DataSourcePath TemperatureSource::path() const
{
    return m_path;
}

void TemperatureSource::setPath(const DataSourcePath &path)
{
    m_path = path;
    setValue(Q_SNAN);
    if (obj) {
        disconnect(obj, 0, this, 0);
    }

    if (m_path.source == DataSourcePath::Ultrasonic) {
        UTChannel *channel = UltrasonicManager::channel(m_path.channel);
        if (channel) {
            connect(channel, &UTChannel::MonitorDataArrived, this, &TemperatureSource::onUltrasonicData);
        }
        obj = channel;
    } else if (m_path.source == DataSourcePath::ThermalCoupler) {
        obj = &m_instance->at;
        connect(&m_instance->at, &AT4500::dataArrived, this, &TemperatureSource::onThermalData);
    }
}

TemperatureManager::TemperatureManager(QObject *parent) :
    QObject(parent)
{
    m_instance = this;

    L = luaL_newstate();
    luaL_openlibs(L);

    sources = new Utils::UniformTreeModel<TemperatureSource>(this);

    connect(UltrasonicManager::instance(), &UltrasonicManager::channelsChanged, this, &TemperatureManager::onChannelsChanged);
}

TemperatureManager::~TemperatureManager()
{
    lua_close(L);
}

TemperatureManager *TemperatureManager::instance()
{
    return m_instance;
}

Utils::UniformTreeModel<TemperatureSource> *TemperatureManager::model()
{
    return m_instance->sources;
}

TemperatureSource *TemperatureManager::addSource(const QString &path)
{
    auto *source = new TemperatureSource(this);
    source->setPath(path);
    sources->rootItem()->appendChild(source);
    emit sourcesChanged();
    return source;
}

void TemperatureManager::removeSource(const QModelIndex &index)
{
    auto *item = sources->itemForIndex(index);
    if (item) {
        delete sources->takeItem(item);
        emit sourcesChanged();
    }
}

qreal TemperatureManager::evaluate(qreal v, const QByteArray &func)
{
    if (func.isEmpty()) {
        return v;
    }
    QByteArray f = "local v = ... return " + func;
    if (luaL_loadstring(L, f.data())) {
        qCDebug(debug) << "Lua error:" << lua_tostring(L, -1);
        lua_pop(L, 1);
        return v;
    }
    lua_pushnumber(L, v);
    if (lua_pcall(L, 1, 1, -2)) {
        qCDebug(debug) << "Lua runtime error:" << lua_tostring(L, -1);
        lua_pop(L, 1);
        return v;
    }
    qreal ret = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return ret;
}

QList<qreal> TemperatureManager::values() const
{
    QList<qreal> values;
    sources->rootItem()->forFirstLevelChildren([&](TemperatureSource *source) {
        values << source->m_value;
    });
    return values;
}

void TemperatureManager::saveSettings(QSettings *setting) const
{
    setting->beginWriteArray("sources");
    for (int index = 0; index < sources->rootItem()->childCount(); ++index) {
        setting->setArrayIndex(index);
        auto *source = sources->rootItem()->childAt(index);
        setting->setValue("path", source->path().toString());
        setting->setValue("func", source->m_func);
    }
    setting->endArray();

    setting->setValue("serialPort", at.portName());
}

void TemperatureManager::loadSettings(QSettings *setting)
{
    sources->clear();
    emit sourcesChanged();

    int count = setting->beginReadArray("sources");
    for (int index = 0; index < count; ++index) {
        setting->setArrayIndex(index);
        auto *source = addSource(setting->value("path").toString());
        source->m_func = setting->value("func").toByteArray();
    }
    setting->endArray();

    at.setPortName(setting->value("serialPort", "COM1").toString());
}

void TemperatureManager::onChannelsChanged()
{
    sources->rootItem()->forFirstLevelChildren([](TemperatureSource *source) {
        source->setPath(source->path().toString());
    });
}
