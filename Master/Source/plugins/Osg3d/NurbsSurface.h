#pragma once

#include <osg/Drawable>

class GLUnurbs;

class NurbsSurface : public osg::Drawable
{
public:
    NurbsSurface();
    NurbsSurface( const NurbsSurface &copy, osg::CopyOp copyop = osg::CopyOp::SHALLOW_COPY);
    META_Object(osg, NurbsSurface);

    void setControlPoints(osg::Vec3Array *va) {controlPoints = va;}
    void setNormalArray(osg::Vec3Array *na) {normals = na;}
    void setTexCoordArray(osg::Vec2Array *ta) {texcoords = ta;}
    void setKnots(osg::FloatArray *ks, osg::FloatArray *kt) {knotS = ks; knotT = kt;}
    void setCounts(int s, int t) {countS = s; countT = t;}
    void setOrders(int s, int t) {orderS = s; orderT = t;}

    osg::ref_ptr<osg::Vec3Array> getControlPoints() {return controlPoints;}
    osg::ref_ptr<osg::Vec3Array> getNormalArray() {return normals;}
    osg::ref_ptr<osg::Vec2Array> getTexcoordArray() {return texcoords;}
    osg::ref_ptr<osg::FloatArray> getKnotS() {return knotS;}
    osg::ref_ptr<osg::FloatArray> getKnotT() {return knotT;}

    virtual osg::BoundingBox computeBoundingBox() const;
    virtual void drawImplementation(osg::RenderInfo&) const;

protected:
    virtual ~NurbsSurface() {}
    osg::ref_ptr<osg::Vec3Array> controlPoints;
    osg::ref_ptr<osg::Vec3Array> normals;
    osg::ref_ptr<osg::Vec2Array> texcoords;
    osg::ref_ptr<osg::FloatArray> knotS;
    osg::ref_ptr<osg::FloatArray> knotT;
    int countS = 0;
    int countT = 0;
    int orderS = 0;
    int orderT = 0;
    mutable GLUnurbs *nurbsObj = NULL;
};
