#include "CScanSurface.h"
#include "UTDataProcessor/UTDocument.h"
#include <arsCore/Platform.h>
#include <qfile.h>
#include <qprogressdialog.h>
#include <qtextstream.h>
#include <Ultrasonic/UTStreamFile.h>
#include <arsCore/UTDataHeader.h>
#include <osg/Texture3D>

using namespace osg;

CScanSurface::CScanSurface(int u, int v, const BSplineSurface &sur, const QString &utFilePath, const QString &stlFilePath)
{
    nu = u;
    nv = v;
    surface = sur;
    osgImg = new Image;

    LoadStlFile(stlFilePath);
    GenerateCScanImage(utFilePath);
}

void CScanSurface::GenerateGridSurface()
{
    qDebug() << "Start to construct grid surface";
    for (uint row = 0; row <= nu; ++row) {
        qreal u = qreal(row) / nu;
        QVector<QVector3D> line;
        for (uint col = 0; col <= nv; ++ col) {
            qreal v = qreal(col) / nv;
            QVector3D point = surface.PointOnSurface(u, v);
            line.append(point);
        }
        gridSurface.append(line);
    }
    qDebug() << "Finish to construct grid surface";
}

void CScanSurface::GenerateCScanImage(const QString &filePath)
{
    UTStreamFile file(document()->GetCurrentFileName());
    Q_UNUSED(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error opening file" << file.fileName() << ":" << file.errorString();
        return;
    }

    UTImage image(QSize(nu + 1, nv + 1));
//    QImage image(nu + 1, nv + 1, QImage::Format_Indexed8);
//    image.setColorTable(colorTable);

    QProgressDialog progress(NULL);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setMinimumDuration(0);
    qreal sizeBase = 100 / qreal(file.size());
    while (!progress.wasCanceled() && !file.atEnd()) {
        UTDataSegment segment = file.ReadSegment(1024);
        progress.setValue(qreal(file.pos()) * sizeBase);
        for (const auto &frame : segment) {
            if (!frame.IsIGateTriggered()) {
                continue;
            }
            QVector3D linePoint = frame.GetWaterPathPosition();
            QVector3D lineDir = frame.GetWaterPathVector();
            qreal u = 0;
            qreal v = 0;
            Vec2 coordinate;     //超声声束与曲面网格的交点对应的(u,v)
            if (BSplineSurface::IntersectRaySurface(surface, linePoint, lineDir, u, v)) {
                coordinate = Vec2(u, v);     //超声声束与曲面网格的交点对应的(u,v)
            }
            //此处使用UTLayer进行数据归并
            float feature = frame.GetFeature(0, GateMaxPeak, DataAmp);         //C扫特征
            image(coordinate.x() * (nu + 1), coordinate.y() * (nv + 1)) = feature;
//            image.setPixel(imageCoordinate.x() * (nu + 1), imageCoordinate.y() * (nv + 1), feature)
        }
    }
    UTImagePalette imagePalette;
    QImage qImg = image.ToImage(imagePalette);

    //将QImage转换成osgImage
    unsigned char* bits = new unsigned char[qImg.byteCount()];
    memcpy(bits, qImg.bits(),qImg.byteCount());
    osgImg->setImage(qImg.width(), qImg.height(), 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, bits, osg::Image::USE_NEW_DELETE);

    ref_ptr<Texture3D> texture3D = new Texture3D;
    texture3D->setFilter(osg::Texture3D::MIN_FILTER,osg::Texture3D::LINEAR);
    texture3D->setFilter(osg::Texture3D::MAG_FILTER,osg::Texture3D::LINEAR);
    texture3D->setWrap(osg::Texture3D::WRAP_R,osg::Texture3D::REPEAT);
    texture3D->setImage(osgImg.get());

    osg::StateSet* stateSet = new osg::StateSet;
    stateSet->setTextureMode(0,GL_TEXTURE_GEN_R,osg::StateAttribute::ON);
    stateSet->setTextureAttributeAndModes(0,texture3D,osg::StateAttribute::ON);
    setStateSet(stateSet);

//    image.save("image.png");
    /* 1、每一帧数据，求与gridSurface的交点对应的(u0,v0)
     * 2、将该帧数据的C扫特征以(u,v)为坐标进行绘图
     * 3、得到所有帧数据，绘制出一个(u,v)为坐标的矩形图片
     * 4、研究如何将图片转换成osg可以识别的
     * */
}

/*!
 * \brief CScanSurface::LoadStlFile 载入STL文件，生成几何节点，并添加到Geode中
 * \param path
 */
void CScanSurface::LoadStlFile(const QString &path)
{
    QFile fileSTL(path);
    if (!fileSTL.open(QIODevice::ReadOnly)) {
        return;
    }
    QProgressDialog progress;
    progress.setLabelText(QString("正在读取STL文件：%1").arg(path));
    progress.setWindowModality(Qt::ApplicationModal);
    if (fileSTL.size() > 3 * 1024000) {                                            //文件大于3M的话，需要显示读取进度
        progress.setMinimumDuration(0);
    }
    qDebug() << "STL file size is: " << fileSTL.size();

    QTextStream stream(&fileSTL);

    triangleGeometry = new Geometry;                                            //三角片元几何
    ref_ptr<Vec3Array> vertexArray = new Vec3Array;                                                //顶点集合
    triangleGeometry->setVertexArray(vertexArray.get());
    ref_ptr<Vec2Array> tcoords = new Vec2Array;                                 //纹理坐标
    triangleGeometry->setTexCoordArray(0, tcoords.get());

    ref_ptr<Vec3Array> normArray = new Vec3Array;                               //法向量集合
    triangleGeometry->setNormalArray(normArray.get());
    triangleGeometry->setNormalBinding(Geometry::BIND_PER_VERTEX);

    /* 处理stlname */
    QString line = stream.readLine().toLocal8Bit();
    QString name = line.mid(6);                                                 //从第6位字符开始为stdyywr名
    qDebug() << "stl file name is: "<< name;
    line = stream.readLine();

    /* 提取三角片元 */
    while (!stream.atEnd() && line != "endsolid" && !progress.wasCanceled()) {
        if (fileSTL.pos() < fileSTL.size()) {
            progress.setValue(qreal(fileSTL.pos()) / qreal(fileSTL.size()) * 100);
        }
        const QStringList elementList = line.split(" ");
        const int size = elementList.size();
        if (size >= 4 && elementList[size - 4] == "normal") {
            QVector3D norm = QVector3D(elementList[size - 3].toDouble(),
                                       elementList[size - 2].toDouble(),
                                       elementList[size - 1].toDouble());
            normArray->push_back(Vec3(norm.x(), norm.y(), norm.z()));
            normArray->push_back(Vec3(norm.x(), norm.y(), norm.z()));
            normArray->push_back(Vec3(norm.x(), norm.y(), norm.z()));
        } else if (size >= 4 && elementList[size - 4] == "vertex") {
            QVector3D point = QVector3D(elementList[size - 3].toDouble(),
                                        elementList[size - 2].toDouble(),
                                        elementList[size - 1].toDouble());
            vertexArray->push_back(Vec3(point.x(), point.y(), point.z()));
            qreal u = 0;
            qreal v = 0;
            if (BSplineSurface::InversePointOnSurface(surface, point, u, v)) {
                tcoords->push_back(Vec2(u, v));                   //将曲面上的点对应的(u,v)添加到纹理坐标中
            }
        }
        line = stream.readLine();
    }
    triangleGeometry->addPrimitiveSet(new DrawArrays(PrimitiveSet::TRIANGLES, 0, vertexArray->size()));
    addDrawable(triangleGeometry.get());
    setName(name.toStdString());
}
