#include "EndPoint.h"
#include <Motion/MotionManager.h>
#include <QJsonValue>
#include <QLoggingCategory>
#include <QMatrix4x4>
#include <QTimer>
#include <utils/algorithm.h>
#include <QtMath>

static Q_LOGGING_CATEGORY(debug, "motion.endpoint");

EndPoint::EndPoint(QObject *parent, EndPointType type) :
    QObject(parent),
    m_type(type)
{
    startTimer(200);
}

void EndPoint::setAxes(const MotionAxisList &axes)
{
    qCDebug(debug) << objectName() << "SetAxes" << axes;
    QList<Axis *> axisList;
    for (int index: axes.toList()) {
        axisList << MotionManager::axis(index);
    }
    setAxisList(axisList);
}

/*!
 * \brief EndPoint::axisIndex
 * \param uuid 轴的uuid
 * \return 轴在EndPoint中的序号
 */
int EndPoint::axisIndex(const QUuid &uuid) const
{
    return m_axisUuidList.indexOf(uuid);
}

/*!
 * \brief EndPoint::axisId
 * \param index: endPoint中的轴序号
 * \return
 */
QUuid EndPoint::axisUuid(int index) const
{
    return m_axisUuidList.value(index);
}

/*!
 * \brief EndPoint::axis
 * \param index: endPoint中的轴序号
 * \return
 */
Axis *EndPoint::axis(int index) const
{
    return m_axisList.value(index);
}

Axis *EndPoint::axis(const QUuid &id) const
{
    return m_axisList.value(m_axisUuidList.indexOf(id));
}

int EndPoint::axisCount() const
{
    return m_axisList.count();
}

/*!
 * \brief EndPoint::motionPose
 * \param vector
 * \return 按照各自轴顺序组成的 MotionPose
 */
MotionPose EndPoint::motionPose(const QVector3D &position)
{
    QVector<float> pose(MotionManager::axisCount());
    for (int index = 0; index < 3; ++index) {
        Axis *axis = m_axisList.value(index);
        if (axis != nullptr) {
            pose[axis->index()] = position[index];
        }
    }
    return MotionPose(pose);
}

MotionPose EndPoint::motionPose(const QVector3D &position, const QVector3D &direction)
{
    QVector<float> pose(MotionManager::axisCount());
    if (m_axisList.isEmpty() && m_axisList.size() < 5) {
        return {};
    }
    pose[m_axisList[0]->index()] = position[0];
    pose[m_axisList[1]->index()] = position[1];
    pose[m_axisList[2]->index()] = -position[2];
    QPair<qreal, qreal> angles = directionBA(direction);
    pose[m_axisList[3]->index()] = angles.first;
    pose[m_axisList[4]->index()] = angles.second;
    return pose;
}

MotionPose EndPoint::motionPose(const QVector3D &position, qreal angleFirst, qreal angleSecond)
{
    QVector<float> pose(MotionManager::axisCount());
    if (m_axisList.isEmpty() && m_axisList.size() < 5) {
        return {};
    }
    pose[m_axisList[0]->index()] = position[0];
    pose[m_axisList[1]->index()] = position[1];
    pose[m_axisList[2]->index()] = -position[2];
    pose[m_axisList[3]->index()] = angleFirst;
    pose[m_axisList[4]->index()] = angleSecond;
    return pose;
}

/*!
 * \brief EndPoint::position
 * \param pose
 * \return x、y、z轴的位置向量
 */
QVector3D EndPoint::position(const MotionPose &pose)
{
    if (pose.count() < 3) {
        return QVector3D();
    }
    return QVector3D(pose[Qt::XAxis], pose[Qt::YAxis], -pose[Qt::ZAxis]);
}

QVector3D EndPoint::direction(const MotionPose &pose, Qt::Axis axis)
{
    if (m_type == Cartesian) {
        QMatrix4x4 matrix;
        matrix.rotate(pose.value(3), QVector3D(0, 1, 0));
        matrix.rotate(pose.value(4), QVector3D(1, 0, 0));

        if (axis == Qt::XAxis) {
            return matrix.map(QVector3D(1, 0, 0));
        } else if (axis == Qt::YAxis) {
            return matrix.map(QVector3D(0, 1, 0));
        } else if (axis == Qt::ZAxis) {
            return matrix.map(QVector3D(0, 0, -1));
        }
    }
    return QVector3D();
}

QPair<qreal, qreal> EndPoint::directionBA(const QVector3D &direction)
{
    const qreal x = -direction.x();
    const qreal y = direction.y();
    const qreal z = direction.z();
    const qreal b = qRadiansToDegrees(qAtan2(x, z));
    const qreal a = qRadiansToDegrees(qAtan2(y, qSqrt(x * x + z * z)));
    return qMakePair(b, a);
}

QPair<qreal, qreal> EndPoint::directionCB(const QVector3D &direction)
{
    const qreal x = direction.x();
    const qreal y = direction.y();
    const qreal z = direction.z();
    const qreal c = qRadiansToDegrees(qAtan2(y, x));
    const qreal b = -qRadiansToDegrees(qAtan2(z, qSqrt(x * x + y * y)));
    return qMakePair(c, b);
}

QVector3D EndPoint::directionFromBA(qreal b, qreal a)
{
    qreal elevation = qDegreesToRadians(a);
    qreal azimuth = qDegreesToRadians(b);
    qreal x = qCos(elevation) * qSin(azimuth);
    qreal y = qSin(elevation);
    qreal z = qCos(elevation) * qCos(azimuth);
    return QVector3D(x, y, z);
}

QVector3D EndPoint::directionFromCB(qreal c, qreal b)
{
    qreal elevation = qDegreesToRadians(b);
    qreal azimuth = qDegreesToRadians(c);
    qreal x = qCos(elevation) * qCos(azimuth);
    qreal y = qCos(elevation) * qSin(azimuth);
    qreal z = qSin(elevation);
    return QVector3D(x, y, z);
}

bool EndPoint::isValid() const
{
    const auto axes = EndPoint::axesOfType(m_type);
    int axisCount = axes.count();
    if (axisCount != m_axisList.count()) {
        return false;
    }

    for (int index = 0; index < axisCount; ++index) {
        if (axes[index]->uuid() != m_axisList[index]->uuid()) {
            return false;
        }
    }
    return true;
}

QList<Axis *> EndPoint::axesOfType(EndPoint::EndPointType type)
{
    QList<Axis *> axes;
    switch (type) {
    case EndPoint::Cartesian: ; break;                                          // TODO
    default:                  ; break;
    }
    return axes;
}

void EndPoint::loadJson(const QJsonValue &value)
{
    const QJsonObject json = value.toObject();
    if (json.contains("Axes")) {
        QList<Axis *> axisList;
        for (auto id: json["Axes"].toArray()) {
            Axis *axis = MotionManager::axis(id.toString().toInt() - 1);
            if (axis) {
                axisList << axis;
            }
        }
        setAxisList(axisList);
    } else {
        m_name = convertFromJson<QString>(json["name"]);
        m_uuid = convertFromJson<QUuid>(json["uuid"]);
        m_type = KeyToEnum(json["type"].toString(), None);
        m_axisUuidList = convertFromJson<QList<QUuid> >(json["axisList"]);
        m_offset = convertFromJson<QVector3D>(json["offset"]);
        updateAxisList();
    }
}

QJsonValue EndPoint::toJson() const
{
    QJsonObject json;
    json["name"] = m_name;
    json["uuid"] = convertToJson(m_uuid);
    json["type"] = EnumToKey(m_type);
    json["axisList"] = convertToJson(m_axisUuidList);
    json["offset"] = convertToJson(m_offset);
    return json;
}

QString EndPoint::description() const
{
    QString name = m_name;
    if (name.isEmpty()) {
        name = m_uuid.toString();
    }
    return tr("%1 (%2 自由度)").arg(name).arg(m_axisList.size());
}

MotionPose EndPoint::currentPos(const MotionAxisList &axes) const
{
    MotionPose axesPos;
    if (axes.isEmpty()) {
        foreach (Axis *axis, m_axisList) {
            axesPos << axis->position();
        }
    } else {
        QList<int> list = axes.toList();
        axesPos = MotionPose(list.count());
        for (int index = 0; index < list.count(); ++index) {
            int axisIndex = list[index];
            if (m_axisIndexList.contains(axisIndex)) {
                Axis *axis = MotionManager::axis(axisIndex);
                if (axis) {
                    axesPos[index] = axis->position();
                }
            }
        }
    }
    return axesPos;
}

QHash<QUuid, qreal> EndPoint::currentPose()
{
    QHash<QUuid, qreal> pose;
    for (Axis *axis: m_axisList) {
        pose[axis->uuid()] = axis->position();
    }
    return pose;
}

QString EndPoint::currentPosText() const
{
    QStringList text;
    for (Axis *axis: m_axisList) {
        text << tr("%1: %2 %3").arg(axis->name()).arg(axis->position()).arg(axis->axisUnit());
    }
    return tr("%1 (%2)").arg(description(), text.join(", "));
}

bool EndPoint::isIdle()
{
    for (Axis *axis: m_axisList) {
        if (axis->isIdle() == false) {
            return false;
        }
    }
    return true;
}

void EndPoint::AddTransform(QMatrix4x4 &m, const MotionPose &pose) const            /* 需要保证参数pose与axisList长度一致 */
{
    for (const auto *axis: m_axisList) {
        axis->AddTransform(m, pose.value(axis->axisId()));
    }
}

void EndPoint::AddTransformChain(QList<QMatrix4x4> &chain, QMatrix4x4 &m, const MotionPose &pose) const
{
    for (const auto *axis: m_axisList) {
        axis->AddTransformChain(chain, m, pose.value(axis->axisId()));
    }
}

QMatrix4x4 EndPoint::GetTransform(const MotionPose &pose) const
{
    QMatrix4x4 matrix;
    AddTransform(matrix, pose);
    return matrix;
}

QHash<QUuid, qreal> EndPoint::filterPose(const QHash<QUuid, qreal> &pose) const
{
    QList<QUuid> illegals = Utils::filtered(pose.keys(), [&] (const QUuid &uuid) {
        return !m_axisUuidList.contains(uuid);
    });
    QHash<QUuid, qreal> filtered = pose;
    for (const auto &illegal: illegals) {
        filtered.remove(illegal);
    }
    return filtered;
}

EndPoint::EndPointType EndPoint::type() const
{
    return m_type;
}

QString EndPoint::name() const
{
    return m_name;
}

QUuid EndPoint::uuid() const
{
    return m_uuid;
}

QList<Axis *> EndPoint::axisList() const
{
    return m_axisList;
}

QVector3D EndPoint::offset() const
{
    return m_offset;
}

QVector3D EndPoint::offsetFromBA(qreal angleB, qreal angleA)
{
    QMatrix4x4 matrix;
    matrix.rotate(angleB, QVector3D(0, 1, 0));
    matrix.rotate(angleA, QVector3D(1, 0, 0));
    return matrix.map(m_offset * QVector3D(1, 1, -1));
}

void EndPoint::MoveAbs(const QHash<QUuid, qreal> &pose)
{
    Motion::TrioDriver *trio = MotionManager::driver();

    trio->moveAbs(filterPose(pose));
}

void EndPoint::Cancel()
{
    foreach (Axis *axis, m_axisList) {
        if (axis->moveType() != AxisStatus::MOVELINK) {
            axis->cancelMove();
        }
    }
}

/*!
 * \brief EndPoint::Locate 定位时的控制指令
 * \param position 定位位置
 */
void EndPoint::Locate(const MotionPose &position, const MotionAxisList &axes)
{
    QHash<QUuid, qreal> pose;
    int count = qMin(position.count(), m_axisList.count());
    for (int index = 0; index < count; ++index) {
        Axis *axis = m_axisList[index];
        if (axis && (axes.isEmpty() || axes.hasAxis(index))) {
            pose[axis->uuid()] = position[index];
        }
    }
    Locate(pose);
}

void EndPoint::Locate(const QHash<QUuid, qreal> &pos)
{
    QHash<QUuid, qreal> position = filterPose(pos);

    for (const QUuid &uuid: position.keys()) {
        Axis *axis = MotionManager::axis(uuid);
        if (axis) {
            axis->SetSpeed(axis->speedLocate());
        }
    }

    auto options = MotionManager::safetyBarrier()->options();
    Axis *axisX = MotionManager::findAxis("name", "X");
    axisX = m_axisList.contains(axisX) ? axisX : nullptr;
    Axis *axisY = MotionManager::findAxis("name", "Y");
    axisY = m_axisList.contains(axisY) ? axisY : nullptr;
    Axis *axisZ = MotionManager::findAxis("isLocateMaster", true);
    axisZ = m_axisList.contains(axisZ) ? axisZ : nullptr;

    bool axesAvailable = axisX && axisY && axisZ;
    if (axesAvailable && (position.contains(axisX->uuid()) || position.contains(axisY->uuid()))) {
        QRectF safeRect = options.rect;
        qreal destX = position.value(axisX->uuid(), axisX->position());
        qreal destY = position.value(axisY->uuid(), axisY->position());

        QHash<QUuid, qreal> sourcePose = currentPose();
        qreal sourceX = sourcePose.value(axisX->uuid(), axisX->position());
        qreal sourceY = sourcePose.value(axisY->uuid(), axisY->position());

        qreal emptyHeight = options.minDepth;
        qreal sourceHeight = sourcePose.value(axisZ->uuid(), axisZ->position());
        qreal destinationHeight = position.value(axisZ->uuid(), axisZ->position());
        qreal locateHeight = qMin(qMin(emptyHeight, sourceHeight), destinationHeight);
        qCDebug(debug) << "Locating at height" << locateHeight << "(min of" << emptyHeight << sourceHeight << destinationHeight << ")";

        bool insideSafe = safeRect.contains(destX, destY) && safeRect.contains(sourceX, sourceY);
        bool heightSafe = (sourceHeight <= locateHeight && destinationHeight <= locateHeight);

        if (insideSafe || heightSafe) {  // 起始点与终点均在安全区内, 或起点和终点高度均不低于安全高度
            MotionManager::driver()->moveAbs(position);
        } else {
            axisZ->MoveAbs(locateHeight);                 // 先抬Z轴
            // 平移
            QHash<QUuid, qreal> positionAbove = position;
            positionAbove[axisZ->uuid()] = locateHeight;
            MotionManager::driver()->moveAbs(positionAbove);
            // 下降
            axisZ->MoveAbs(destinationHeight);
        }
    } else {
        MotionManager::driver()->moveAbs(position);
    }
}

void EndPoint::setType(EndPoint::EndPointType type)
{
    if (m_type == type) {
        return;
    }
    m_type = type;
    emit typeChanged(m_type);
}

void EndPoint::setName(const QString &name)
{
    m_name = name;
}

void EndPoint::setAxisList(const QList<Axis *> &axisList)
{
    for (Axis *axis: m_axisList) {
        axis->disconnect(this);
    }

    m_axisList = axisList;
    m_axisUuidList = Utils::transform(m_axisList, &Axis::uuid);
    m_axisIdList = Utils::transform(m_axisList, [](Axis *axis) {return axis->config().axisId;});
    m_axisIndexList = Utils::transform(m_axisList, [](Axis *axis) {return axis->index();});

    for (Axis *axis: m_axisList) {
        connect(axis, &Axis::statusChanged, this, &EndPoint::onAxisStatusChanged, Qt::UniqueConnection);
    }
    emit axisListChanged(axisList);
}

void EndPoint::setOffset(const QVector3D offset)
{
    m_offset = offset;
}

void EndPoint::setUuid(const QUuid &uuid)
{
    m_uuid = uuid;
}

void EndPoint::onAxisStatusChanged()
{
    bool idle = true;
    MotionPose axesPos;
    foreach (Axis *axis, m_axisList) {
        axesPos << axis->position();
        idle &= axis->isIdle();
    }
    if (idle != prevIdle) {
        prevIdle = idle;
        if (idle) {
            prevIdleTime = QDateTime::currentDateTime();
        }
    }
    emit PositionUpdated(axesPos);
}

void EndPoint::updateAxisList()
{
    for (Axis *axis: m_axisList) {
        axis->disconnect(this);
    }
    m_axisList.clear();
    m_axisIdList.clear();
    m_axisIndexList.clear();
    for (const QUuid &id: m_axisUuidList) {
        Axis *axis = MotionManager::axis(id);
        if (axis) {
            m_axisList << axis;
            m_axisIdList << axis->axisId();
            m_axisIndexList << axis->index();
            connect(axis, &Axis::statusChanged, this, &EndPoint::onAxisStatusChanged, Qt::UniqueConnection);
        }
    }
}

void EndPoint::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if (prevIdle && prevIdleTime.isValid() && prevIdleTime.msecsTo(QDateTime::currentDateTime()) > 500) {
        prevIdleTime = QDateTime();
        qCDebug(debug) << objectName() << "came idle";
        emit EndPointIdle();
    }
}
