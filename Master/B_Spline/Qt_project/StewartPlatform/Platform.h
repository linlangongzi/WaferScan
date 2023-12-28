#ifndef PLATFORM_H
#define PLATFORM_H
#include <QVector>
#include <QVector3D>
#include <QObject>

class Platform : public QObject
{
    Q_OBJECT
public:
    enum Type{
        LinkType,
        ClassicType
    };

    explicit Platform(Type type = LinkType);
    ~Platform();

signals:
    void jointsChanged(double j0, double j1, double j2, double j3, double j4, double j5);

public slots:
    void SetPos(double xp, double yp, double zp, double ap, double bp, double cp, Type type = LinkType);
    void SetPos(QVector<double> pos, Type type = LinkType);
    bool GetJoints(QVector<double> &joints);

private:
    bool CalculateAngle(qreal &angle, QVector3D p0, QVector3D p1, qreal L1, qreal L2, int jointNO);
    QVector3D Inverse(QVector3D point);

public:
    QVector<QVector3D> range;                                                           //(min, origine, max)
private:
    /* pose of stewart platform */
    double x;
    double y;
    double z;
    double a;
    double b;
    double c;

    Type stewartType;
    bool linkType = false;                                                       //false为外摆结构，true为内摆结构

    /* mechanical parameters of stewart platform */
    double topRadius;                                                           /* 动平台参考点六边形内切圆半径 */
    double topInterval;                                                         /* 动平台相邻参考点间隔 */
    double bottomRadius;                                                        /* 定平台参考点六边形内切圆半径 */
    double bottomInterval;                                                      /* 定平台相邻参考点间隔 */
    double lengthOfSteerWheel;                                                  /* 电机连杆长度 */
    double lengthOfCardan;                                                      /* 万向节中心到舵盘径线的距离 */
    double lengthOfBar;                                                         /* 连杆长度 */
    double motorBar;
    double theta0;
    double baseLength;
    /* 主要参考点 */
    QVector<QVector3D> topPlatform;                                             /* 上平台参考点 */
    QVector<QVector3D> bottomPlatform;                                          /* 下平台参考点，即电机轴心 */
    QVector<QVector3D> endOfSteelWheel;                                         /* 舵盘半径 */
    QVector<QVector3D> cardanCenter;                                            /* 万向节中心到舵盘径线的距离 */

};

#endif // PLATFORM_H
