#include "MotionManager.h"
#include "Axis.h"
#include <Core/ICore.h>
#include <QDir>
#include <QFileInfo>
#include <QLoggingCategory>
#include <QtMath>
#include <utils/algorithm.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <Motion/MotionConstants.h>

using namespace Core;
using namespace Motion;

static Q_LOGGING_CATEGORY(debug, "motion.manager");

static MotionManager *m_instance = nullptr;

enum PositionColumn {
    ColName,
    ColPosition,
};

MotionManager::MotionManager(QObject *parent) :
    QObject(parent)
{
    Q_ASSERT(m_instance == nullptr);
    m_instance = this;
    m_trio = new TrioDriver(this);
    m_safetyBarrier = new SafetyBarrier(this);
}

MotionManager::~MotionManager()
{
    delete m_safetyBarrier;
    delete m_trio;
    m_instance = nullptr;
}

MotionManager *MotionManager::instance()
{
    return m_instance;
}

void MotionManager::importLegacyConfig()
{
    qCDebug(debug) << "Importing legacy config...";

    QJsonObject platform = ReadFileAsJson("Platform.json").toObject();
    ControllerConfig config = m_trio->config();
    config.host = platform["MotionIP"].toString();                   // FIXME: 2.0 移除对Platform的兼容配置
    m_trio->setConfig(config);

    loadAxisConfig(F_AXIS);
    QFile::remove(F_AXIS);

    loadEndPointConfig(F_ENDPOINT);
    QFile::remove(F_ENDPOINT);

    QJsonObject safeRect = platform["SafeRect"].toObject();
    SafetyBarrierOptions barrier;
    barrier.minDepth = safeRect["EmptyHeight"].toDouble();
    barrier.rect = JsonToRectF(safeRect["Rect"]);
    safetyBarrier()->setOptions(barrier);
}

void MotionManager::loadConfig()
{
    QSettings *settings = ICore::settings();
    QDir path = ICore::settingsPath();
    if (!QFileInfo::exists(path.filePath(F_AXIS))) {
        importLegacyConfig();
        return;
    }

    qCDebug(debug) << "Loading config from" << path.path();
    settings->beginGroup(G_MOTION_SETTINGS);
    ControllerConfig config;
    config.host = settings->value(K_CONTROLLER_HOST).toString();
    config.ioConfig = settings->value(K_CONTROLLER_IO_CONFIG).value<QList<ControllerIoConfig> >();
    m_axisPrecision = settings->value(K_AXIS_PRESITION, m_axisPrecision).toDouble();
    qCInfo(debug) << "Initializing motion device on IP address" << config.host;
    m_trio->setConfig(config);

    MotionManager::safetyBarrier()->setOptions(settings->value(K_CONTROLLER_SAFE_MODULE).value<SafetyBarrierOptions>());

    loadAxisConfig(path.filePath(F_AXIS));

    loadEndPointConfig(path.filePath(F_ENDPOINT));
    simulateDynamicConnection(settings->value(K_CONTROLLER_SIMULATE_DYNAMIC).toBool());
    settings->endGroup();

    QString errorString;
    m_positionModel.clear();
    QJsonValue json = ReadFileAsJson(path.filePath(F_GLOBAL_POSITION), &errorString);
    for (auto value: json.toArray()) {
        auto *item = new NamedPosition;
        JsonToGadget(value, NamedPosition::staticMetaObject, item);
        m_positionModel.rootItem()->appendChild(item);
    }
}

void MotionManager::saveConfig()
{
    auto *settings = ICore::settings();

    settings->beginGroup(G_MOTION_SETTINGS);
    auto *motionDriver = driver();
    ControllerConfig config = motionDriver->config();
    settings->setValue(K_CONTROLLER_HOST, config.host);
    settings->setValue(K_CONTROLLER_IO_CONFIG, QVariant::fromValue(config.ioConfig));
    settings->setValue(K_CONTROLLER_SAFE_MODULE, QVariant::fromValue(safetyBarrier()->options()));
    settings->setValue(K_AXIS_PRESITION, QVariant::fromValue(m_axisPrecision));
    settings->endGroup();

    QJsonArray axisList;
    for (Axis *axis: m_axisList) {
        axisList << axis->toJson();
    }
    WriteFileAsJson(ICore::settingsPath() + "/Axis.json", axisList, true);

    QJsonArray endPointList;
    for (EndPoint *endPoint: m_endpoints) {
        endPointList << endPoint->toJson();
    }
    WriteFileAsJson(ICore::settingsPath() + "/EndPoint.json", endPointList, true);

    QJsonArray positionList;
    m_positionModel.forItemsAtLevel<1>([&](NamedPosition *item) {
        positionList << GadgetToJson(NamedPosition::staticMetaObject, item);
    });
    WriteFileAsJson(ICore::settingsPath() + "/GlobalPosition.json", positionList, true);
}

void MotionManager::loadAxisConfig(const QString &path)
{
    QString errorString;
    QJsonValue json = ReadFileAsJson(path, &errorString);
    if (!json.isArray()) {
        qWarning() << "Failed reading axis config:" << errorString;
        return;
    } else {
        for (auto value: json.toArray()) {
            auto *axis = new Axis(this);
            axis->fromJson(value);
            addAxis(axis);
        }
    }
}

void MotionManager::loadEndPointConfig(const QString &path)
{
    QString errorString;
    QJsonValue json = ReadFileAsJson(path, &errorString);
    for (auto value: json.toArray()) {
        auto *endPoint = new EndPoint(this);
        endPoint->loadJson(value);
        addEndPoint(endPoint);
    }
    if (m_endpoints.isEmpty()) {
        m_globalEndpoint = new EndPoint(this);
        m_endpoints << m_globalEndpoint;
    } else {
        m_globalEndpoint = m_endpoints.first();
    }
    m_globalEndpoint->setName(tr("默认机械臂"));
    updateAxisList();
}

int MotionManager::axisCount()
{
    return m_instance->m_axisList.count();
}

QList<Axis *> MotionManager::axisList()
{
    return m_instance->m_axisList;
}

Axis *MotionManager::axis(const QUuid &uuid)
{
    for (Axis *axis: qAsConst(m_instance->m_axisList)) {
        if (axis->uuid() == uuid) {
            return axis;
        }
    }
    return nullptr;
}

Axis *MotionManager::axis(int index)
{
    return m_instance->m_axisList.value(index);
}

Axis *MotionManager::findAxis(const QString &key, const QVariant &value)
{
    for (Axis *axis: qAsConst(m_instance->m_axisList)) {
        if (MetaType::readGadgetProperty(axis->config(), key) == value) {
            return axis;
        }
    }
    return nullptr;
}

QStringList MotionManager::axisNames()
{
    return Utils::transform(m_instance->m_axisList, [](Axis *axis) {
        return axis->name();
    });
}

int MotionManager::axisIndex(const QUuid &uuid)
{
    return m_instance->m_axisList.indexOf(MotionManager::axis(uuid));
}

bool MotionManager::isEqualWith(qreal value, qreal target)
{
    return qAbs(value - target) < m_instance->m_axisPrecision;
}

int MotionManager::displayPrecision()
{
    return m_instance->m_displayPrecision;
}

void MotionManager::loadAxisList(const QJsonArray &json)
{
    for (const auto &value: json) {
        auto *axis = new Axis(this);
        axis->fromJson(value);
        addAxis(axis);
    }
}

void MotionManager::addAxis(Axis *axis)
{
    if (!axis || m_axisList.contains(axis)) {
        return;
    }

    if (axis->uuid().isNull()) {
        const QUuid uuid = allocateUuid();
        qCInfo(debug) << "Allocated axis uuid:" << axis->config().name << uuid;
        axis->setUuid(uuid);
    }
    axis->setParent(this);
    m_axisList.append(axis);

    updateAxisList();
}

void MotionManager::removeAxis(Axis *axis)
{
    if (!axis || !m_axisList.contains(axis)) {
        return;
    }

    m_axisList.removeAll(axis);
    for (EndPoint *endpoint: qAsConst(m_endpoints)) {
        auto axisList = endpoint->axisList();
        if (axisList.contains(axis)) {
            axisList.removeAll(axis);
            endpoint->setAxisList(axisList);
        }
    }
    axis->deleteLater();

    updateAxisList();
}

QList<EndPoint *> MotionManager::endPoints()
{
    return m_instance->m_endpoints;
}

EndPoint *MotionManager::endPoint(int index)
{
    if (index == 0) {
        return m_instance->m_globalEndpoint;
    }
    return m_instance->m_endpoints.value(index);
}

EndPoint *MotionManager::endPoint(const QUuid &id)
{
    for (EndPoint *endPoint: endPoints()) {
        if (endPoint->uuid() == id) {
            return endPoint;
        }
    }
    return nullptr;
}

void MotionManager::addEndPoint(EndPoint *endPoint)
{
    if ((endPoint == nullptr) || m_endpoints.contains(endPoint)) {
        return;
    }
    if (endPoint->uuid().isNull()) {
        const QUuid uuid = allocateUuid();
        qCInfo(debug) << "Allocated end-point uuid:" << endPoint->name() << uuid;
        endPoint->setUuid(uuid);
    }
    endPoint->setParent(this);
    m_endpoints.append(endPoint);

    emit endPointsChanged();
}

void MotionManager::removeEndPoint(EndPoint *endPoint)
{
    if ((endPoint == nullptr) || !m_endpoints.contains(endPoint)) {
        return;
    }
    if (endPoint == m_globalEndpoint) {
        return;
    }
    m_endpoints.removeAll(endPoint);
    endPoint->deleteLater();

    emit endPointsChanged();
}

QPair<QVector3D, QVector3D> MotionManager::robotPosition(const UTDataFrame &frame)
{
    /*! \todo 完成机器人坐标变换 */
    Q_UNUSED(frame);
    return qMakePair(QVector3D(), QVector3D());
}

NamedPositionModel *MotionManager::positionModel()
{
    return &m_instance->m_positionModel;
}

TrioDriver *MotionManager::driver()
{
    return m_instance->m_trio;
}

SafetyBarrier *MotionManager::safetyBarrier()
{
    return m_instance->m_safetyBarrier;
}

QVector<float> MotionManager::fillPose(const qint32 data[], int size, const QVector<float> &offset, quint32 *direction)
{
    if (m_instance == nullptr) {                                                // TODO: Remove dependency
        return {};
    }
    static quint32 directionDummy = 0;
    if (direction == nullptr) {                                                           /* 确保direction不是空指针 */
        direction = &directionDummy;
    }

    QVector<float> pose;
    pose.reserve(m_instance->m_axisList.count());

    for (int index = 0; index < m_instance->m_axisList.size(); ++index) {
        Axis *axis = m_instance->m_axisList[index];
        int encoder = axis->m_config.encoderId;
        if (uint(encoder) >= uint(size)) {
            pose << 0;
            continue;
        }

        qint32 raw = data[encoder];
        auto isReversed = bool(raw & 0x80000000);                               /* 根据ISO/IEC 14882:1998标准文档，bool的构造体将任意非0值转为true(1) */
        *direction |= quint32(isReversed) << index;                             /* 所以此处位操作是安全的 */
        Q_ASSERT( (quint32(isReversed) & 0x1) == quint32(isReversed) );         /* 关于此特性的断言 */
        raw = ( (raw & 0x40000000) << 1 ) | (raw & 0x7FFFFFFF);
        MotionPose::value_type value = raw / axis->m_config.unitsInstrument;
        value += offset.value(index);

        qreal repeat = axis->m_config.repeatDistance;
        if (qIsFinite(repeat) && repeat > 0) {
            int quotient = qFloor(value / repeat);
            value -= quotient * repeat;
        }
        pose << value;
    }
    return pose;
}

void MotionManager::simulateDynamicConnection(bool enable)
{
    EndPoint *point = endPoint(0);
    if (point == nullptr) {
        return;
    }
    if (enable) {
        connect(point, &EndPoint::PositionUpdated, this, &MotionManager::setStandardPose, Qt::UniqueConnection);
    } else {
        disconnect(point, &EndPoint::PositionUpdated, this, &MotionManager::setStandardPose);
    }
}

void MotionManager::setStandardPose(const MotionPose &pose)
{
    for (UTInstrument *intrument: UltrasonicManager::instruments()) {
        intrument->SetStandardPos(UTPose(pose));
    }
}

QUuid MotionManager::allocateUuid()
{
    QUuid uuid;
    do {
        uuid = QUuid::createUuid();
    } while (axis(uuid) || endPoint(uuid));
    return uuid;
}

void MotionManager::updateAxisList()
{
    if (m_globalEndpoint) {
        m_globalEndpoint->setAxisList(m_axisList);
    }
    m_trio->refreshAxisList(m_axisList);

    QStringList names = axisNames();
    names.prepend(tr("名称"));
    m_positionModel.setHeader(names);

    for (int index = 0; index < m_axisList.count(); ++index) {
        Axis *axis = m_axisList[index];
        axis->setIndex(index);
    }

    emit axisListChanged();
}
