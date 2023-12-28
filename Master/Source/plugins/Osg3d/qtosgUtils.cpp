#include "qtosgUtils.h"
#include <QColor>
#include <QVector3D>
#include <QVector4D>

QColor toColor(const osg::Vec4 &vec)
{
    return QColor::fromRgba(vec.asRGBA());
}

QVector3D toVector3D(const osg::Vec3 &vec)
{
    return QVector3D(vec.x(), vec.y(), vec.z());
}

QVector4D toVector4D(const osg::Vec4 &vec)
{
    return QVector4D(vec.x(), vec.y(), vec.z(), vec.w());
}

QVector4D toVector4D(const osg::Quat &quat)
{
    return QVector4D(quat.x(), quat.y(), quat.z(), quat.w());
}

osg::Vec3 toVec3(const QVector3D &vector)
{
    return osg::Vec3(vector.x(), vector.y(), vector.z());
}

osg::Vec4 toVec4(const QColor &color)
{
    return osg::Vec4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

osg::Vec4 toVec4(const QVector4D &vector)
{
    return osg::Vec4(vector.x(), vector.y(), vector.z(), vector.w());
}
