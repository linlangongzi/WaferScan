#include "ForwardSolver.h"
#include <qmath.h>

ForwardSolver::ForwardSolver()
{
    T21 = QMatrix4x4(1, 0, 0, L21x,
                     0, 1, 0, L21y,
                     0, 0, 1, L21z,
                     0, 0, 0, 1);
    T32 = QMatrix4x4(1, 0, 0, L32x,
                     0, 1, 0, L32y,
                     0, 0, 1, L32z,
                     0, 0, 0, 1);
    T43 = QMatrix4x4(1, 0, 0, L43x,
                     0, 1, 0, L43y,
                     0, 0, 1, L43z,
                     0, 0, 0, 1);
    T54 = QMatrix4x4(1, 0, 0, L54x,
                     0, 1, 0, L54y,
                     0, 0, 1, L54z,
                     0, 0, 0, 1);
    T65 = QMatrix4x4(1, 0, 0, L65x,
                     0, 1, 0, L65y,
                     0, 0, 1, L65z,
                     0, 0, 0, 1);
    LTCP = QVector<qreal>(4) << LTCPx << LTCPy << LTCPz << 1;
}

QVector<qreal> ForwardSolver::getTCP(QVector<qreal> joints)
{
    //声明返回值
    QVector<qreal> TCP = QVector<qreal>(6, 0.0);

    j1 = joints[0] * pi / 180;
    R10 = QMatrix4x4(qCos(j1), -qSin(j1), 0, 0,
                     qSin(j1), qCos(j1),  0, 0,
                     0,        0,         1, 0,
                     0,        0,         0, 1);
    j2 = joints[1] * pi / 180;
    R21 = QMatrix4x4(1, 0,        0,         0,
                     0, qCos(j2), -qSin(j2), 0,
                     0, qSin(j2), qCos(j2),  0,
                     0, 0,        0,         1);
    j3 = joints[2] * pi / 180;
    R32 = QMatrix4x4(1, 0,        0,         0,
                     0, qCos(j3), -qSin(j3), 0,
                     0, qSin(j3), qCos(j3),  0,
                     0, 0,        0,         1);
    j4 = joints[3] * pi / 180;
    R43 = QMatrix4x4(qCos(j4),  0, qSin(j4), 0,
                     0,         1, 0,        0,
                     -qSin(j4), 0, qCos(j4), 0,
                     0,         0, 0,        1);
    j5 = joints[4] * pi / 180;
    R54 = QMatrix4x4(1, 0,        0,         0,
                     0, qCos(j5), -qSin(j5), 0,
                     0, qSin(j5), qCos(j5),  0,
                     0, 0,        0,         1);
    j6 = joints[5] * pi / 180;
    R65 = QMatrix4x4(qCos(j6),  0, qSin(j6), 0,
                     0,         1, 0,        0,
                     -qSin(j6), 0, qCos(j6), 0,
                     0,         0, 0,        1);
    QMatrix4x4 Trans1 = R10 * T21 * R21 * T32 *R32
                       * T43 * R43 * T54 * R54 * T65 * R65;
    QMatrix4x4 Trans2 = R10 * R21 * R32 * R43 * R54 * R65;
    QVector3D temp;
    //计算TCP点在世界坐标系下的xyz坐标
//    for (int i = 0; i <=2; i++) {
//        qreal sum1 = 0;
//        qreal sum2 = 0;
//        for (int j = 0; j<=3; j++) {
//            sum1 += Trans1(i, j) * LTCP[j];
//            sum2 += Trans2(i, j) * LTCP[j];
//        }
//        TCP[i] = sum1;
//        temp[i] = sum2;
//    }
    TCP[0] = Trans1(0,3);
    TCP[1] = Trans1(1,3);
    TCP[2] = Trans1(2,3);
    temp[0] = Trans2(0,1);
    temp[1] = Trans2(1,1);
    temp[2] = Trans2(2,1);
    TCP[5] = joints[5];         //即角度c的值
    //计算角度b的值
    TCP[4] = qAcos(temp[2]) / 3.14159 * 180;
    //计算角度a的值
    qreal r = qSqrt(qPow(temp[0], 2) + qPow(temp[1], 2));       //指向量在xyo平面的投影长度
    if (temp[0] == 0){
        TCP[3] = 0;
    }else if (temp[1] >= 0){
        TCP[3] = qAcos(temp[0] / r) / 3.14159 * 180;
    }else if (temp[1] < 0){
        TCP[3] = - qAcos(temp[0] / r) / 3.14159 * 180;
    }

    return TCP;

}

