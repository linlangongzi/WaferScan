#pragma once
#include <QVector>
#include <QVector3D>
#include <qmath.h>
#include "BSpline.h"

/*!
 * \brief B样条曲线
 */
class BSplineCurve
{
public:
    typedef cv::Mat_<qreal> Matrix;

    BSplineCurve() {}
    BSplineCurve(int _order, QVector<qreal> knotVector, Matrix controlPoints);
    BSplineCurve(BSpline bs, Matrix controlPoints);
    qreal PointOnCurve(qreal u, int axis);
    QVector3D PointOnCurve(qreal u);
    QVector<qreal> CurveDerivs(qreal u, int diffOrder, int p, int axis);
    static BSplineCurve FromInterpolation(int order, const Matrix &interpolationPoints);
    static BSplineCurve FromInterpolation(int order, Matrix interpolationPoints, QVector3D D0, QVector3D Dn);
    static BSplineCurve FromInterpolation(int order, Matrix interpolationPoints, QVector3D D0, QVector3D Dn, QVector3D A0, QVector3D An);
    static Matrix CurveSubdivide(const Matrix &points, const int numOfSubdivide);

private:
    Matrix ctrlPoints;
//    QVector<QVector3D> ctrlPoints;
    BSpline bspline;
};
