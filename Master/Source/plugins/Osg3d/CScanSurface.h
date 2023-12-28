#pragma once

#include "BSplineSurface.h"
#include <osg/Geode>
#include <osg/Geometry>
#include <QRgb>

/*!
 * \brief The CScanSurface class    本身做为了一个node，用于显示C扫图的节点，由三角片元+纹理贴图实现
 */
class CScanSurface : public QObject, public osg::Geode
{
public:
    CScanSurface(int u, int v, const BSplineSurface &sur, const QString &utFilePath, const QString &stlFilePath);
    void GenerateGridSurface();                        //生成网格曲面
    void GenerateCScanImage(const QString &filePath);                           //导入超声C扫数据，生成(u,v)的C扫图
    void LoadStlFile(const QString &path);                                      //导入stl文件，生成triangleGeometry

private:
    unsigned int nu;                                                            //网格密度(nu+1)*(nv+1)
    unsigned int nv;
    BSplineSurface surface;
    QVector<QVector<QVector3D> >gridSurface;                                    //nu*nv的网格，按等u,v参数进行细分

    osg::ref_ptr<osg::Geometry> triangleGeometry;
//    osg::ref_ptr<CScanSurface> surfaceNode;                                     //用于显示C扫图的节点，由三角片元+纹理贴图实现
    osg::ref_ptr<osg::Image> osgImg;                                             //osgImage
    QVector<QRgb> colorTable;
};
