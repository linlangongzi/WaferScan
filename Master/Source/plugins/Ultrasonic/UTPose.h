#pragma once

#include <QVector>
#include <QVector3D>
#include <QJsonArray>

/*!
 * \class UTPose
 * \brief UTPose是一个机械臂姿态，其行为类似于数值，基于一个QVector，可以被复制。
 *
 * 除此之外，还可以获取当前姿态的末端位置向量，以及方向向量。
 * 方向向量可以使用GetElevation()和GetAzimuth()来转为仰角和方向角的数值。
 */

typedef float UTPoseValue;

typedef QVector<UTPoseValue> UTPoseBase;

class UTPose : public UTPoseBase
{
public:
    UTPose() = default;
    UTPose(int size, UTPoseValue fill = 0) : UTPoseBase(size, fill) {}
    UTPose(const QVector3D &position);
    UTPose(const UTPoseBase &v);
    UTPose(const QList<qreal> &v);
    UTPose(const QJsonArray &array);
    UTPose(const UTPose &other);

    QVector3D GetPosition() const;                                              /* 获取位置 */
    QVector3D GetDirection() const;                                             /* 获取方向 */
    bool isEqualWith(const UTPose &other, double limit = 0.01) const;  /* 与另一个位置是否相同 */
    static QVector3D GetVectorByAngles(float azimuth, float elevation);
};

Q_DECLARE_METATYPE(UTPose)
