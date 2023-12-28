#include "SurfaceContouring.h"
#include "ui_SurfaceContouring.h"
#include "InverseSolver.h"
#include "arsCore/Platform.h"
#include <Acquisition/ScanPlanModel.h>
#include <QItemSelectionModel>
#include <Ultrasonic/UTChannel.h>
#include "NurbsSurface.h"
#include "OsgContainer.h"
#include "PathPlanningWizard.h"
#include "SurfaceMt.h"
#include "SurfacePrediction.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <osg/StateSet>
#include <allrising/Util.h>
#include "qtosgUtils.h"
#include <osg/BoundingSphere>

using namespace osg;

SurfaceContouring::SurfaceContouring(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurfaceContouring)
{
    ui->setupUi(this);
    ui->cloudMode->setEnabled(false);
    ui->feedMode->setEnabled(false);

    channel = UltrasonicManager::channel(0);
    connect(channel, SIGNAL(MonitorDataArrived(UTDataFrame)), this, SLOT(newDataArrived(UTDataFrame)));
    connect(ScanController::instance(), SIGNAL(StateChanged(int)), this, SLOT(onStateChangedForScanController(int)));

    root = ui->osgContainer->getRoot();

    currentPointMt = new PointMt(currentPoint, NULL);
    root->addChild(currentPointMt.get());
    osg::StateSet *state1 = currentPointMt->getOrCreateStateSet();
    osg::Material *material1 = new osg::Material;
    material1->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 0.0, 0.0, 1.0));
    material1->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 0.0, 0.0, 1.0));
    material1->setTransparency(osg::Material::FRONT_AND_BACK, 0.5);                   //透明
    state1->setAttributeAndModes(material1, osg::StateAttribute::PROTECTED | osg::StateAttribute::ON);

    pointMts = new osg::Group;
    root->addChild(pointMts.get());
    osg::StateSet *state2 = pointMts->getOrCreateStateSet();
    osg::Material *material2 = new osg::Material;
    material2->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(.0, 0.0, 1.0, 1.0));
    material2->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(.0, 0.0, 1.0, 1.0));
    material2->setTransparency(osg::Material::FRONT_AND_BACK, 0.5);                   //透明
    state2->setAttributeAndModes(material2, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);


    parts = new osg::Group;
    root->addChild(parts.get());

    scanPath = new osg::Group;
    root->addChild(scanPath.get());

    surfaceMts = new osg::Group;
    root->addChild(surfaceMts.get());
}

SurfaceContouring::~SurfaceContouring()
{
    delete ui;
}

/*!
 * \brief 执行扫描计划
 */
void SurfaceContouring::StartNextScanPlan(MotionPath mp)
{
    //当前扫描控制器为IDLE时，才可开始扫描
    qDebug() << "scan controller state is: " << ScanController::instance()->currentState();
    static const char DEFAULT_NAME[] = "曲面自动重建";
    QJsonObject obj;
    obj["Name"] = DEFAULT_NAME;
    obj["Type"] = "ScanPlan";
    obj["MotionType"] = "CAM";
    ScanPlanModel *model = ScanPlanModel::instance();
    ScanPlan *root = model->root();

    ScanPlan *scanPlan = root->findChild(DEFAULT_NAME);
    if (!scanPlan) {
        scanPlan = model->insertNewScanPlan(obj, model->index(0, 0, QModelIndex()));
    }
    scanPlan->setMotionPath(mp);
    qDebug() << "Into start next scan plan";
    ScanController::instance()->LoadScanPlan(scanPlan);
    ScanController::instance()->Start();

    if (ScanController::instance()->currentState() != ScanController::RUNNING) {
        qWarning() << "Fail starting scanController : " << ScanController::instance()->currentState();
    } else {
        if (ui->radioCloud->isChecked()) {                                      //采用点云模式时，清空数据
            rawLine.clear(); //开始下一次扫描前清空rawLine
        } else if (ui->radioFeed->isChecked()) {
            newLine.clear();
        }
    }
}

void SurfaceContouring::newDataArrived(const UTDataFrame &frame)
{
    currentPoint = frame.UpdatePosition().first;                           //焦斑点，亦可用来实时显示在osgContainer中
    currentPointMt->setPosition(currentPoint);                                  //实时更新位置
    if (!frame.IsIGateTriggered()) {
        return;
    }
    if (frame.Context.IsReverse(1)) {                                      //删除Y正向数据、X反向数据。X正向与Y负向匹配，主要受扫描线方向影响
        return;
    }
    if (isRunning) {
        if (ui->radioCloud->isChecked()) {
            rawLine.append(currentPoint);
            qDebug() <<  "Raw point: " << currentPoint;
        } else if (ui->radioFeed->isChecked()) {
            newLine.append(currentPoint);
            qDebug() <<  "New point: " << currentPoint;
        }
    }
}

void SurfaceContouring::onStateChanged(ScanPlan::ScanStatus state)
{
    isRunning = (state == ScanPlan::RUNNING);
}

/*!
 * \brief 用来捕获扫描计划中活动的scanPlan
 */
void SurfaceContouring::onStateChangedForScanController(int state)
{
    if (state == ScanController::RUNNING) { // FIXME: 将扫描状态的追踪更改为ScanTask
        Q_UNUSED(state)
//        ScanPlan *rootScanPlan = ScanController::instance()->rootScanPlan();
//        connect(rootScanPlan, &ScanPlan::ScanStatusChanged, this, &SurfaceContouring::onStateChanged, Qt::UniqueConnection);
    }
}

void SurfaceContouring::on_startFeed_clicked()                                  //手动模式下，点击开始步进后，开始预测+扫描
{
    if (!newLine.isEmpty()) {
        rawLine.append(newLine);                                          //将新采集的线加入进来
    }

    parts->removeChildren(0, parts->getNumChildren());
    osg::ref_ptr<Part> part = new Part();
    parts->addChild(part.get());
    if (rawLine.isEmpty()) {
        return;
    }
    if (!part->CreatePartNodeFromCloud(rawLine)) {
        qDebug() << "Failed create part from cloud points";
        return;
    }
    QList<Triangle> tris = part->GetTriangles();
    if (tris.isEmpty()) {
        return;
    }
    //利用SurfacePrediction来生成下一条扫描线
    SurfacePrediction dialog(tris);
    dialog.exec();
    if (!dialog.IsValid()) {
        return;
    }
    QVector<QVector3D> line = dialog.GetPath();                                 //路径点及法向量
    QVector<QVector3D> direction = dialog.GetDirection();
    //生成扫描计划
    MotionPath mp;
    QList<int> axex;
    axex << 0 << 1 << 2 << 3 << 4;
    MotionAxisList axisList(axex);
    MotionElement ele;
    ele.type = MotionElement::Cam;
    ele.setAxisList(axisList);

    for (int posIndex = 0; posIndex < line.size(); ++posIndex) {
        if (direction[posIndex].z() < 0) {
            direction[posIndex] = -direction[posIndex];
        }
        QVector<qreal> jointPos = InverseSolver::Inverse(line[posIndex], direction[posIndex], ui->waterPath->value());
        MotionPose motionPose;
        for (int index = 0; index < 5; ++index) {
            if (index == 2 || index == 3) {
                motionPose.append(-jointPos[index]);
            } else {
                motionPose.append(jointPos[index]);
            }
        }
        ele.pos.append(motionPose);
    }
    ele.speed = ui->speed->value();
    ele.stepSpeed = ui->speed->value();
    ele.stepInterval = 1;
    mp.elements.append(ele);
    StartNextScanPlan(mp);

    //添加参考坐标系
    AddIndicator();
}
void SurfaceContouring::on_sample_clicked()
{
    initeInterpolationPoints.append(currentPoint);
    osg::ref_ptr<PointMt> pointMt = new PointMt(currentPoint, NULL);
    pointMts->addChild(pointMt);                                                //将采样点显示在osgContainer中

    sampleIndex++;
    if (sampleIndex == ui->numInLine->value() * 3) {
        ui->labelSampleIndex->setText(QString("完成初始曲面采点"));
        ui->sample->setEnabled(false);
        ui->numInLine->setEnabled(false);
        ui->radioCloud->setEnabled(true);
        ui->radioFeed->setEnabled(true);
        ui->finishConstruction->setEnabled(true);
        //将初始曲面插值点转换给interpolationPoints
        interpolationPoints = QVector<QVector<QVector3D> >(3, QVector<QVector3D>(ui->numInLine->value()));
        for (int i = 0; i < ui->numInLine->value() * 3; ++i) {
            interpolationPoints[i / ui->numInLine->value()][i % ui->numInLine->value()] = initeInterpolationPoints[i];
        }
        qDebug() << "initeInterpolationPoints: " << initeInterpolationPoints;
    } else {
        ui->labelSampleIndex->setText(QString("第%1行，第%2个点").arg((sampleIndex) / ui->numInLine->value() + 1).arg((sampleIndex) % ui->numInLine->value() + 1));
    }
}

void SurfaceContouring::on_finishConstruction_clicked()
{
    if (ui->radioFeed->isChecked()) {
        //保存仿形结果，保存插值点即可
        QString filePath = QFileDialog::getSaveFileName(this, QString("保存仿形结果"), QString(), QString("sample(*.csv)"));
        QFile csvFile(filePath);
        if (!csvFile.open(QIODevice::WriteOnly)) {
            return;
        }
        QTextStream streamOut(&csvFile);
        if (interpolationPoints.isEmpty()) {
            return;
        }
        streamOut << interpolationPoints.size() <<","
                  << interpolationPoints.first().size()<< endl;
        for (QVector<QVector3D> line : interpolationPoints) {                       //一条线一条线地保存
            for (QVector3D p : line) {
                streamOut << p.x() << ","
                          << p.y() << ","
                          << p.z() << endl;
            }
        }
    } else if (ui->radioCloud->isChecked()) {
        //将rawLine保存到csv
        QString filePath = QFileDialog::getSaveFileName(this, QString("保存仿形的点云数据"), QString(), QString("Cloud Points(*.csv)"));
        QFile csvFile(filePath);
        if (!csvFile.open(QIODevice::WriteOnly)) {
            return;
        }
        QTextStream streamOut(&csvFile);
        QProgressDialog progress;
        progress.setLabelText(QString("正在保存点云数据：%1").arg(filePath));
        progress.setWindowModality(Qt::ApplicationModal);
        for (int i = 0; i < rawLine.size(); ++i) {
            QVector3D p = rawLine[i];
            streamOut << p.x() << ","
                      << p.y() << ","
                      << p.z() << endl;
            progress.setValue(qreal(i) / qreal(rawLine.size()) * 100);
        }
    }
}

void SurfaceContouring::on_resetFeed_clicked()
{
    newLine.clear();
    on_startFeed_clicked();
}

/*!
 * \brief 调用路径规划向导生成一个扫描计划
 */
void SurfaceContouring::on_firstCloudMode_clicked()
{
    ui->firstCloudMode->setEnabled(false);
    ui->startCloudMode->setEnabled(true);
    qDebug() << "Into first cloud mode";
    qDebug() << interpolationPoints;
    PathPlanningWizard dialog(interpolationPoints, this);
    qDebug() << "New pathplanningWizard";
    dialog.exec();
    MotionPath mp = dialog.GetMotionPath();
    if (mp.elements.isEmpty()) {
        return;
    }
    StartNextScanPlan(mp);
    //添加参考坐标系
    AddIndicator();
}

void SurfaceContouring::on_startCloudMode_clicked()
{
    qDebug() << "Into first cloud mode";
    parts->removeChildren(0, parts->getNumChildren());
    osg::ref_ptr<Part> part = new Part();
    parts->addChild(part.get());
    if (rawLine.isEmpty()) {
        return;
    }
    //利用Part可以将点云进行三角片元化
    if (!part->CreatePartNodeFromCloud(rawLine)) {
        qDebug() << "Failed create part from cloud points";
        return;
    }
    //利用Part可以对点云三角片元化结果进行路径规划，开启一个路径规划向导
    QList<Triangle> tris = part->GetTriangles();
    if (tris.isEmpty()) {
        return;
    }
    PathPlanningWizard dialog(tris, this);
    dialog.exec();
    MotionPath mp = dialog.GetMotionPath();
    if (mp.elements.isEmpty()) {
        return;
    }
    StartNextScanPlan(mp);
    AddIndicator();
}

void SurfaceContouring::AddIndicator()
{
    //添加参考坐标系
    if (indicator) {
        root->removeChild(indicator.get());
    }
    const BoundingSphere bound = root->getBound();                              //根据root的包围球自动设置坐标轴的尺度
    indicator = new OriginIndicator("Preference", qMax<qreal>(10, bound.radius() / 3), toVector3D(bound.center()));
    root->addChild(indicator.get());
}


void SurfaceContouring::on_radioFeed_clicked()
{
    if (ui->radioFeed->isChecked()) {
        ui->feedMode->setEnabled(true);
        ui->cloudMode->setEnabled(false);
    }
}

void SurfaceContouring::on_radioCloud_clicked()
{
    if (ui->radioCloud->isChecked()) {
        ui->feedMode->setEnabled(false);
        ui->cloudMode->setEnabled(true);
    }
}

void SurfaceContouring::on_openSampleFile_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, QString("读取仿形数据"), QString(), QString("仿形数据(*.csv)"));
    QFile file(path);
    QDir dir(path);
    ui->filePath->setText(dir.dirName());
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    if (pointMts->getNumChildren() > 0) {
        pointMts->removeChildren(0, pointMts->getNumChildren());
    }
    QTextStream csv(&file);
    QString line = csv.readLine();
    QStringList list = line.split(",");
    int row = list.value(0).toInt();
    int col = list.value(1).toInt();
    interpolationPoints = QVector<QVector<QVector3D> >(row, QVector<QVector3D>(col));

    for (int r = 0; r < row; ++r) {
        for (int c = 0; c < col; ++c) {
            line = csv.readLine();
            list = line.split(",");
            if (list.size() == 3) {
                for (int dim = 0; dim < 3; ++dim) {
                    qreal value = list[dim].toDouble();
                    interpolationPoints[r][c][dim] = value;
                }
                osg::ref_ptr<PointMt> pointMt = new PointMt(interpolationPoints[r][c], NULL);
                pointMts->addChild(pointMt.get());                                  //将插值点添加到显示中
            }
        }
    }
    ui->sample->setEnabled(false);
    ui->numInLine->setEnabled(false);
    ui->radioCloud->setEnabled(true);
    ui->radioFeed->setEnabled(true);
    ui->radioFeed->setChecked(true);
    ui->finishConstruction->setEnabled(true);
    ui->startCloudMode->setEnabled(false);
    qDebug() << "Succeed to open sample file";
    qDebug() << interpolationPoints;

    AddIndicator();
}

void SurfaceContouring::on_openCloudPointsFile_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, QString("读取点云数据"), QString(), QString("点云数据(*.csv)"));
    QDir dir(path);
    ui->filePathCloudPoint->setText(dir.dirName());
    QList<QList<double> > data = ReadCsvAsDouble(path);
    if (data.isEmpty()) {
        return;
    }
//    rawLine.clear();                                                          //注释掉后，可实现点云的叠加并运算
    for(auto list : data) {
        if (list.size() == 3) {
            rawLine.append(QVector3D(list[0], list[1], list[2]));
        }
    }
    parts->removeChildren(0, parts->getNumChildren());
    osg::ref_ptr<Part> part = new Part();
    parts->addChild(part.get());
    if (rawLine.isEmpty()) {
        return;
    }
    part->CreatePartNodeFromCloud(rawLine);

    ui->sample->setEnabled(false);
    ui->numInLine->setEnabled(false);
    ui->radioCloud->setEnabled(true);
    ui->radioCloud->setChecked(true);
    ui->radioFeed->setEnabled(true);
    ui->finishConstruction->setEnabled(true);
    ui->firstCloudMode->setEnabled(false);
    //添加参考坐标系
    AddIndicator();
}
