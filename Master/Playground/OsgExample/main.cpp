#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Depth>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
using namespace osg;

int main( int argc, char** argv )
{
    Geometry* geom = new Geometry;

    Vec3Array* coords = new Vec3Array;
    coords->push_back(Vec3(1.0, 0.0, 0.0));//corner+heightVec;
    coords->push_back(Vec3(0.0, 0.0, 0.0));//corner;
    coords->push_back(Vec3(0.0, 1.0, 0.0));//corner+widthVec;
    coords->push_back(Vec3(1.0, 1.0, 0.0));//corner+widthVec+heightVec;
    geom->setVertexArray(coords);

    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(Vec3(0.0, 0.0, 1.0));
    geom->setNormalArray(normals, osg::Array::BIND_OVERALL);

    osg::ref_ptr<osg::Vec4Array> c =new osg::Vec4Array();
    geom->setColorArray(c, Array::BIND_PER_VERTEX);
    c->push_back(osg::Vec4(0.0f,0.0f,0.0f,1.f));
    c->push_back(osg::Vec4(0.0f,0.0f,0.0f,1.f));
    c->push_back(osg::Vec4(0.8f,0.8f,0.8f,1.f));
    c->push_back(osg::Vec4(0.8f,0.8f,0.8f,1.f));

//#if defined(OSG_GLES1_AVAILABLE) || !defined(OSG_GLES2_AVAILABLE)
//    DrawElementsUByte* elems = new DrawElementsUByte(PrimitiveSet::TRIANGLES);
//    elems->push_back(0);
//    elems->push_back(1);
//    elems->push_back(2);

//    elems->push_back(2);
//    elems->push_back(3);
//    elems->push_back(0);
//    geom->addPrimitiveSet(elems);
//#else
//    geom->addPrimitiveSet(new DrawArrays(PrimitiveSet::QUADS,0,4));
//#endif
    geom->addPrimitiveSet(new DrawArrays(PrimitiveSet::QUADS,0,4));

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable( geom );

    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setClearMask( 0 );
    camera->setCullingActive( false );
    camera->setAllowEventFocus( false );
    camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    camera->setRenderOrder( osg::Camera::POST_RENDER );
    camera->setProjectionMatrix( osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0) );
    camera->addChild( geode.get() );

    osg::StateSet* cameraStateSet = camera->getOrCreateStateSet();
    cameraStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    cameraStateSet->setAttributeAndModes( new osg::Depth(osg::Depth::LEQUAL, 1.0, 1.0) );

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( camera.get() );
    root->addChild( osgDB::readNodeFile("cessna.osg") );


    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );
    return viewer.run();
}
