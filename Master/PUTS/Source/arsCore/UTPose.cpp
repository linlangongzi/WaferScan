#include "UTPose.h"
#include <QDebug>
#include <QtMath>

UTPoseFull UTPose::axisMultiplier;
UTPoseFull UTPose::axisOffset;
QMap<int, int> UTPose::EncorderMap;

/*!
 * \brief 默认构造体，创建一个空vector，并且其值为原始值。
 */
UTPose::UTPose() :
    UTPoseBase()
{}

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

/*!
 * \brief 创建一个末端在指定位置的位姿，并指定其指向。两个向量中的单位为mm
 * \param position 位置向量
 * \param direction 方向向量。向量的长度不会影响结果
 */
UTPose::UTPose(const QVector3D &position, const QVector3D &direction)
{
    push_back( position.x() );
    push_back( position.y() );
    push_back( position.z() );
    push_back( GetAzimuth(direction) );
    push_back( GetElevation(direction) );
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

#ifdef ARS_JSON_SUPPORT
UTPose::UTPose(const QJsonArray &array)
{
    foreach (const QJsonValue value, array) {
        push_back( value.toDouble() );
    }
}
#endif

/*!
 * \brief 复制构造体
 * \param other 被复制的UTPose
 */
UTPose::UTPose(const UTPose &other) :
    UTPoseBase(other)
{}

float UTPose::DistanceTo(const UTPose &other) const
{
    return GetPosition().distanceToPoint( other.GetPosition() );
}

void UTPose::Translate(const UTPose &offset)
{
    for (int index = 0; index < count(); ++index) {
        (*this)[index] += offset.value(index);
    }
}

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
 * \brief 使用指定的原始数据填充当前 UTPose 。
 * \param data 原始数据
 * \param offset 坐标矫正的偏移量
 * \return 坐标方向
 */
quint32 UTPose::FillPose(const qint32 data[], const UTPose &offset)
{
    resize( axisMultiplier.count() );
    squeeze();
    quint32 direction = 0;
    for (int index = 0; index < axisMultiplier.count(); ++index) {
        if (!EncorderMap.contains(index)) {
            continue;
        }
        int encord = EncorderMap[index];
        qint32 raw = data[encord];                                              /* 取用映射地址的数据 */
        bool isReversed = bool(raw & 0x80000000);                               /* 根据ISO/IEC 14882:1998标准文档，bool的构造体将任意非0值转为true(1) */
        direction |= quint32(isReversed) << index;                              /* 所以此处位操作是安全的 */
        Q_ASSERT( (quint32(isReversed) & 0x1) == quint32(isReversed) );         /* 关于此特性的断言 */
        raw = ( (raw & 0x40000000) << 1 ) | (raw & 0x7FFFFFFF);
        UTPoseValue value = UTPoseValue(raw) * axisMultiplier.value(index, 1) + axisOffset.value(index, 0);
        value += offset.value(index);
        replace(index, value);
    }
    return direction;
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
    int Num = qMin( NumAxis, other.count() );
    Num = qMin( Num, count() );
    for (int i = 0; i < Num; ++i) {
        if (qAbs( other.value(i) - value(i) ) > limit) {
            return false;
        }
    }
    return true;
}

QPointF UTPose::GetPoint(int axisX, int axisY) const
{
    return QPointF( value(axisX), value(axisY) );
}

/*!
 * \brief 计算一个向量的仰角。仰角为方向向量与其在x-y平面之间的夹角。
 * \param 方向向量
 * \return 仰角，单位为弧度
 */
float UTPose::GetElevation(const QVector3D &direction)
{
    return qAsin( direction.z() / direction.length() );
}

/*!
 * \brief 计算一个向量的方向角。方向角为方向向量在x-y平面上的投影，与x轴的夹角。
 * \param 方向向量
 * \return 方向角，单位为弧度
 */
float UTPose::GetAzimuth(const QVector3D &direction)
{
    return qAtan2( direction.y(), direction.x() );
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

void UTPose::SetAxisParameter(const QMap<int, int> &encorderMap, UTPoseFull vecAxisMultiplier, UTPoseFull vecAxisOffset)
{
    axisMultiplier = vecAxisMultiplier;
    axisOffset = vecAxisOffset;
    EncorderMap = encorderMap;
}
