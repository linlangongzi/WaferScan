#ifndef FORWARDSOLVER_H
#define FORWARDSOLVER_H
#include <QVector>
#include <QMatrix4x4>
const qreal pi = 3.141592654;

class ForwardSolver
{
public:
    ForwardSolver();
    QVector<qreal> getTCP(QVector<qreal> joints);

private:
    /*
     * 记录各关节之间的偏移量，为const qreal
     */
    const qreal L21x = 0;
    const qreal L21y = 29.5;
    const qreal L21z = 45.5;
    const qreal L32x = 0;
    const qreal L32y = 0;
    const qreal L32z = 120;
    const qreal L43x = 0;
    const qreal L43y = 0;
    const qreal L43z = 20;
    const qreal L54x = 0;
    const qreal L54y = 125;
    const qreal L54z = 0;
    const qreal L65x = 0;
    const qreal L65y = 30;
    const qreal L65z = 0;

    const qreal LTCPx = 0;
    const qreal LTCPy = 1;
    const qreal LTCPz = 0;

    qreal j1;
    qreal j2;
    qreal j3;
    qreal j4;
    qreal j5;
    qreal j6;
    QVector<qreal> LTCP;
    QMatrix4x4 T21;
    QMatrix4x4 T32;
    QMatrix4x4 T43;
    QMatrix4x4 T54;
    QMatrix4x4 T65;
    QMatrix4x4 R10;
    QMatrix4x4 R21;
    QMatrix4x4 R32;
    QMatrix4x4 R43;
    QMatrix4x4 R54;
    QMatrix4x4 R65;





};

#endif // FORWARDSOLVER_H
