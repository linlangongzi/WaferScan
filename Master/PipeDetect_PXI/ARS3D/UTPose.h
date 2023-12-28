#ifndef MOTION_H
#define MOTION_H

#include <QVector>
#include <QVector3D>
#include <QJsonArray>
#include <QtMath>

/*!
 * \class UTPose
 * \brief UTPose是一个机械臂姿态，其行为类似于数值，基于一个QVector，可以被复制。
 *
 * 由于下位机提交的机械臂姿态是qint32，而位姿换算非常耗时，所以数值使用了union，可以作为原始的
 * qint32来读取，也可以作为float来读取，得到标准单位(mm)的位姿。
 * EnsureFull()会确保当前位姿为标准单位。从原始值转为标准单位的运算量比较大。
 *
 * 除此之外，还可以获取当前姿态的末端位置向量，以及方向向量。
 * 方向向量可以使用GetElevation()和GetAzimuth()来转为仰角和方向角的数值。
 */

typedef qint32 UTPoseValueRaw;
//static const qint32 UTPoseValueRawMask = 1 << (sizeof(UTPoseValueRaw) - 1);
typedef float UTPoseValueFull;
Q_STATIC_ASSERT(sizeof(UTPoseValueRaw) == sizeof(UTPoseValueFull));

/* 位姿数值，可以是原始值，也可以是标准单位 */
union UTPoseValue
{
    UTPoseValueRaw raw;
    UTPoseValueFull value;

    /* 联合的构造函数 */
    UTPoseValue () : raw(0) {}
    UTPoseValue (UTPoseValueRaw v) : raw(v) {}
    UTPoseValue (UTPoseValueFull v) : value(v) {}
    UTPoseValue (double v) : value(v) {}
};

typedef QVector<UTPoseValue> UTPoseBase;
typedef QVector<UTPoseValueFull> UTPoseFull;

class UTPose : public UTPoseBase
{
public:
    UTPose(bool full = false);
    UTPose(const QVector3D &position);
    UTPose(const QVector3D &position, const QVector3D &direction);
    UTPose(const QJsonArray &array);
    UTPose(const UTPose &other);

    float DistanceTo(const UTPose &other) const;
    QVector3D GetPosition() const;
    QVector3D GetDirection() const;
    void EnsureFull();
    bool IsFull() const;
    void SetFull(bool full);
    bool IsEqualWith(const UTPose &other, int NumAxis = 3, double limit = 0.001) const;

    float GetSpeed();
    void SetSpeed(float value);

    static float GetElevation(const QVector3D &direction);
    static float GetAzimuth(const QVector3D &direction);
    static QVector3D GetVectorByAngles(float azimuth, float elevation);

protected:
    friend class Axis;
    static void SetAxisParameter(UTPoseFull vecAxisMultiplier, UTPoseFull vecAxisOffset);
//    QJsonObject ToJson();
//    void FromJson(const QJsonObject &obj);

private:
//    void UpdateVector3D();
//    void UpdatePose();

private:
//    QVector<QVector3D> vec;
//    bool vecValid = false;
//    typedef float DataType;
    bool isFull;

    float speed;

    enum Vec3DIndex {
        POSITION = 0,
        DIRECTION = 1
    };

    static UTPoseFull axisMultiplier;     /* 轴参数，位移(mm)/光栅脉冲 */
    static UTPoseFull axisOffset;         /* 轴参数，零点偏移(mm) */
};

#endif // MOTION_H
