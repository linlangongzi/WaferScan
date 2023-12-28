#pragma once

#include <QVector>
#include <opencv.h>

/*!
 * \brief B样条基函数类
 */
class BSpline
{
public:
    typedef cv::Mat_<qreal> Matrix;

    BSpline();
    BSpline(const QVector<qreal> &knots, int order);

    void SetOrder(int newOrder);
    qreal GetOrder() const {return order;}
    QVector<qreal> BasisFunc(qreal u, int span);
    Matrix DerivativeBasis(qreal u, int span, int diffOrder);
    int FindSpan(qreal u);
    QVector<qreal> GetKnots() {return knots;}

private:
    QVector<qreal> knots;                                                       /* 节点矢量 */
    int order;                                                                  /* B样条基函数的阶次 */
    friend class BSplineCurve;
    friend class BSplineSurface;
};
