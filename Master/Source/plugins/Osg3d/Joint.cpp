#include "Joint.h"
#include <QFile>
#include <QDebug>
#include <osg/Config>

#include <QtMath>

#include <osg/Material>

using namespace osg;

Joint::Joint()
{
    getOrCreateStateSet()->setAttributeAndModes(new Material, StateAttribute::PROTECTED | StateAttribute::ON);
    forwardLimitReached = false;
    reverseLimitReached = false;
}

void Joint::setPointBase(const QVector3D &pb)
{
    pointBase = pb;
    UpdateMatrix();
}

void Joint::setAxisZ(const QVector3D &axis)
{
    axisZ = axis;
    UpdateMatrix();
}

void Joint::setPos(qreal pos)
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
        moveBaseMatrixBefore.makeRotate(qDegreesToRadians(moveBase), Vec3(axisZ.x(), axisZ.y(), axisZ.z()));
    }
    baseMatrixBefore = basePointMatrixBefore * moveBaseMatrixBefore;
    baseMatrixAfter.makeTranslate(pointBase.x(), pointBase.y(), pointBase.z());
}

void Joint::SetHighlight(bool enable)
{
    static bool enablePrevious = false;
    if (enablePrevious == enable) {
        return;
    }
    enablePrevious = enable;
    StateSet *state = getOrCreateStateSet();
    state->setMode(GL_BLEND, StateAttribute::ON);
    Material *material = dynamic_cast<Material *>(state->getAttribute(StateAttribute::MATERIAL));
    if (!material) {
        material = new Material;
    }
    if (enable) {
        material->setDiffuse(Material::FRONT_AND_BACK, Vec4(1.0, 0.0, 0.0, 1.0));
        material->setAmbient(Material::FRONT_AND_BACK, Vec4(1.0, 0.0, 0.0, 1.0));
        material->setTransparency(Material::FRONT_AND_BACK, 0.5);                   //透明
        state->setAttributeAndModes(material, StateAttribute::OVERRIDE | StateAttribute::ON);
    } else {
        state->setAttributeAndModes(new Material, StateAttribute::OVERRIDE | StateAttribute::ON);
    }
    state->setRenderingHint(StateSet::TRANSPARENT_BIN);
}

void Joint::ForwardLimitUpdated(bool enable)
{
    forwardLimitReached = enable;
    SetHighlight(forwardLimitReached || reverseLimitReached);
}

void Joint::ReverseLimitUpdated(bool enable)
{
    reverseLimitReached = enable;
    SetHighlight(forwardLimitReached || reverseLimitReached);
}
