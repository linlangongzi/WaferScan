#pragma once
#include <QVector>
#include <qmath.h>
#include <opencv.h>

QVector<qreal> Gauss(QVector<QVector<qreal> > A, QVector<qreal> b);
QVector<qreal> Gauss(cv::Mat_<qreal> A, QVector<qreal> b);
