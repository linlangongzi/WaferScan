#pragma once

#include <QVector3D>

class ScanPathUtilities
{
public:
    static Qt::Axis maximumAxis(const QVector3D &delta);
    static QVector3D unitVector(const Qt::Axis &axis);
};

