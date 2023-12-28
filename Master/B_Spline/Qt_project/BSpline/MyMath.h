#ifndef MYMATH_H
#define MYMATH_H
#include <QVector>
#include <qmath.h>
#include <opencv2/opencv.hpp>

QVector<qreal> Gauss(QVector<QVector<qreal> > A, QVector<qreal> b);
QVector<qreal> Gauss(cv::Mat_<qreal> A, QVector<qreal> b);


#endif // MYMATH_H
