#include "OriginIndicator.h"
#include <QColor>
#include <osg/LineWidth>
#include <osg/Material>
#include <osgText/Text>
#include "qtosgUtils.h"

using namespace osg;

OriginIndicator::OriginIndicator(const QString &name, qreal length, QVector3D ori)
{
    qreal font = length / 5;
    DrawText("X", ori + QVector3D(length, 0, 0), Qt::black, font);
    DrawText("Y", ori + QVector3D(0, length, 0), Qt::black, font);
    DrawText("Z", ori + QVector3D(0, 0, length), Qt::black, font);
    DrawText(name, ori, Qt::black, font);
    DrawAxis(toVec3(ori), length, Qt::black);

    ref_ptr<Material> mat = new Material;
    mat->setDiffuse(Material::FRONT_AND_BACK, toVec4(Qt::red));     //散射
    mat->setSpecular(Material::FRONT_AND_BACK, toVec4(Qt::red));       //反射
    mat->setAmbient(Material::FRONT_AND_BACK, toVec4(Qt::red));     //环境光
    mat->setShininess(Material::FRONT_AND_BACK, 100.f);
    getOrCreateStateSet()->setAttribute(mat.get());
}

void OriginIndicator::DrawText(const QString &name, const QVector3D &pos, const QColor &color, qreal fontSize)
{
    // 设置文字几何
    ref_ptr<osgText::Text> text = new osgText::Text;
    text->setColor(toVec4(color));
    text->setCharacterSize(fontSize);
    text->setPosition(toVec3(pos));
    text->setAxisAlignment(osgText::Text::SCREEN);
    text->setText(name.toStdString());
    //设置文字节点
    ref_ptr<Geode> geode = new Geode;
    geode->addDrawable(text.get());
    geode->setName("text");
    addChild(geode.get());
}

/*!
 * \brief AxisNode::DrawAxis 画坐标轴
 * \param origin            原点
 */
void OriginIndicator::DrawAxis(const Vec3 &origin, qreal length, const QColor &color)
{
    ref_ptr<Geode> nodeAxis = new Geode();

    ref_ptr<Geometry> geomAxis = new Geometry;

    ref_ptr<Vec3Array> coords = new Vec3Array(6);
    (*coords)[0] = origin;
    (*coords)[1] = origin + Vec3(length, 0.0, 0.0);
    (*coords)[2] = origin;
    (*coords)[3] = origin + Vec3(0.0, length, 0.0);
    (*coords)[4] = origin;
    (*coords)[5] = origin + Vec3(0.0, 0.0, length);

    geomAxis->setVertexArray(coords);

    ref_ptr<Vec4Array> colorArray = new Vec4Array;
    colorArray->push_back(toVec4(color));
    geomAxis->setColorArray(colorArray, Array::BIND_OVERALL);
    geomAxis->addPrimitiveSet(new DrawArrays(PrimitiveSet::LINES, 0, 6));

    ref_ptr<StateSet> stateset = new StateSet;
    ref_ptr<LineWidth> linewidth = new LineWidth();
    linewidth->setWidth(1.0f);
    stateset->setAttributeAndModes(linewidth,StateAttribute::ON);
    geomAxis->setStateSet(stateset);

    nodeAxis->addDrawable(geomAxis);
    nodeAxis->setName("axis");
    addChild(nodeAxis);
}
