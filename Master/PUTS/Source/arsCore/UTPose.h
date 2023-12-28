#ifndef UTPOSE_H
#define UTPOSE_H

#include <QVector>
#include <QVector3D>

#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
#   define ARS_JSON_SUPPORT
#endif
#ifdef ARS_JSON_SUPPORT
#    include <QJsonArray>
#endif

/*!
 * \class UTPose
 * \brief UTPose是一个机械臂姿态，其行为类似于数值，基于一个QVector，可以被复制。
 *
 * 除此之外，还可以获取当前姿态的末端位置向量，以及方向向量。
 * 方向向量可以使用GetElevation()和GetAzimuth()来转为仰角和方向角的数值。
 */

typedef float UTPoseValue;

typedef QVector<UTPoseValue> UTPoseBase;
typedef QVector<UTPoseValue> UTPoseFull;

class UTPose : public UTPoseBase
{
public:
    UTPose();
    UTPose(int size, UTPoseValue fill = 0) : UTPoseBase(size, fill) {}
    UTPose(const QVector3D &position);
    UTPose(const QVector3D &position, const QVector3D &direction);
    UTPose(const UTPoseBase &v);
    UTPose(const QList<qreal> &v);
#ifdef ARS_JSON_SUPPORT
    UTPose(const QJsonArray &array);
#endif
    UTPose(const UTPose &other);

    float DistanceTo(const UTPose &other) const;
    void Translate(const UTPose &offset);
    QVector3D GetPosition() const;
    QVector3D GetDirection() const;
    bool IsEqualWith(const UTPose &other, int NumAxis = 3, double limit = 0.01) const;
    QPointF GetPoint(int axisX, int axisY) const;

    quint32 FillPose(const qint32 data[], const UTPose &offset);

    static float GetElevation(const QVector3D &direction);
    static float GetAzimuth(const QVector3D &direction);
    static QVector3D GetVectorByAngles(float azimuth, float elevation);

protected:
    friend class Axis;
    static void SetAxisParameter(const QMap<int, int> &encorderMap, UTPoseFull vecAxisMultiplier, UTPoseFull vecAxisOffset);
//    QJsonObject ToJson();
//    void FromJson(const QJsonObject &obj);

private:
    static UTPoseFull axisMultiplier;                                           /* 轴参数，位移(mm)/光栅脉冲 */
    static UTPoseFull axisOffset;                                               /* 轴参数，零点偏移(mm) */
    static QMap<int, int> EncorderMap;
};

Q_DECLARE_METATYPE(UTPose)

#endif                                                                          // UTPOSE_H
