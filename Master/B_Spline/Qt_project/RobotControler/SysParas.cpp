#include "SysParas.h"

SysParas::SysParas()
{
    maxJoint = QVector<qreal>() << 45.00 << 45.00 << 0.00 << 45.00 << 45.00 << 45.00;
    minJoint = QVector<qreal>() << -45 << -45.00 << -90.00 << -45.00 << -45.00 << -45.00;
    oriJoint = QVector<qreal>() << 0 << 0 << 0 << 0 << 0 << 0;


    maxTCP = QVector<qreal>(6, 300);
    minTCP = QVector<qreal>(6, -300);
    oriTCP = QVector<qreal>(6,0);
}

