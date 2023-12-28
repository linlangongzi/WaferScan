#include "arsCore/Platform.h"
#include <Ultrasonic/UTPose.h>
#include <Motion/TrioDriver.h>
#include "Part.h"
#include "PathPlanningWizard.h"
#include <Acquisition/ScanPlanModel.h>
#include "ui_PathPlanningWizard.h"
#include <Motion/MotionPath.h>
#include <osgDB/ReadFile>
#include <osgText/Text>
#include <QFileDialog>
#include <QFormLayout>
#include <QMessageBox>
#include "OsgContainer.h"
#include "qtosgUtils.h"
#include <allrising/Util.h>

using namespace osg;

PathPlanningWizard::PathPlanningWizard(QList<Triangle> tris, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PathPlanningWizard)
{
    ui->setupUi(this);

    pathPlanning = new PathPlanning;
    if (!tris.isEmpty()) {                                                      //tris非空，说明是从工件进入路径规划
        triangles = tris;
        ui->fromCloudOrCAD->setChecked(true);
        for (Triangle t : triangles) {
            pointCloud << t.v1 << t.v2 << t.v3;
        }
    }
    ui->surfaceFit->setCurrentIndex(0);
    root = ui->osgContainer->getRoot();
    pointGeo = osgDB::readNodeFile("sphere.STL");

    points = new Group;
    root->addChild(points.get());
    surfaceMts = new Group;
    root->addChild(surfaceMts);

    ui->stackedWidget->setCurrentIndex(0);
    ui->previous->setEnabled(false);
}

PathPlanningWizard::PathPlanningWizard(QVector<QVector<QVector3D> > interpolations, QWidget *parent) :
    QDialog(parent), ui(new Ui::PathPlanningWizard)
{
    ui->setupUi(this);
    pathPlanning = new PathPlanning;
    if (!interpolations.isEmpty()) {
        ui->surfaceFit->widget(1)->setEnabled(false);                           //仿形数据不支持cut截平面模式
        ui->fromSample->setChecked(true);
        ui->fromCloudOrCAD->setEnabled(false);
        ui->openFile->setEnabled(false);
        ui->projectionDirectionX->setEnabled(false);
        ui->projectionDirectionY->setEnabled(false);
        ui->projectionDirectionZ->setEnabled(false);
        ui->projectionYaw->setEnabled(false);
        ui->numInU->setEnabled(false);
        ui->numInV->setEnabled(false);
        ui->orderU->setEnabled(true);
        ui->orderV->setEnabled(true);
    }

    root = ui->osgContainer->getRoot();
    pointGeo = osgDB::readNodeFile("sphere.STL");
    points = new Group;
    root->addChild(points.get());
    surfaceMts = new Group;
    root->addChild(surfaceMts);

    ui->stackedWidget->setCurrentIndex(0);
    ui->previous->setEnabled(false);

    int col = interpolations.size();
    int row = interpolations.first().size();
    int size[3] = {row, col, 3};
    BSplineSurface::Matrix matrixPoints(3, size);
    for (int r = 0; r < row; ++r) {
        for (int c = 0; c < col; ++c) {
            for (int dim = 0; dim < 3; ++dim) {
                matrixPoints(r, c, dim) = interpolations[c][r][dim];
            }
            ref_ptr<PointMt> pointMt = new PointMt(interpolations[c][r], NULL);
            points->addChild(pointMt.get());                                          //将插值点添加到显示中
        }
    }
    interpolationPoints = matrixPoints;
}

PathPlanningWizard::~PathPlanningWizard()
{
    delete ui;
}

/*!
 * \brief PathPlanningWizard::Update 更新界面
 */
void PathPlanningWizard::Update()
{
    static BSplineSurface surface;
    static BSplineSurface surfaceExtend;
    static BSplineSurface surfaceResample;

    switch (ui->stackedWidget->currentIndex()) {
    case 0: {
        if (ui->fromCloudOrCAD->isChecked()) {
            if (ui->surfaceFit->currentIndex() == 0) {
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
                    break;
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
        } else if (ui->fromSample->isChecked()) {
            surface = BSplineSurface::FromInterpolation(ui->orderU->value(), ui->orderV->value(), interpolationPoints);
        }
        if (!surface.IsValid()) {
            QMessageBox msgBox(QMessageBox::Warning, tr("Warning"), QString("曲面拟合失败，请检查相关设置及数据源"), 0, this);
            msgBox.exec();
            break;
        }
        ClearSurfaceMts();
        ClearPath();
        ref_ptr<SurfaceMt> surfaceMt = new SurfaceMt(surface, pointGeo, ui->orderU->value(), ui->orderV->value());
        surfaceMts->addChild(surfaceMt);
        /* 重置曲面重采样的密度,保持与原数据不变，以防精度丢失 */
        ui->resampleU->setValue(surface.GetCtrlPoints().size[0]);
        ui->resampleV->setValue(surface.GetCtrlPoints().size[1]);
        break;
    }
    case 1: {
        if (!surface.IsValid()) {
            return;
        }
        surfaceResample = surface;
        if (ui->resampleU->value() != surfaceResample.GetCtrlPoints().size[0]
            || ui->resampleV->value() != surfaceResample.GetCtrlPoints().size[1]) {
            surfaceResample = BSplineSurface::SurfaceResample(surfaceResample, ui->resampleU->value(), ui->resampleV->value());
        }
        ClearSurfaceMts();
        ClearPath();
        ref_ptr<SurfaceMt> surfaceMt = new SurfaceMt(surfaceResample, pointGeo,
                                                     ui->orderU->value(), ui->orderV->value());
        surfaceMts->addChild(surfaceMt);
        break;
    }
    case 2: {
        if (!surfaceResample.IsValid()) {
            return;
        }
        surfaceExtend = surfaceResample;
        if (!qFuzzyCompare(ui->extendTop->value(), 0)) {
            surfaceExtend = BSplineSurface::SurfaceExtend(surfaceExtend, Qt::TopEdge, ui->extendTop->value(),
                                                          surfaceExtend.GetOrderU(), surfaceExtend.GetOrderV());
        }
        if (!qFuzzyCompare(ui->extendBottom->value(), 0)) {
            surfaceExtend = BSplineSurface::SurfaceExtend(surfaceExtend, Qt::BottomEdge, ui->extendBottom->value(),
                                                          surfaceExtend.GetOrderU(), surfaceExtend.GetOrderV());
        }
        if (!qFuzzyCompare(ui->extendLeft->value() , 0)) {
            surfaceExtend = BSplineSurface::SurfaceExtend(surfaceExtend, Qt::LeftEdge, ui->extendLeft->value(),
                                                          surfaceExtend.GetOrderU(), surfaceExtend.GetOrderV());
        }
        if (!qFuzzyCompare(ui->extendRight->value(), 0)) {
            surfaceExtend = BSplineSurface::SurfaceExtend(surfaceExtend, Qt::RightEdge, ui->extendRight->value(),
                                                          surfaceExtend.GetOrderU(), surfaceExtend.GetOrderV());
        }

        ClearSurfaceMts();
        ClearPath();
        ref_ptr<SurfaceMt> surfaceMt = new SurfaceMt(surfaceExtend, pointGeo,
                                                     ui->orderU->value(), ui->orderV->value());
        surfaceMts->addChild(surfaceMt);
        break;
    }
    case 3: {
        pathPlanning->SetTriangles(triangles);
        if (!surfaceExtend.IsValid()) {
            qDebug() << "Triangles size: " << triangles.size();
            if (triangles.isEmpty()) {
                return;
            }
            ui->pathPlanningType->setCurrentText("等截面法");
        } else{
            pathPlanning->SetSurface(surfaceExtend);
            if (ui->pathPlanningPosition->currentText() == "左侧") {
                pathPlanningPosition = Qt::LeftEdge;
            } else if (ui->pathPlanningPosition->currentText() == "右侧") {
                pathPlanningPosition = Qt::RightEdge;
            } else if (ui->pathPlanningPosition->currentText() == "上部") {
                pathPlanningPosition = Qt::TopEdge;
            } else {
                pathPlanningPosition = Qt::BottomEdge;
            }
        }
        if (ui->pathPlanningType->currentText() == "等弧长进给") {
            pathPlanningType = PathPlanning::IsoArc;
        } else if (ui->pathPlanningType->currentText() == "等参数进给") {
            pathPlanningType = PathPlanning::IsoParametric;
        } else if (ui->pathPlanningType->currentText() == "等截面法") {
            pathPlanningType = PathPlanning::IsoPlane;
        }
        if (ui->scanDirection->currentText() == "反向") {
            scanDirection = PathPlanning::Backward;
        } else {
            scanDirection = PathPlanning::Forward;
        }
        if (ui->surfaceNormType->currentText() == "正向") {
            surfaceNormType = true;
        } else {
            surfaceNormType = false;
        }
        QVector3D feedDirection = QVector3D(ui->feedDirectionX->value(), ui->feedDirectionY->value(), ui->feedDirectionZ->value());
        QVector3D sortDirection = QVector3D(ui->cutSortX->value(), ui->cutSortY->value(), ui->cutSortZ->value());
        motionPath = pathPlanning->CalculatePath(ui->feedInterval->value() ,ui->scanInterval->value(),
                                                pathPlanningType, ui->incidence->value(),
                                                ui->azimuth->value(), pathPlanningPosition,
                                                scanDirection, surfaceNormType, ui->waterPath->value(), ui->speed->value(),
                                                ui->startIgnore->value(), ui->endIgnore->value(),
                                                feedDirection, sortDirection);
        QVector<QVector<QVector3D> > path = pathPlanning->GetPath(ui->startIgnore->value(), ui->endIgnore->value());
        QVector<QVector<QVector3D> > direction = pathPlanning->GetPathDirection(ui->startIgnore->value(), ui->endIgnore->value());
        if (path.isEmpty()) {
            qDebug() << "fail to plan path";
            return;
        }
        qDebug() << "Path line num: " << path.size();
        ClearPath();
        DrawPath(path, direction);
        break;
    }
    }
    //添加参考坐标系
    if (indicator) {
        root->removeChild(indicator.get());
    }
    const BoundingSphere bound = root->getBound();                              //根据root的包围球自动设置坐标轴的尺度
    indicator = new OriginIndicator("Preference", qMax<qreal>(10, bound.radius() / 3), toVector3D(bound.center()));
    root->addChild(indicator.get());
}

void PathPlanningWizard::DrawPath(const QVector<QVector3D> &path, QVector<QVector3D> direction)
{
    //定义路径几何点
    qreal directionLineLength = path.first().distanceToPoint(path.last()) / 5;

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
        points->addChild(node.get());
    }
}

void PathPlanningWizard::DrawPath(const PathPlanning::Matrix &path)
{
    int size[3] = {path.size[0], path.size[1], 3};
    QVector3D start = QVector3D(path(0, 0, 0), path(0, 0, 1), path(0, 0, 2));
    QVector3D end = QVector3D(path(size[0] - 1, 0, 0),path(size[0] - 1, 0, 1), path(size[0] - 1, 0, 2));
    qreal fontSize = start.distanceToPoint(end) / 10;
    for (int row = 0; row < size[1]; ++ row) {
        QVector<QVector3D> line;
        for (int col = 0; col < size[0]; ++col) {
            QVector3D point(path(col, row, 0), path(col, row, 1), path(col, row, 2));
            line.append(point);
        }
//        qDebug() << QString("Draw lines %1/%2").arg(row + 1).arg(size[1]);
        DrawPath(line, QVector<QVector3D>());
    }
    DrawMyText(points.get(), "S", start, fontSize);
    DrawMyText(points.get(), "E", end, fontSize);
}

void PathPlanningWizard::DrawPath(const QVector<QVector<QVector3D> > &path, const QVector<QVector<QVector3D> > &direction)
{
    if (path.isEmpty() || direction.isEmpty()) {
        return;
    }
    QVector3D start = path.first().first();
    QVector3D end = path.first().last();
    qreal fontSize = start.distanceToPoint(end) / 10;
    if (path.size() == direction.size()) {
        for (int index= 0 ; index < path.size(); ++index) {
            DrawPath(path[index], direction[index]);
        }
    }
    DrawMyText(points.get(), "S", start, fontSize);                  //画出第一条线的起终点
    DrawMyText(points.get(), "E", end, fontSize);
}

void PathPlanningWizard::DrawMyText(Group *parent, const QString &text, const QVector3D &pos, qreal fontSize)
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

void PathPlanningWizard::ClearPath()
{
    if (points) {
        points->removeChildren(0, points->getNumChildren());
    }
}

void PathPlanningWizard::ClearSurfaceMts()
{
    if (surfaceMts) {
        surfaceMts->removeChildren(0, surfaceMts->getNumChildren());
    }
}

void PathPlanningWizard::on_previous_clicked()
{
    ui->next->setEnabled(true);
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() - 1);
    if (ui->stackedWidget->currentIndex() == 0) {
        ui->previous->setEnabled(false);
    }
    if (ui->stackedWidget->currentIndex() <= ui->stackedWidget->count() - 2) {
        ui->next->setText(QString("下一步"));
    }
    Update();
}

void PathPlanningWizard::on_next_clicked()
{
    ui->previous->setEnabled(true);
    if (ui->stackedWidget->currentIndex() == ui->stackedWidget->count() - 1) {  /* 路径规划完成后生成扫描计划 */
        QJsonObject obj;
        obj["Type"] = "ScanPlan";
        obj["MotionPath"] = motionPath.toJson();
        obj["MotionType"] = "CAM";
        ScanPlanModel *model = ScanPlanModel::instance();
        model->insertNewScanPlan(obj, model->index(0, 0, QModelIndex()));
        if (ui->fromSample->isChecked()) {
            qDebug() << "Emit generateMotionPath()";
        }
        close();
        return;
    } else if (ui->stackedWidget->currentIndex() == ui->stackedWidget->count() - 2) {
        ui->next->setText(QString("完成"));
    }
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() + 1);
    Update();
}

void PathPlanningWizard::on_cancel_clicked()
{
    close();
}

void PathPlanningWizard::on_update_clicked()
{
    Update();
}

void PathPlanningWizard::on_openFile_clicked()
{
    if (ui->fromCloudOrCAD->isChecked()) {
        QString path = QFileDialog::getOpenFileName(this, QString("读取点云数据"), QString(), QString("点云数据(*.csv)"));
        QDir dir(path);
        ui->filePath->setText(dir.dirName());
        QList<QList<double> > data = ReadCsvAsDouble(path);
        if (data.isEmpty()) {
            return;
        }
        for (auto p: data) {
            if (p.size() == 3) {
                pointCloud.append(QVector3D(p[0], p[1], p[2]));
            }
        }
        ui->surfaceFit->widget(1)->setEnabled(false);
    } else {
        QString path = QFileDialog::getOpenFileName(this, QString("读取仿形数据"), QString(), QString("仿形数据(*.csv)"));
        ui->filePath->setText(path);
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }
        QTextStream csv(&file);
        QString line = csv.readLine();
        QStringList list = line.split(",");
        int col = list.value(0).toInt();
        int row = list.value(1).toInt();
        ui->numInU->setValue(col);
        ui->numInV->setValue(row);
        int size[3] = {row, col, 3};
        interpolationPoints = BSplineSurface::Matrix(3, size);
        for (int c = 0; c < col; ++c) {
            for (int r = 0; r < row; ++r) {
                line = csv.readLine();
                list = line.split(",");
                if (list.size() == 3) {
                    QVector3D p;
                    for (int dim = 0; dim < 3; ++dim) {
                        p[dim] = list[dim].toDouble();
                        interpolationPoints(r, c, dim) = p[dim];
                    }
                    ref_ptr<PointMt> pointMt = new PointMt(p, NULL);
                    points->addChild(pointMt);                                  //将插值点添加到显示中
                }
            }
        }
    }

}

void PathPlanningWizard::on_fromSample_clicked()
{
    ui->projectionDirectionX->setEnabled(false);
    ui->projectionDirectionY->setEnabled(false);
    ui->projectionDirectionZ->setEnabled(false);
    ui->projectionYaw->setEnabled(false);
    ui->numInU->setEnabled(false);
    ui->numInU->setEnabled(false);
    ui->surfaceFit->widget(1)->setEnabled(false);
}

void PathPlanningWizard::on_fromCloudOrCAD_clicked()
{
    ui->projectionDirectionX->setEnabled(true);
    ui->projectionDirectionY->setEnabled(true);
    ui->projectionDirectionZ->setEnabled(true);
    ui->projectionYaw->setEnabled(true);
    ui->numInU->setEnabled(true);
    ui->numInU->setEnabled(true);
}

void PathPlanningWizard::on_getPathPlanningType_currentIndexChanged(const QString &value)
{
    if (value == "等截面法") {
        ui->feedDirectionX->setEnabled(true);
        ui->feedDirectionY->setEnabled(true);
        ui->feedDirectionZ->setEnabled(true);
        ui->cutSortX->setEnabled(true);
        ui->cutSortY->setEnabled(true);
        ui->cutSortZ->setEnabled(true);
        ui->pathPlanningPosition->setEnabled(false);
    } else {
        ui->feedDirectionX->setEnabled(false);
        ui->feedDirectionY->setEnabled(false);
        ui->feedDirectionZ->setEnabled(false);
        ui->cutSortX->setEnabled(false);
        ui->cutSortY->setEnabled(false);
        ui->cutSortZ->setEnabled(false);
        ui->pathPlanningPosition->setEnabled(true);
    }
}
