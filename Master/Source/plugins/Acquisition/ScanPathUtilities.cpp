#include "ScanPathUtilities.h"
#include  <QVector3D>
#include <QVector>

Qt::Axis ScanPathUtilities::maximumAxis(const QVector3D &delta)
{
    auto max = qAbs(delta[0]);
    int maxIndex = 0;

    for (int index = 1; index < 3; ++index) {
        const auto value = qAbs(delta[index]);
        if (value > max) {
            max = value;
            maxIndex = index;
        }
    }
    return Qt::Axis(maxIndex);
}

QVector3D ScanPathUtilities::unitVector(const Qt::Axis &axis)
{
    QVector3D norm;
    for(int i = 0; i < 3; ++i) {
        if (i == axis) {
            norm[i] = 1;
        }
    }
    return norm;
}
