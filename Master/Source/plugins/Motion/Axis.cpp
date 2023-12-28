#include "Axis.h"
#include "MotionManager.h"
#include "TrioDriver.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QMatrix4x4>
#include <QMessageBox>
#include <QVariant>
#include <allrising/Util.h>

Axis::Axis(QObject *parent) :
    QObject(parent)
{
}

QString Axis::name() const
{
    return m_config.name;
}

int Axis::index() const
{
    return m_index;
}

int Axis::axisId() const
{
    return m_config.axisId;
}

AxisConfig::AxisType Axis::axisType() const
{
    return m_config.axisType;
}

int Axis::moveType() const
{
    return m_status.moveType;
}

/*! 获取轴的最新位置 */
double Axis::position() const
{
    return m_status.position;
}

QString Axis::positionText() const
{
    return tr("%1 %2").arg(position()).arg(axisUnit(UNIT_DISTANCE));
}

bool Axis::isRotation() const
{
    auto type = axisType();
    return type == AxisConfig::Rotation || type == AxisConfig::RotationLimited;
}

bool Axis::isInfiniteRotating() const
{
    return axisType() == AxisConfig::Rotation;
}

/*! 查询轴是否已停止运动 */
bool Axis::isIdle()
{
    return m_status.moveType == AxisStatus::Idle;
}

/*! 获取轴单位的字符串 */
QString Axis::axisUnit(UnitType type) const
{
    switch (type) {
    case UNIT_NONE:     return tr("");
    case UNIT_DISTANCE: return isRotation() ? tr(" °") : tr(" mm");
    case UNIT_SPEED:    return isRotation() ? tr(" °/s") : tr(" mm/s");
    case UNIT_ACCEL:    return isRotation() ? tr(" °/s²") : tr(" mm/s²");
    default:            return tr("");
    }
}

QUuid Axis::uuid() const
{
    return m_uuid;
}

AxisConfig Axis::config() const
{
    return m_config;
}

AxisSetup Axis::setup() const
{
    return m_setup;
}

AxisStatus Axis::status() const
{
    return m_status;
}

QString Axis::moveTypeText() const
{
    return EnumToKey(m_status.moveType);
}

/*! 设置速度 */
void Axis::SetSpeed(double speed)
{
    SetAxisParameter("SPEED", qMin(speed, m_config.speedMax));
}

/*! 设置-前向行走-最大值 */
void Axis::SetFSLimit(double fsLimit)
{
    SetAxisParameter("FS_LIMIT", qMin(fsLimit, m_config.limitForward));
}

/*! 设置-反向行走-最小值 */
void Axis::SetRSLimit(double rsLimit)
{
    SetAxisParameter("RS_LIMIT", qMax(rsLimit, m_config.limitReverse));
}

void Axis::cancelMove()
{
    MotionManager::driver()->cancel(axisId());
}

void Axis::MoveForward()
{
    MotionManager::driver()->forward(axisId());
}

void Axis::MoveReverse()
{
    MotionManager::driver()->reverse(axisId());
}

void Axis::MoveAbs(double pos)
{
    MotionManager::driver()->moveAbs(axisId(), pos);
}

void Axis::MoveRel(double pos)
{
    MotionManager::driver()->moveRel(axisId(), pos);
}

void Axis::setConfig(const AxisConfig &config)
{
    if (m_config == config) {
        return;
    }

    m_config = config;

    auto *trio = MotionManager::driver();
    trio->setAxisConfig(axisId(), config);

    emit configChanged(config);
}

void Axis::setSetup(const AxisSetup &setup)
{
    if (m_setup == setup) {
        return;
    }

    auto *trio = MotionManager::driver();
    trio->setAxisSetup(axisId(), setup);
    m_setup = setup;
    emit setupChanged(setup);
}

void Axis::setIndex(int index)
{
    if (m_index == index) {
        return;
    }

    m_index = index;
    emit indexChanged(m_index);
}

void Axis::setUuid(const QUuid &uuid)
{
    m_uuid = uuid;
}

void Axis::setStatus(const AxisStatus &status)
{
    if (status == m_status) {
        return;
    }
    m_status = status;
    emit statusChanged(m_status);
}

void Axis::SetAxisParameter(const QString &key, qreal value)
{
    MotionManager::driver()->setAxisParameter(m_config.axisId, key, value);
}

void Axis::fromJson(const QJsonValue &value)
{
    const QJsonObject json = value.toObject();

    if (json.contains("AxisID")) {
        AxisConfig config;
        AxisSetup setup;

        config.axisId = json["AxisID"].toInt();
        config.name = json["Name"].toString();
        config.axisType = KeyToEnum(json["AxisType"].toString(), AxisConfig::Prismatic);
        config.hidden = json["Hidden"].toBool();
        config.isLocateMaster = json["IsEmptyMove"].toBool();

        config.units = json["Units"].toDouble();
        config.repeatDistance = json["RepDist"].toDouble();
        config.encoderId = json["AxisEncoder"].toInt(config.axisId);
        config.unitsInstrument = json["UnitsInstrument"].toDouble(config.units);
        config.backlash = json["Backlash"].toDouble();

        config.accelLimit = json["AccelLimit"].toDouble();
        config.decelLimit = json["DecelLimit"].toDouble();
        config.speedMax = json["SpeedLimit"].toDouble();

        config.sRamp = json["SRamp"].toDouble();

        config.datumInput = json["DatumIn"].toInt();
        config.datumSpeed = json["DatumSpeed"].toDouble();
        config.datumCreep = json["DatumCreep"].toDouble();
        config.datumSequence = json["DatumSequence"].toInt();
        config.datumReversed = json["DatumReversed"].toBool();
        config.datumPosition = json["DatumDef"].toDouble();

        config.limitForward = json["LimitForwardPhysical"].toDouble();
        config.limitReverse = json["LimitReversePhysical"].toDouble();
        config.limitForwardInput = json["LimitForwardIn"].toInt(-1);
        config.limitReverseInput = json["LimitReverseIn"].toInt(-1);

        config.basePoint = convertFromJson<QVector3D>(json["Offset"]);
        config.baseDirection = convertFromJson<QVector3D>(json["Vector"]);

        QJsonObject sensors = json["Sensor"].toObject();
        for (const auto &key: sensors.keys()) {
            if (sensors[key].toInt(-1) < 0) {
                continue;
            }
            config.sensors.insert(key, sensors[key].toInt());
        }

        for (const auto &v: json["Kinematics"].toArray()) {
            config.kinematics << AxisKinematics::fromJson(v);
        }

        setup.disabled = json["Disabled"].toBool(true);
        setup.speedJog = json["SpeedJog"].toDouble();
        setup.speedLocate = json["SpeedLocate"].toDouble();

        setup.jogStep = json["JogStep"].toDouble();
        setup.accel = json["Accel"].toDouble();
        setup.decel = json["Decel"].toDouble();
        setup.forwardKey = json["HotkeyForward"].toString();
        setup.reverseKey = json["HotkeyReverse"].toString();

        setConfig(config);
        setSetup(setup);

        return;
    }

    m_uuid = convertFromJson<QUuid>(json["uuid"]);
    setConfig(convertFromJson<AxisConfig>(json["config"]));
    setSetup(convertFromJson<AxisSetup>(json["setup"]));
}

QJsonObject Axis::toJson() const
{
    QJsonObject obj;
    obj["uuid"] = convertToJson(m_uuid);
    obj["config"] = convertToJson(m_config);
    obj["setup"] = convertToJson(m_setup);
    return obj;
}

double Axis::speedJog() const
{
    return m_setup.speedJog;
}

double Axis::speedLocate() const
{
    return m_setup.speedLocate;
}

void Axis::AddTransform(QMatrix4x4 &m, qreal position) const
{
    for (const auto &kinematic: m_config.kinematics) {
        kinematic.addTransform(m, position);
    }
}

void Axis::AddTransformChain(QList<QMatrix4x4> &chain, QMatrix4x4 &m, qreal position) const
{
    for (const auto &kinematic: m_config.kinematics) {
        kinematic.addTransform(m, position);
        chain << m;
    }
}

/*! 获取当前轴的传感器列表 */
QMap<QString, int> Axis::GetSensorMap()
{
    return m_config.sensors;
}

/*! 获取轴心向量 */
QVector3D Axis::baseDirection() const
{
    return m_config.baseDirection;
}

/*! 获取轴心偏移 */
QVector3D Axis::baseOffset() const
{
    return m_config.basePoint;
}

bool Axis::datumDone() const
{
    return m_status.datumDone;
}

bool Axis::withinLimit(qreal pos)
{
    return qFuzzyCompare(pos, qBound(m_config.limitReverse, pos, m_config.limitForward));
}

/*! 获取以AxisID排序好的Axis集合 */
QList<Axis *> Axis::GetAxesSortByAxisID()
{
    QMultiMap<int, Axis *> axesMap;
    for (Axis *axis: MotionManager::axisList()) {
        axesMap.insert(axis->axisId(), axis);
    }
    return axesMap.values();
}

/*! 获取最大的轴号 */
int Axis::GetMaxAxisID()
{
    int max = 0;
    for (Axis *axis: MotionManager::axisList()) {
        max = qMax(max, axis->axisId());
    }
    return max;
}
