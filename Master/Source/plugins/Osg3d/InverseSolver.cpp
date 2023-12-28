#include "InverseSolver.h"
#include <QMatrix4x4>
#include <QtMath>

InverseSolver::InverseSolver()
{
}

/*!
 * \brief InverseSolver::Inverse    根据matlab程序InverseSolver.m改写，在笛卡尔坐标系下
 * \param pos               焦斑位置
 * \param dir               方向，曲面朝外
 * \param wp                水距
 * \return
 */
QVector<qreal> InverseSolver::Inverse(QVector3D pos, QVector3D dir, qreal wp)
{
    //设备尺寸参数
    qreal offsetBAX = 60;
    qreal offsetAToolZ = 60;
    qreal offsetAToolY = 0;
    qreal lengthOfTool = 46 + wp;

    dir.normalize();
    if (dir.z() <= 0) {
        qDebug() << "Normal vector is wrong, and the angle of Aor B will out of range";
        return QVector<qreal>(5);
    }
    qreal jointB = qRadiansToDegrees(qAtan(dir.x() / dir.z()));
    qreal jointA = qRadiansToDegrees(-qAsin(dir.y()));

    QMatrix4x4 Rb;
    Rb.rotate(jointB, QVector3D(0, 1, 0));
    QMatrix4x4 Tba;
    Tba.translate(-offsetBAX, 0, 0);
    QMatrix4x4 Ra;
    Rb.rotate(jointA, QVector3D(1, 0, 0));
    QVector3D TCPa(0, -offsetAToolY, -offsetAToolZ - lengthOfTool);
    QVector3D point = pos - Rb * Tba * Ra * TCPa;

    QVector<qreal> pose;
    pose << point.x() << point.y() << point.z() << jointB << jointA;
    return pose;
}
