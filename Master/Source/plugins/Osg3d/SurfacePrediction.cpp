#include "SurfacePrediction.h"
#include "ui_SurfacePrediction.h"
#include "BSplineSurface.h"
#include "InverseSolver.h"
#include <QMessageBox>
#include <osg/Geometry>
#include <osgText/Text>
#include "OsgContainer.h"
#include "SurfaceMt.h"
#include "qtosgUtils.h"
#include "PathPlanning.h"

using namespace osg;

SurfacePrediction::SurfacePrediction(QList<Triangle> tris, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurfacePrediction)
{
    ui->setupUi(this);
    if (tris.isEmpty()) {

        return;
    }
    triangles = tris;
    root = ui->osgContainer->getRoot();
    scanPath = new Group;
    root->addChild(scanPath.get());
    surfaceMts = new Group;
    root->addChild(surfaceMts.get());

    osg::StateSet *state1 = scanPath->getOrCreateStateSet();
    osg::Material *material1 = new osg::Material;
    material1->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 0.0, 0.0, 1.0));
    material1->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 0.0, 0.0, 1.0));
    material1->setTransparency(osg::Material::FRONT_AND_BACK, 0.5);                   //透明
    state1->setAttributeAndModes(material1, osg::StateAttribute::PROTECTED | osg::StateAttribute::ON);
}

SurfacePrediction::~SurfacePrediction()
{
    delete ui;
}

void SurfacePrediction::DrawPath(const QVector<QVector3D> &path, QVector<QVector3D> direction)
{
    if (scanPath && scanPath->getNumChildren() > 0) {
        scanPath->removeChildren(0, scanPath->getNumChildren());
    }

    qDebug() << "Draw path" << path.size() << direction.size() << path.first() << path.last();
    qreal directionLineLength = path.first().distanceToPoint(path.last()) / 5;
    qDebug() << "Draw path: line length " << directionLineLength;
    if (!direction.isEmpty() && direction.size() == path.size()) {
        ref_ptr<Geometry> geome = new Geometry;
        ref_ptr<Vec3Array> vertexArray = new Vec3Array;
        geome->setVertexArray(vertexArray.get());
        ref_ptr<Vec4Array> colorArray = new Vec4Array;
        geome->setColorArray(colorArray.get());
        colorArray->push_back(Vec4(1.f, 0.5f, 0.f, 1.0f));
        geome->setColorBinding(Geometry::BIND_OVERALL);
        for(int index = 0; index < path.size(); ++index) {
            vertexArray->push_back(Vec3(path[index].x(), path[index].y(), path[index].z()));
            vertexArray->push_back(Vec3(path[index].x(), path[index].y(), path[index].z())
                                   + Vec3(direction[index].x(), direction[index].y(), direction[index].z()) * directionLineLength);
        }
        geome->addPrimitiveSet(new DrawArrays(PrimitiveSet::LINES, 0, vertexArray->size()));

        ref_ptr<Geode> node = new Geode;
        node->addDrawable(geome.get());
        scanPath->addChild(node.get());
    }
    QVector3D start = path.first();
    QVector3D end = path.last();
    qreal fontSize = start.distanceToPoint(end) / 10;
    DrawMyText(scanPath.get(), "S", start, fontSize);
    DrawMyText(scanPath.get(), "E", end, fontSize);
    qDebug() << "Surface Prediction: Succeed to draw path";
}

void SurfacePrediction::DrawMyText(Group *parent, const QString &text, const QVector3D &pos, qreal fontSize)
{
    if (!parent) {
        return;
    }
    //设置文字几何
    ref_ptr<osgText::Text> textGeom = new osgText::Text;
    Vec4 color(1.f, 1.f, 1.f, 1.f);
    textGeom->setColor(color);
    textGeom->setCharacterSize(fontSize);
    textGeom->setPosition(Vec3(pos.x(), pos.y(), pos.z()));
    textGeom->setAxisAlignment(osgText::Text::SCREEN);
    textGeom->setText(text.toStdString());
    //设置文字节点
    ref_ptr<Geode> textGeode = new Geode;
    textGeode->addDrawable(textGeom.get());
    //添加到machine
    parent->addChild(textGeode.get());
}

void SurfacePrediction::on_update_clicked()
{
    BSplineSurface surface;
    if (ui->surfaceFit->currentIndex() == 0) {
        QVector<QVector3D> pointCloud;
        for (Triangle t : triangles) {
            pointCloud << t.v1 << t.v2 << t.v3;
        }
        surface = BSplineSurface::FromInterpolationByCloud(pointCloud,
                                                 QVector3D(ui->projectionDirectionX->value(),
                                                           ui->projectionDirectionY->value(),
                                                           ui->projectionDirectionZ->value()),
                                                 ui->projectionYaw->value(),
                                                 ui->numInU->value(), ui->numInV->value(),
                                                 ui->orderU->value(), ui->orderV->value());
    } else if (ui->surfaceFit->currentIndex() == 1) {
        if (triangles.isEmpty()) {
            QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), QString("三角片元数据不存在，请采用点云模式"), 0, this);
            msgBox.exec();
        }
        PathPlanning::Matrix matrix = PathPlanning::CreateInterpolationsByTriangles(triangles, ui->cutStep->value(),
                                                      QVector3D(ui->stepX->value(), ui->stepY->value(), ui->stepZ->value()),
                                                      QVector3D(ui->sortX->value(), ui->sortY->value(), ui->sortZ->value()),
                                                      ui->cutNumInLine->value());
        if (ui->startDeleteNum->value()!=0 || ui->endDeleteNum->value()!=0) {
            matrix = PathPlanning::ClipInterpolationMatrix(matrix, ui->startDeleteNum->value(), ui->endDeleteNum->value());
        }
        surface = BSplineSurface::FromInterpolation(ui->cutOrderU->value(), ui->cutOrderV->value(), matrix);
    }
        if (surfaceMts) {
        surfaceMts->removeChildren(0, surfaceMts->getNumChildren());
    }
    ref_ptr<SurfaceMt> surfaceMt = new SurfaceMt(surface, NULL, surface.GetOrderU(), surface.GetOrderV());
    surfaceMts->addChild(surfaceMt.get());

    if (!surface.IsValid()) {                                                    //曲面为空
        QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), QString("曲面拟合失败，请检查数据"), 0, this);
        msgBox.exec();
        return;
    }

    //添加参考坐标系
    if (indicator) {
        root->removeChild(indicator.get());
    }
    const BoundingSphere bound = root->getBound();                              //根据root的包围球自动设置坐标轴的尺度
    indicator = new OriginIndicator("Preference", qMax<qreal>(10, bound.radius() / 3), toVector3D(bound.center()));
    root->addChild(indicator.get());

    //预测一下条线
    qDebug() << "Surface Prediction: Succeed to build a surface";
    qreal u = 0, v = 0;
    QString feedDirection = ui->feedDirection->currentText();
    qreal L  = 0;
    QVector3D pointPrevious = surface.PointOnSurface(u, v);
    while (u < 1) {
        u += 0.001;
        QVector3D pointCurrent;
        if (feedDirection == "底部") {
            v = 0;
            pointCurrent = surface.PointOnSurface(u, v);
        } else if (feedDirection == "上部") {
            v = 1;
            pointCurrent = surface.PointOnSurface(u, v);
        } else if (feedDirection == "左侧") {
            v = 0;
            pointCurrent = surface.PointOnSurface(v, u);
        } else if (feedDirection == "右侧") {
            v = 1;
            pointCurrent = surface.PointOnSurface(v, u);
        }
        L += pointPrevious.distanceToPoint(pointCurrent);
        pointPrevious = pointCurrent;
    }
    const int numOfScanPoints = int(L / ui->scanInterval->value());
    qreal duv = 1.0 / (50 * numOfScanPoints);
    qDebug() << "Surface Prediction: duv" << duv;

    line.clear();
    direction.clear();

    //开始计算扫描点
    int count = 0;
    //确定pointPrevious
    if (ui->scanDirection->currentText() == "正向") {
        u = 0;
    } else {
        u = 1;
    }
    if (feedDirection == "底部") {
        v = 0;
        pointPrevious = surface.PointOnSurface(u, v);
    } else if (feedDirection == "上部") {
        v = 1;
        pointPrevious = surface.PointOnSurface(u, v);
    } else if (feedDirection == "左侧") {
        v = 0;
        pointPrevious = surface.PointOnSurface(v, u);
    } else if (feedDirection == "右侧") {
        v = 1;
        pointPrevious = surface.PointOnSurface(v, u);
    }
    qreal dL = 0;
    while (u <= 1 && u >=0 && count <= numOfScanPoints) {
        QVector3D pointCurrent;
        while (dL < ui->scanInterval->value() * count && u <= 1 && u >= 0) {
            if (ui->scanDirection->currentText() == "正向") {
                u += duv;
            } else {
                u -= duv;
            }
            if (feedDirection == "底部") {
                v = 0;
                pointCurrent = surface.PointOnSurface(u, v);
            } else if (feedDirection == "上部") {
                v = 1;
                pointCurrent = surface.PointOnSurface(u, v);
            } else if (feedDirection == "左侧") {
                v = 0;
                pointCurrent = surface.PointOnSurface(v, u);
            } else if (feedDirection == "右侧") {
                v = 1;
                pointCurrent = surface.PointOnSurface(v, u);
            }
            dL += pointCurrent.distanceToPoint(pointPrevious);
            pointPrevious = pointCurrent;
        }
        if (count != 0 && (u == 1 || u == 0)) {
            break;          //舍弃最后一个点，避免造成速度冲击
        }
        BSplineSurface::Matrix dx;
        BSplineSurface::Matrix dy;
        BSplineSurface::Matrix dz;
        QVector3D feedDir;
        QVector3D normDir;
        if (feedDirection == "底部") {
            v = 0;
            dx = surface.SurfaceDerivative(u, v, 1, Qt::XAxis);
            dy = surface.SurfaceDerivative(u, v, 1, Qt::YAxis);
            dz = surface.SurfaceDerivative(u, v, 1, Qt::ZAxis);
            feedDir = -QVector3D(dx(0, 1), dy(0, 1), dz(0, 1));
            normDir = surface.NormVector(u, v);
        } else if (feedDirection == "上部") {
            v = 1;
            dx = surface.SurfaceDerivative(u, v, 1, Qt::XAxis);
            dy = surface.SurfaceDerivative(u, v, 1, Qt::YAxis);
            dz = surface.SurfaceDerivative(u, v, 1, Qt::ZAxis);
            feedDir = QVector3D(dx(0, 1), dy(0, 1), dz(0, 1));
            normDir = surface.NormVector(u, v);
        } else if (feedDirection == "左侧") {
            v = 0;
            dx = surface.SurfaceDerivative(v, u, 1, Qt::XAxis);
            dy = surface.SurfaceDerivative(v, u, 1, Qt::YAxis);
            dz = surface.SurfaceDerivative(v, u, 1, Qt::ZAxis);
            feedDir = -QVector3D(dx(1, 0), dy(1, 0), dz(1, 0));
            normDir = surface.NormVector(v, u);
        } else if (feedDirection == "右侧") {
            v = 1;
            dx = surface.SurfaceDerivative(v, u, 1, Qt::XAxis);
            dy = surface.SurfaceDerivative(v, u, 1, Qt::YAxis);
            dz = surface.SurfaceDerivative(v, u, 1, Qt::ZAxis);
            feedDir = QVector3D(dx(1, 0), dy(1, 0), dz(1, 0));
            normDir = surface.NormVector(v, u);
        }
        feedDir.normalize();
        line.append(pointPrevious + ui->feed->value() * feedDir);
        if (ui->normDirection->currentText() == "正向") {
            direction.append(normDir);
        } else {
            direction.append(-normDir);
        }
        count++;
        qDebug() << "Surface Prediction: count " << count << line.last();
    }

    DrawPath(line, direction);
}

void SurfacePrediction::on_finish_clicked()
{
    isValid = true;
    close();
}
