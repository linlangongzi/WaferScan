#pragma once

#include "BSplineSurface.h"
#include "NurbsSurface.h"
#include "PointMt.h"
#include <osg/MatrixTransform>

class SurfaceMt : public osg::MatrixTransform
{
public:
    SurfaceMt(const BSplineSurface::Matrix &interpolations, osg::Node *node, int u = 3, int v = 3);
    SurfaceMt(BSplineSurface sur, osg::Node *node, int u = 3, int v = 3);
    void Constructor(BSplineSurface sur, osg::Node *node);
    BSplineSurface GetBSplineSurface();
    void UpdateSurface();

private:
    BSplineSurface::Matrix interpolation;                                       //插值点阵列
    QVector<QVector<osg::ref_ptr<PointMt> > > interpolationMt;                  //插值点节点，可拖动
    osg::ref_ptr<osg::Geode> surfaceNode;

    osg::ref_ptr<NurbsSurface> nurbsSurface;
    int orderU;
    int orderV;
};
