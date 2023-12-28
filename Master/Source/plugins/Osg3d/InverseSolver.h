#pragma once

#include <QVector3D>

class InverseSolver
{
public:
    InverseSolver();
    static QVector<qreal> Inverse(QVector3D pos, QVector3D dir, qreal wp);
};
