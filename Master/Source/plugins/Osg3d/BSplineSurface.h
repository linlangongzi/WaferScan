#pragma once
#include <QVector>
#include <QVector3D>
#include <qmath.h>
#include "BSpline.h"


bool CompareQVector3D(const QVector3D &p1, const QVector3D &p2);
QVector<QVector3D> UniquePointCloud(QVector<QVector3D> pointCloud);


/*!
 * \brief B样条曲面，只能进行三维曲面的拟合，即点的维度是三维
 */
class BSplineSurface
{
public:
    typedef cv::Mat_<qreal> Matrix;
    BSplineSurface();
    BSplineSurface(int orderU, int orderV, QVector<qreal> knotVectorU, QVector<qreal> knotVectorV, Matrix controlPoints);
    BSplineSurface(const BSpline &u, const BSpline &v, Matrix controlPoints);
    QVector3D PointOnSurface(qreal u, qreal v);
    Matrix SurfaceDerivative(qreal u, qreal v, int diffOrder, int axis);
    QVector3D NormVector(qreal u, qreal v);

    static BSplineSurface FromInterpolation(int orderU, int orderV, const Matrix &points);
    static BSplineSurface FromInterpolationByCloud(QVector<QVector3D> pointCloud, QVector3D direction, qreal yaw, int numU, int numV, int orderU = 3, int orderV = 3);
    static BSplineSurface SurfaceExtend(BSplineSurface sur, Qt::Edge edge, qreal extendLength, int orderU = 3, int orderV = 3);
    static BSplineSurface SurfaceResample(BSplineSurface sur, int rowDensity, int colDensity, int orderU = 3, int orderV = 3);
    static bool IntersectRaySurface(BSplineSurface &sur, QVector3D linePoint, QVector3D lineDir, qreal &u, qreal &v);  //计算直线与曲面的交点的参数(u,v)
    static bool IntersectRaySurfaceByNewTon(BSplineSurface &sur, QVector3D linePoint, QVector3D lineDir, qreal &u, qreal &v);
    static bool InversePointOnSurface(BSplineSurface &surface, const QVector3D &point, qreal &u, qreal &v);                          //计算曲面上的点对应的(u,v)
    static bool InversePointOnSurfaceByNewTon(BSplineSurface &surface, const QVector3D &point, qreal &u, qreal &v);                    //采用拟牛顿法求曲面上的点对应的(u,v)

    Matrix GetCtrlPoints() {return ctrlPoints;}
    QVector<qreal> GetKnotsU() { return bsplineU.GetKnots();}
    QVector<qreal> GetKnotsV() { return bsplineV.GetKnots();}
    int GetOrderU() {return bsplineU.GetOrder();}
    int GetOrderV() {return bsplineV.GetOrder();}
    bool IsValid() {return isValid;}

private:
    Matrix ctrlPoints;                                                          /* 三维mat，前两维为点阵列，第三维为点的维度，点的维度必须是3维 */
    BSpline bsplineU;
    BSpline bsplineV;
    bool isValid = false;                                                       //false表示曲面无效
};
