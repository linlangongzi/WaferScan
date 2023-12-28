#include "NurbsSurface.h"
#include <GL/glu.h>
#include <osg/GL>

using namespace osg;

NurbsSurface::NurbsSurface()
{}

NurbsSurface::NurbsSurface(const NurbsSurface &copy, osg::CopyOp copyop) :
    osg::Drawable(copy, copyop),
    controlPoints(copy.controlPoints),
    normals(copy.normals),
    texcoords(copy.texcoords),
    knotS(copy.knotS),
    knotT(copy.knotT),
    orderS(copy.orderS),
    orderT(copy.orderT),
    nurbsObj(copy.nurbsObj)
{}

osg::BoundingBox NurbsSurface::computeBoundingBox() const
{
    BoundingBox bb;
    if (!controlPoints.valid()) {
        return bb;
    }
    for (unsigned int i = 0; i < controlPoints->size(); ++i) {
        bb.expandBy((*controlPoints)[i]);
    }
    return bb;
}

void NurbsSurface::drawImplementation(osg::RenderInfo &) const
{
    if (!nurbsObj) {
        nurbsObj = gluNewNurbsRenderer();
        gluNurbsProperty(nurbsObj, GLU_SAMPLING_TOLERANCE, 10);
        gluNurbsProperty(nurbsObj, GLU_DISPLAY_MODE, GLU_FILL);
        nurbsObj = nurbsObj;
    }
    if (controlPoints.valid() && knotS.valid() && knotT.valid()) {
        glEnable(GL_MAP2_NORMAL);
        glEnable(GL_MAP2_TEXTURE_COORD_2);
        gluBeginCurve(nurbsObj);
        if (texcoords.valid()) {
            gluNurbsSurface(nurbsObj,
                            knotS->size(), &((*knotS)[0]),
                            knotT->size(), &((*knotT)[0]),
                            countS*2, 2,
                            &((*texcoords)[0][0]),
                            orderS, orderT,
                            GL_MAP2_TEXTURE_COORD_2);
        }
        if (normals.valid()) {
            gluNurbsSurface(nurbsObj,
                            knotS->size(), &((*knotS)[0]),
                            knotT->size(), &((*knotT)[0]),
                            countS*3, 3,
                            &((*normals)[0][0]),
                            orderS, orderT,
                            GL_MAP2_NORMAL);
        }
        gluNurbsSurface(nurbsObj,
                        knotS->size(), &((*knotS)[0]),
                        knotT->size(), &((*knotT)[0]),
                        countS*3, 3,
                        &((*controlPoints)[0][0]),
                        orderS, orderT,
                        GL_MAP2_VERTEX_3);

        gluEndCurve(nurbsObj);
        glDisable(GL_MAP2_NORMAL);
        glDisable(GL_MAP2_TEXTURE_COORD_2);
//        gluNurbsProperty(theNurbs, GLU_SAMPLING_TOLERANCE, 1.0f);             //设置采样容差
        glEnable(GL_AUTO_NORMAL);                                               //添加自动法向量，否则无法实现光照反射效果
//        glEnable(GL_NORMALIZE);
    }
}
