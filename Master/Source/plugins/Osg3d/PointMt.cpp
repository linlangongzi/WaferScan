#include "PointMt.h"
#include <osg/Geometry>
#include <osg/Material>
#include <osg/Point>
#include <osg/ShapeDrawable>
#include <QMenu>
#include <QVector3D>

using namespace osg;

PointMt::PointMt(const QVector3D &p, Node *node, qreal pointSize)
{
    setMatrix(Matrix::translate(p.x(), p.y(), p.z()));
    if (node) {
        addChild(node);                                                         //此node用来实现鼠标hover和select
    }
    setName("pointMt");
    draggerTranslate = new QAction(tr("平移"), this);
    connect(draggerTranslate, &QAction::triggered, this, &PointMt::DraggerTranslate, Qt::QueuedConnection);

    /* 画一个点 */
    ref_ptr<Geometry> geomPoint = new Geometry;                                 //此几何点用来显示点
    ref_ptr<Vec3Array> pathVertexArray = new Vec3Array;
    geomPoint->setVertexArray(pathVertexArray.get());
    pathVertexArray->push_back(Vec3(0, 0, 0));
    //定义路径颜色
    ref_ptr<Vec4Array> pathColor = new Vec4Array;
    geomPoint->setColorArray(pathColor.get());
    geomPoint->setColorBinding(Geometry::BIND_OVERALL);
    pathColor->push_back(Vec4(1.f, 1.f, 1.f, 1.f));
    //设置点的关联方式为Line
    geomPoint->addPrimitiveSet(new DrawArrays(PrimitiveSet::POINTS, 0, pathVertexArray->size()));
    //添加法线
    ref_ptr<Vec3Array> norm = new Vec3Array;
    geomPoint->setNormalArray(norm.get());
    geomPoint->setNormalBinding(Geometry::BIND_OVERALL);
    norm->push_back(Vec3(0.f, 0.f, 1.f));

    //生成节点node
    ref_ptr<Geode> geodePoint = new Geode;
    geodePoint->addDrawable(geomPoint.get());
    //设置点的大小
    osg::Point *point = new osg::Point;
    point->setSize(pointSize);
    geodePoint->getOrCreateStateSet()->setAttribute(point);
    geodePoint->setInitialBound(osg::BoundingSphere(pathVertexArray->front(), pointSize));    //设置一个点的包围球尺寸，不设置的话，一个点无法显示，因为其没有大小

    addChild(geodePoint.get());
    postionPrevious = GetPosition();
}

void PointMt::ContextMenu(QMenu *menu, const QVector3D &p)
{
    menu->addAction(draggerTranslate);
    Q_UNUSED(p);
}

QVector3D PointMt::GetPosition()
{
    const BoundingSphere bound = getBound();
    return QVector3D(bound.center().x(), bound.center().y(), bound.center().z());
}

void PointMt::setPosition(const QVector3D &pos)
{
    setMatrix(Matrix::translate(pos.x(), pos.y(), pos.z()));
}

bool PointMt::PositionChanged()
{
    const QVector3D position = GetPosition();
    if (position == postionPrevious) {
        return false;
    } else {
        postionPrevious = position;
        return true;
    }
}

void PointMt::DraggerTranslate()
{
    AddDragger(Item3d::TranslateDragger);
}
