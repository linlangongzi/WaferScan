#pragma once

#include <osg/MatrixTransform>
#include <QString>
#include <QVector3D>

/*!
 * \brief The AxisNode class    直角坐标系节点
 *                                  包括原点、X轴、Y轴、Z轴、坐标系名称
 */
class OriginIndicator : public osg::MatrixTransform
{
public:
    OriginIndicator(const QString &name, qreal length = 300, QVector3D ori = QVector3D());

private:
    void DrawText(const QString &name, const QVector3D &pos, const QColor &color, qreal fontSize = 50);
    void DrawAxis(const osg::Vec3 &origin, qreal length, const QColor &color);
};
