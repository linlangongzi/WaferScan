#ifndef BSPLINE_H
#define BSPLINE_H

#include <QVector>
#include <opencv2/opencv.hpp>


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

private:
    QVector<qreal> knots;                                                       /* 节点矢量 */
    int order;                                                                  /* B样条基函数的阶次 */
    friend class BSplineCurve;
    friend class BSplineSurface;
};

#endif // BSPLINE_H
