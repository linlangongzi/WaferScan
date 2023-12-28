#include "Joint.h"
#include <QFile>
#include <QDebug>
#include <osg/Config>

#include <QtMath>


using namespace osg;


Joint::Joint()
{
}

void Joint::SetPointBase(const QVector3D &pb)
{
    pointBase = pb;
    UpdateMatrix();
}

void Joint::SetAxisZ(const QVector3D &axis)
{
    axisZ = axis;
    UpdateMatrix();
}

void Joint::SetPos(qreal pos)
{
    if (axisZ.isNull()) {                                                       //axisZ未指定
        return;
    }
    if (type == Linear) {
        pos = qBound<qreal>(moveMin, pos, moveMax);
        matrix.makeTranslate(pos * axisZ.x(), pos * axisZ.y(), pos * axisZ.z());
    } else if (type == Rotary) {
        pos = qBound<qreal>(moveMin, pos, moveMax);
        matrix.makeRotate(qDegreesToRadians(pos), Vec3(axisZ.x(), axisZ.y(), axisZ.z()));
    }
    setMatrix(baseMatrixBefore * matrix * baseMatrixAfter);                     //先移到原点，再进行平移或旋转变换，最后再移动到原位置
}

void Joint::UpdateMatrix()
{
    Matrix basePointMatrixBefore, moveBaseMatrixBefore;
    basePointMatrixBefore.makeTranslate(-pointBase.x(), -pointBase.y(), -pointBase.z());
    //处理零点偏移带来的矩阵
    if (type == Linear) {
        moveBaseMatrixBefore.makeTranslate(moveBase * axisZ.x(), moveBase * axisZ.y(), moveBase * axisZ.z());
    } else if (type == Rotary) {
        moveBaseMatrixBefore.makeRotate(moveBase, Vec3(axisZ.x(), axisZ.y(), axisZ.z()));
    }
    baseMatrixBefore = basePointMatrixBefore * moveBaseMatrixBefore;
    baseMatrixAfter.makeTranslate(pointBase.x(), pointBase.y(), pointBase.z());
}
