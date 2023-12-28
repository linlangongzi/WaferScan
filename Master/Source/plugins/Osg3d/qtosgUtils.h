#pragma once

#include <QtGlobal>
#include <osg/Quat>
#include <osg/Vec3>
#include <osg/Vec4>

QColor toColor(const osg::Vec4 &vec);
QVector3D toVector3D(const osg::Vec3 &vec);
QVector4D toVector4D(const osg::Vec4 &vec);
QVector4D toVector4D(const osg::Quat &quat);

osg::Vec3 toVec3(const QVector3D &vector);

osg::Vec4 toVec4(const QColor &color);
osg::Vec4 toVec4(const QVector4D &vector);
