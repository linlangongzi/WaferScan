#include "UTPose.h"
#include <QDebug>

UTPoseFull UTPose::axisMultiplier;
UTPoseFull UTPose::axisOffset;

/*!
 * \brief 默认构造体，创建一个空vector，并且其值为原始值。
 */
UTPose::UTPose(bool full) :
    UTPoseBase(),
    isFull(full)
{
    speed = -1;
}

/*!
 * \brief 以一个位置向量创建位姿，位置向量中数值的单位为mm
 * \param 位置向量
 */
UTPose::UTPose(const QVector3D &position)
{
    push_back(UTPoseValue(position.x()));
    push_back(UTPoseValue(position.y()));
    push_back(UTPoseValue(position.z()));
    isFull = true;
    speed = -1;
}

/*!
 * \brief 创建一个末端在指定位置的位姿，并指定其指向。两个向量中的单位为mm
 * \param position 位置向量
 * \param direction 方向向量。向量的长度不会影响结果
 */
UTPose::UTPose(const QVector3D &position, const QVector3D &direction)
{
    push_back(UTPoseValue(position.x()));
    push_back(UTPoseValue(position.y()));
    push_back(UTPoseValue(position.z()));
    push_back(UTPoseValue(GetAzimuth(direction)));
    push_back(UTPoseValue(GetElevation(direction)));
    isFull = true;
    speed = -1;
}

UTPose::UTPose(const QJsonArray &array) :
    isFull(true)
{
    foreach (const QJsonValue value, array)
    {
        push_back(UTPoseValue(value.toDouble()));
    }
    speed = -1;
}

/*!
 * \brief 复制构造体
 * \param other 被复制的UTPose
 */
UTPose::UTPose(const UTPose &other) :
    UTPoseBase(other),
    isFull(other.isFull)
{
    speed = -1;
}

float UTPose::DistanceTo(const UTPose &other) const
{
//    qDebug() << GetPosition() << other.GetPosition() << GetPosition().distanceToPoint(other.GetPosition());
    return GetPosition().distanceToPoint(other.GetPosition());
}

/*!
 * \brief 获取一个位姿的位置向量
 * \return 位置向量
 */
QVector3D UTPose::GetPosition() const
{
    if (count() >= 3 && isFull)
    {
        return QVector3D(at(0).value, at(1).value, at(2).value);
    }
    return QVector3D();
}

/*!
 * \brief 获取一个位姿的方向向量
 * \return 方向向量
 */
QVector3D UTPose::GetDirection() const
{
    if (count() >= 5 && isFull)
    {
        return GetVectorByAngles(value(3).value, value(4).value);               /* 第4轴为方向角，第5轴为仰角 */
    }
    return QVector3D();
}

/*!
 * \brief 检查位姿，确保其中的数值是标准单位(mm)，而不是原始值。
 * 这个方法会进行单位换算，比较耗时。
 */
void UTPose::EnsureFull()
{
    if (!isFull)
    {
        if (axisMultiplier.count() > count())
        {
            qCritical() << "UTPose::EnsureFull: axisMultiplier not enough, expecting" << count() << ", got" << axisMultiplier.count();
        }
        int index = 0;
        for (iterator it = begin(); it != end(); ++it)
        {
            /* axisMultiplier不存在时，将使用默认值1，axisOffset不存在时，将使用默认值0。 */
            UTPoseValueRaw raw = it->raw;
            raw = ((raw & 0x40000000) << 1) | (raw & 0x7FFFFFFF);
            it->value = static_cast<UTPoseValueFull>(raw) * axisMultiplier.value(index, 1) + axisOffset.value(index, 0);
            index++;
        }
        isFull = true;
    }
}

bool UTPose::IsFull() const
{
    return isFull;
}

void UTPose::SetFull(bool full)
{
    isFull = full;
}

/*!
 * \brief 检测是否与另一个UTPose相同，这个比较基于运动控制器的控制精度
 * 这个比较用于检测从一个位姿移到另一个位姿是否需要移动
 * 如果作为参数传入的UTPose长度大于当前项，则返回false
 * 当用于比较运动控制器的当前状态和期望状态时，应当用current.IsEqualWith(expected)，因为
 * 运动控制器报告的轴数应当是最多的
 * \param other 目标位姿
 * \return bool
 */
bool UTPose::IsEqualWith(const UTPose &other, int NumAxis, double limit) const
{
//    if (other.count() > count())                                                /* 如果被比较项的坐标数量多于自身，则不可能相等 */
//    {
//        return false;
//    }
    int Num = qMin(NumAxis, other.count());
    Num = qMin(Num, count());
    for (int i = 0; i < Num; ++i)
    {
        if (qAbs(other.value(i).value - value(i).value) > limit)
        {
            return false;
        }
    }
    return true;
}

/*!
 * \brief UTPose::GetSpeed 返回速度
 * \return 速度
 */
float UTPose::GetSpeed()
{
    return speed;
}

/*!
 * \brief UTPose::SetSpeed 设置速度
 */
void UTPose::SetSpeed(float value)
{
    speed = value;
}

/*!
 * \brief 计算一个向量的仰角。仰角为方向向量与其在x-y平面之间的夹角。
 * \param 方向向量
 * \return 仰角，单位为弧度
 */
float UTPose::GetElevation(const QVector3D &direction)
{
    return qAsin(direction.z() / direction.length());
}

/*!
 * \brief 计算一个向量的方向角。方向角为方向向量在x-y平面上的投影，与x轴的夹角。
 * \param 方向向量
 * \return 方向角，单位为弧度
 */
float UTPose::GetAzimuth(const QVector3D &direction)
{
    return qAtan2(direction.y(), direction.x());
}

/*!
 * \brief 根据方向角和仰角，计算方向向量
 * \param elevation 仰角，单位为弧度
 * \param azimuth 方向角，单位为弧度
 * \return 方向向量，长度为1
 */
QVector3D UTPose::GetVectorByAngles(float azimuth, float elevation)
{
    qreal projectLength = qCos(elevation);                                      /* 向量在x-y平面上投影的长度 */
    return QVector3D(projectLength * qCos(azimuth), projectLength * qSin(azimuth), qSin(elevation));
}

void UTPose::SetAxisParameter(UTPoseFull vecAxisMultiplier, UTPoseFull vecAxisOffset)
{
    axisMultiplier = vecAxisMultiplier;
    axisOffset = vecAxisOffset;
}
