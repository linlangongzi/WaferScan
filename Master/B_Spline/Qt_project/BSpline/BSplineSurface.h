#ifndef BSPLINESURFACE_H
#define BSPLINESURFACE_H
#include <QVector>
#include <QVector3D>
#include <qmath.h>
#include <BSpline.h>
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
    BSplineSurface SurfaceExtend(Qt::Edge edge, qreal extendLength, int orderU = 3, int orderV = 3);
    Matrix GetCtrlPoints() {return ctrlPoints;}
private:
    Matrix ctrlPoints;                                                          /* 三维mat，前两维为点阵列，第三维为点的维度，点的维度必须是3维 */
    BSpline bsplineU;
    BSpline bsplineV;
};

#endif // BSPLINESURFACE_H
