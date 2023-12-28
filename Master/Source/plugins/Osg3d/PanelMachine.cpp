#include <Motion/Axis.h>
#include <Motion/EndPoint.h>
#include "arsCore/Platform.h"
#include <Ultrasonic/UTPose.h>
#include "Motion/MotionPath.h"
#include <Motion/TrioDriver.h>
#include "CScanSurface.h"
#include "Machine.h"
#include <osg/Material>
#include "OriginIndicator.h"
#include "PanelMachine.h"
#include "PathPlanningWizard.h"
#include <Acquisition/ScanPlanModel.h>
#include "ui_PanelMachine.h"
#include <osg/Depth>
#include <osg/LightSource>
#include <osg/LineWidth>
#include <osg/Point>
#include <osgDB/ReadFile>
#include <osgText/Text>
#include <QFileDialog>
#include <QProgressDialog>
#include <QPushButton>
#include <Ultrasonic/UTChannel.h>
#include <arsUi/Dialog/DialogImageOptions.h>
#include "SurfaceContouring.h"
#include "qtosgUtils.h"
#include "OsgContainer.h"

using namespace osg;

PanelMachine::PanelMachine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelMachine)
{
    ui->setupUi(this);

    pointGeo = osgDB::readNodeFile("sphere.STL");
    root = ui->container->getRoot();

    parts = new Group;
    root->addChild(parts.get());

    cScanImages = new Group;
    root->addChild(cScanImages.get());

    points = new Group;
    root->addChild(points.get());

    setSceneOptions(OsgSceneOptions());

    QString machinePath = platform()->Get("MachineModel").toString();
    if (!machinePath.isEmpty()) {
        importMachine(machinePath);
    }
    connect(MotionManager::instance(), &MotionManager::axisListChanged, this, &PanelMachine::UpdateAxisList);
    connect(MotionManager::endPoint(0), &EndPoint::PositionUpdated, this, &PanelMachine::OnPositionUpdated);
    UpdateAxisList();

    probeIndicator = new PointMt(QVector3D(), NULL);

    StateSet *state = probeIndicator->getOrCreateStateSet();

    osg::Material *material = new osg::Material;
    material->setDiffuse(osg::Material::FRONT_AND_BACK, Vec4(1.0, 0.0, 0.0, 1.0));
    material->setAmbient(osg::Material::FRONT_AND_BACK, Vec4(1.0, 0.0, 0.0, 1.0));
    material->setTransparency(osg::Material::FRONT_AND_BACK, 0.5);                   //透明
    state->setAttributeAndModes(material, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);

    root->addChild(probeIndicator);
    connect(UltrasonicManager::channel(0), &UTChannel::MonitorDataArrived, this, &PanelMachine::onDataArrived);
}

PanelMachine::~PanelMachine()
{
    delete ui;
}

OsgSceneOptions PanelMachine::sceneOptions() const
{
    return m_sceneOptions;
}

void PanelMachine::setSceneOptions(OsgSceneOptions option)
{
    m_sceneOptions = option;
}

void PanelMachine::UpdateAxisList()
{
    if (!machine) {
        return;
    }
    qDebug() << "Updating axis list";
    joints.clear();
    EndPoint *endpoint = MotionManager::endPoint(0);
    if (!endpoint) {
        return;
    }
    for (auto *axis: endpoint->GetAxesPointer()) {
        joints.append(NULL);                                                    //joints中保留空指针，用来保证UTPose的正确顺序
        qDebug() << "joint name from EndPoint:" << axis->GetName();
        for (Joint *joint: machine->jointList) {
            if (axis->GetName() == QString::fromStdString(joint->getName())) {
                qDebug() << "joint name from Model: " <<QString::fromStdString(joint->getName());
                joints.last() = joint;
                connect(axis, SIGNAL(ForwardLimitUpdated(bool)), joint, SLOT(ForwardLimitUpdated(bool)));
                connect(axis, SIGNAL(ReverseLimitUpdated(bool)), joint, SLOT(ReverseLimitUpdated(bool)));
                break;
            }
        }
    }
    qDebug() << "Valid joint count: " << joints.size();
}

void PanelMachine::OnPositionUpdated(const UTPose &pose)
{
    for (int index = 0; index < pose.count(); ++index) {
        Joint *joint = joints.value(index);
        if (joint) {
            joint->setPos(pose[index]);
        }
    }
}

void PanelMachine::onDataArrived(const UTDataFrame &frame)
{
    if (frame.isNull()) {
        return;
    }
    auto pos = frame.UpdatePosition();
    probeIndicator->setPosition(pos.first);
}

/*!
 * \brief PanelMachine::OnDrawPath 接收路径规划结果中的一条线，绘制该路径
 *                                    在OSG坐标系下，即设备的逻辑坐标系
 * \param path
 */
void PanelMachine::OnDrawPath(const QVector<QVector3D> &path, QVector<QVector3D> direction)
{
    //定义路径几何点
    qDebug() << "Draw path";
    if (!pointGeo) {
        qDebug() << "Sphere point is NULL";
        return;
    }
    foreach (QVector3D p, path) {
        ref_ptr<PointMt> point = new PointMt(p, NULL);
        points->addChild(point.get());
    }

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
                                   + Vec3(direction[index].x(), direction[index].y(), direction[index].z()) * 20);
        }
        geome->addPrimitiveSet(new DrawArrays(PrimitiveSet::LINES, 0, vertexArray->size()));

        ref_ptr<Geode> node = new Geode;
        node->addDrawable(geome.get());
        points->addChild(node.get());
    }
}

void PanelMachine::on_actionZoomFit_triggered()
{
    ui->container->home();
}

/*!
 * \brief PanelMachine::on_actionImportModel_triggered
 *          导入模型文件
 */
void PanelMachine::on_actionImportModel_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, QString("导入模型文件"), QString(), QString("mprobo(*.mprobo)"));
    if (path.isEmpty()) {
        return;
    }
    if (importMachine(path)) {
        platform()->Set("MachineModel", path);
    }
}

/*! 导入工件 */
void PanelMachine::on_actionImportPart_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, QString("读取工件STL文件模型"), QString(), QString("STL文件或仿形数据(*.STL *.csv)"));
    ref_ptr<Part> part = new Part;
    if (!part->CreatePartNode(path)) {                                          //二进制和ASCII格式的STL文件均可
        qDebug() << "Failed importing part";
        return;
    }
    parts->addChild(part.get());
    on_actionZoomFit_triggered();
}

bool PanelMachine::importMachine(const QString &path)
{
    ref_ptr<Machine> mach = Machine::importXml(path);
    if (mach) {
        if (machine) {
            root->removeChild(machine);
        }
        machine = mach;
        root->addChild(mach);
    } else {
        qDebug() << "Fail to import model";
        return false;
    }
    mach->setName("machine");

    ui->progress->show();
    QFuture<void> progress = mach->loadProgress();
    auto *watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcherBase::progressRangeChanged, ui->progress, &QProgressBar::setRange);
    connect(watcher, &QFutureWatcherBase::progressValueChanged, ui->progress, &QProgressBar::setValue);
    connect(watcher, &QFutureWatcherBase::finished, [watcher, this]() {
        ui->progress->hide();
        ui->container->home();
        watcher->deleteLater();
    });
    watcher->setFuture(progress);

    /* 让OSG坐标与设备逻辑坐标系重合 */
    {
        QVector3D offset;
        foreach (Joint *j, mach->jointList) {
            if (j->motionType() == Joint::Linear){
                offset -= j->GetMoveBase() * j->GetAxisZ();
            }
            if (j->getName() == "B") {
                offset -= j->GetPointBase();
                break;
            }
        }
        mach->setTranslate(offset);
    }

    UpdateAxisList();
    on_actionZoomFit_triggered();                                               //调整视角到整个窗口
    return true;
}

void PanelMachine::on_actionCScanImage_triggered()
{
    QString stlFile = QFileDialog::getOpenFileName(this, QString("读取曲面STL文件模型"), QString(), QString("stl(*.STL)"));
//    ref_ptr<CScanSurface> cScanSurface = new CScanSurface(100, 100, surfaceMt->GetBSplineSurface(), QString(), stlFile);
//    cScanImages->addChild(cScanSurface.get());
}

void PanelMachine::on_actionAddOriginIndicator_triggered(bool checked)
{
    root->removeChild(originIndicator.get());
    if (checked) {
        const BoundingSphere bound = root->getBound();                              //根据root的包围球自动设置坐标轴的尺度
        originIndicator = new OriginIndicator("O", qMax<qreal>(2, bound.radius() / 5));
        root->addChild(originIndicator.get());
    }
}

void PanelMachine::on_actionSurfAutoReconstruction_triggered()
{
    //手动采点3条线构建初始曲面
    SurfaceContouring *dialog = new SurfaceContouring(this);
    connect(dialog, &SurfaceContouring::finished, dialog, &SurfaceContouring::deleteLater);
    dialog->show();
}

void PanelMachine::on_actionSamplePoint_triggered()
{
    UTChannel *channel = UltrasonicManager::channel(0);
    if (!channel) {
        return;
    }
    UTDataFrame frame = channel->lastFrame();
    if (frame.isNull()) {
        return;
    }
    auto position = frame.UpdatePosition();
    qDebug() << "Adding point at" << position;
    PointMt *point = new PointMt(position.first, pointGeo);
    points->addChild(point);
}

void PanelMachine::on_actionPathPlanning_triggered()
{
    PathPlanningWizard pathPlanning;
    pathPlanning.exec();
}

void PanelMachine::on_actionPartFromScan_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, QString(), QString(), tr("*.ard;;*.*"));
    if (path.isEmpty()) {
        return;
    }
    ref_ptr<Part> part = new Part;
    QVector<QVector3D> points;
    UTStreamFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    DialogImageOptions options(this);
    options.exec();
    ImageOptions option = options.GetOptions();

    QProgressDialog progress(this);
    progress.setMaximum(10000);
    progress.setMinimumDuration(0);
    progress.setWindowModality(Qt::ApplicationModal);
    while (!file.atEnd() && !progress.wasCanceled()) {
        UTDataSegment segment = file.ReadSegment(10240);
        for (const auto &frame: segment) {
            if (!option.isChannelEnabled(frame.Context.GetChannelID()) || !option.isDirectionEnabled(frame.Context.IsReverse(0))) {
                continue;
            }
            if (!frame.IsIGateTriggered()) {
                continue;
            }
            auto pos = frame.UpdatePosition();
            points << pos.first;
        }
        progress.setValue(qreal(file.pos()) / file.size() * 10000);
    }
    qDebug() << "Finished reading" << points.size() << "points";
    if (part->CreatePartNodeFromCloud(points)) {
        parts->addChild(part);
    }
}
