#include "SurfaceMt.h"
#include <osg/Material>

using namespace osg;

/*!
 * \brief The PointMtCallback class
 *                  针对PointMt回调类，用来识别PointMt位置是否改变
 *                  根据需要，亦可实现作为其它类的回调，做判断即可
 */
class PointMtCallback : public NodeCallback
{
public:
    virtual void operator()(Node *node, NodeVisitor *nv);
};

void PointMtCallback::operator()(Node *node, NodeVisitor *nv) {
    ref_ptr<PointMt> pointMt = dynamic_cast<PointMt *>(node);
    if (pointMt && pointMt->PositionChanged()) {
        if (pointMt->getNumParents() > 0) {
            ref_ptr<SurfaceMt> surfaceMt = dynamic_cast<SurfaceMt *>(pointMt->getParent(0));
            if (surfaceMt) {
                qDebug() << "surfaceMt will be updated";
                surfaceMt->UpdateSurface();
            }
        }
    }
    traverse(node, nv);
}

/*!
 * \brief SurfaceMt::SurfaceMt
 * \param interpolations        插值点
 * \param node                  点几何，便于显示与拖动
 */
SurfaceMt::SurfaceMt(const BSplineSurface::Matrix &interpolations, Node *node, int u, int v)
{
    orderU = u;
    orderV = v;
    interpolation = interpolations;
    BSplineSurface sur = BSplineSurface::FromInterpolation(orderU, orderV, interpolations);        //曲面拟合没有问题
    Constructor(sur, node);
}

SurfaceMt::SurfaceMt(BSplineSurface sur, Node *node, int u, int v)
{
    orderU = u;
    orderV = v;
    /* 先采样,构造插值点 */
    const int size[3] = {sur.GetCtrlPoints().size[0], sur.GetCtrlPoints().size[1], 3};
    BSplineSurface::Matrix interpolations(3, size);
    for (int row = 0; row < size[0]; ++row) {
        qreal u = 1.0 * row / (size[0] - 1);
        for (int col = 0; col < size[1]; ++col){
            qreal v = 1.0 * col / (size[1] - 1);
            const QVector3D point = sur.PointOnSurface(u, v);
            for (int dim = 0; dim < 3; ++dim) {
                interpolations(row, col, dim) = point[dim];
            }
        }
    }
    interpolation = interpolations;
    Constructor(sur, node);
}

void SurfaceMt::Constructor(BSplineSurface sur, Node *node)
{
    BSplineSurface::Matrix points = sur.GetCtrlPoints();
    interpolationMt = QVector<QVector<ref_ptr<PointMt> > >(interpolation.size[0],
                                   QVector<ref_ptr<PointMt> >(interpolation.size[1]));
    //设置控制点
    nurbsSurface = new NurbsSurface;
    ref_ptr<Vec3Array> controlPoints = new Vec3Array;
    controlPoints->reserve(points.size[0] * points.size[1]);
    for (int row = 0; row < points.size[0]; ++row) {
        for (int col = 0; col < points.size[1]; ++col) {
            QVector3D point;
            for (int dim = 0; dim < 3; ++dim) {
                point[dim] = interpolation(row, col, dim);
            }
            interpolationMt[row][col] = new PointMt(point, node);          //添加插值点
            addChild(interpolationMt[row][col]);
            interpolationMt[row][col]->setUpdateCallback(new PointMtCallback);
            controlPoints->push_back(Vec3(points(row, col, 0),                 //添加控制点给Nurbs
                                            points(row, col, 1),
                                            points(row, col, 2)));
        }
    }
    //设置节点矢量
    ref_ptr<FloatArray> knotsU = new FloatArray;
    knotsU->reserve(sur.GetKnotsU().size());
    foreach (auto k, sur.GetKnotsU()) {
        knotsU->push_back(k);
    }
    ref_ptr<FloatArray> knotsV = new FloatArray;
    knotsV->reserve(sur.GetKnotsV().size());
    foreach (auto k, sur.GetKnotsV()) {
        knotsV->push_back(k);
    }

    nurbsSurface->setControlPoints(controlPoints.get());
    nurbsSurface->setKnots(knotsU.get(), knotsV.get());
    nurbsSurface->setCounts(points.size[1], points.size[0]);
    nurbsSurface->setOrders(sur.GetOrderU() + 1, sur.GetOrderV() + 1);

    surfaceNode = new Geode;
    surfaceNode->addDrawable(nurbsSurface.get());
    surfaceNode->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::ON);
    surfaceNode->setName("surNode");

    //设置材质
    ref_ptr<Material> mat = new Material;
    mat->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.7f, 0.7f, 0.7f, 1.f));     //散射
    mat->setSpecular(Material::FRONT_AND_BACK, Vec4(1.f, 1.f, 1.f, 1.f));       //反射
    mat->setAmbient(Material::FRONT_AND_BACK, Vec4(0.2f, 0.2f, 0.2f, 1.f));     //环境光
    mat->setShininess(Material::FRONT_AND_BACK, 100.f);
    surfaceNode->getOrCreateStateSet()->setAttribute(mat.get());

    addChild(surfaceNode.get());
}

BSplineSurface SurfaceMt::GetBSplineSurface()
{
    //插值点位置更新
    for (int row = 0; row < interpolationMt.size(); ++row) {
        for (int col = 0; col < interpolationMt.first().size(); ++col) {
            QVector3D position = interpolationMt[row][col]->GetPosition();
            for (int dim = 0; dim < 3; ++dim) {
                interpolation(row, col, dim) = position[dim];
            }
        }
    }
    //曲面拟合
    return BSplineSurface::FromInterpolation(orderU, orderV, interpolation);        //曲面拟合没有问题
}

void SurfaceMt::UpdateSurface()
{
    //插值点位置更新
    for (int row = 0; row < interpolationMt.size(); ++row) {
        for (int col = 0; col < interpolationMt.first().size(); ++col) {
            QVector3D position = interpolationMt[row][col]->GetPosition();
            for (int dim = 0; dim < 3; ++dim) {
                interpolation(row, col, dim) = position[dim];
            }
        }
    }
    //曲面拟合
    BSplineSurface surface = BSplineSurface::FromInterpolation(orderU, orderV, interpolation);        //曲面拟合没有问题

    BSplineSurface::Matrix points = surface.GetCtrlPoints();

    ref_ptr<Vec3Array> controlPoints = nurbsSurface->getControlPoints();
    controlPoints->reserve(points.size[0] * points.size[1]);
    controlPoints->clear();
    //设置控制点
    for (int row = 0; row < points.size[0]; ++row) {
        qDebug() << "New line";
        for (int col = 0; col < points.size[1]; ++col) {
            controlPoints->push_back(Vec3(points(row, col, 0),                 //添加控制点给Nurbs
                                            points(row, col, 1),
                                            points(row, col, 2)));
        }
    }
    //设置节点矢量
    ref_ptr<FloatArray> knotsU = nurbsSurface->getKnotS();
    knotsU->reserve(surface.GetKnotsU().size());
    knotsU->clear();
    foreach (auto k, surface.GetKnotsU()) {
        knotsU->push_back(k);
    }
    ref_ptr<FloatArray> knotsV = nurbsSurface->getKnotT();
    knotsV->reserve(surface.GetKnotsV().size());
    knotsV->clear();
    foreach (auto k, surface.GetKnotsV()) {
        knotsV->push_back(k);
    }
    //几何体更新
    nurbsSurface->dirtyDisplayList();
}
