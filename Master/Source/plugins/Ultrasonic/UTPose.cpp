#include "UTPose.h"
#include <QDebug>
#include <QtMath>

static const int typeId = qRegisterMetaType<UTPose>("UTPose");

/*!
 * \brief 以一个位置向量创建位姿，位置向量中数值的单位为mm
 * \param 位置向量
 */
UTPose::UTPose(const QVector3D &position)
{
    push_back( position.x() );
    push_back( position.y() );
    push_back( position.z() );
}

UTPose::UTPose(const UTPoseBase &v) :
    UTPoseBase(v)
{}

UTPose::UTPose(const QList<qreal> &v) :
    UTPoseBase(v.count())
{
    for (int index = 0; index < v.count(); ++index) {
        (*this)[index] = v[index];
    }
}

UTPose::UTPose(const QJsonArray &array)
{
    foreach (const QJsonValue value, array) {
        push_back(value.toDouble());
    }
}

/*!
 * \brief 复制构造体
 * \param other 被复制的UTPose
 */
UTPose::UTPose(const UTPose &other) = default;

/*!
 * \brief 获取一个位姿的位置向量
 * \return 位置向量
 */
QVector3D UTPose::GetPosition() const
{
    if (count() >= 3) {
        return QVector3D( value(0), value(1), value(2) );
    }
    return QVector3D();
}

/*!
 * \brief 获取一个位姿的方向向量
 * \return 方向向量
 */
QVector3D UTPose::GetDirection() const
{
    if (count() >= 5) {
        return GetVectorByAngles( value(3), value(4) );                         /* 第4轴为方向角，第5轴为仰角 */
    }
    return QVector3D();
}

/*!
 * \brief 检测是否与另一个UTPose相同，这个比较基于运动控制器的控制精度
 * 这个比较用于检测从一个位姿移到另一个位姿是否需要移动
 * 如果作为参数传入的UTPose长度大于当前项，则返回false
 * 当用于比较运动控制器的当前状态和期望状态时，应当用current.isEqualWith(expected)，因为
 * 运动控制器报告的轴数应当是最多的
 * \param other 目标位姿
 * \return bool
 */
bool UTPose::isEqualWith(const UTPose &other, double limit) const
{
    int Num = qMin(other.count(), count());
    for (int i = 0; i < Num; ++i) {
        if (qAbs( other.value(i) - value(i) ) > limit) {
            return false;
        }
    }
    return true;
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
    return QVector3D( projectLength * qCos(azimuth), projectLength * qSin(azimuth), qSin(elevation) );
}
