#pragma once

#include "OriginIndicator.h"
#include "Part.h"
#include "PathPlanning.h"
#include "PointMt.h"
#include "SurfaceMt.h"
#include <Ultrasonic/UTData.h>
#include <arsCore/UTPose.h>
#include <Ultrasonic/UTData.h>
#include <osg/Group>
#include <osg/ref_ptr>
#include <QLabel>
#include <QWidget>
#include <QColor>
#include <QVector4D>

struct OsgSceneOptions
{
    QColor backgroundTop = QColor(224, 226, 237);
    QColor backgroundBottom = Qt::white;
    QColor lightDiffuse = Qt::white;
    QColor lightAmbient = Qt::white;
    QColor lightSpecular = Qt::white;
    QVector3D lightDirection = QVector3D(0.f, 0.f, -1.f);
    QVector4D lightPosition = QVector4D(0.f, 0.f, 0.f, 1.f);
};

namespace Ui {
class PanelMachine;
}
class Joint;
class Machine;

class PanelMachine : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(OsgSceneOptions sceneOptions READ sceneOptions WRITE setSceneOptions)

public:
    explicit PanelMachine(QWidget *parent = 0);
    ~PanelMachine();

    OsgSceneOptions sceneOptions() const;

public slots:
    void setSceneOptions(OsgSceneOptions option);

private slots:
    void UpdateAxisList();
    void OnPositionUpdated(const UTPose &pose);
    void onDataArrived(const UTDataFrame &frame);

    void OnDrawPath(const QVector<QVector3D> &path, QVector<QVector3D> direction = QVector<QVector3D>());                            //路径绘制与清除
    void on_actionZoomFit_triggered();                                          /* 窗口视角重置 */
    void on_actionImportModel_triggered();                                      /* 模型导入与消除 */
    void on_actionImportPart_triggered();                                       /* 工件的导入与清除 */
    void on_actionCScanImage_triggered();                                       /* C扫成像 */
    void on_actionAddOriginIndicator_triggered(bool checked);                   /* 添加坐标轴指示 */
    void on_actionSurfAutoReconstruction_triggered();                           /* 曲面自动重建 */
    void on_actionSamplePoint_triggered();
    void on_actionPathPlanning_triggered();
    void on_actionPartFromScan_triggered();

private:
    bool importMachine(const QString &path);

private:
    Ui::PanelMachine *ui;
//    osg::ref_ptr<osg::Group> root;                                              /* 维护OSG所有场景子节点 */
    osg::Group *root;
    osg::ref_ptr<Machine> machine;                                              /* 设备 */
    QList<Joint *> joints;                                                      /* 此joints列表为设备轴链对应的列表 */
    osg::ref_ptr<osg::Group> parts;                                             /* 所有工件 */
    osg::ref_ptr<osg::Group> points;                                            /* 保存路径规划、仿形测量、曲面拟合所有的点pointMt */
    osg::ref_ptr<osg::Node> pointGeo;                                           /* 点几何，读取spere.STL生成一个球形node，用于显示点 */
    osg::ref_ptr<OriginIndicator> originIndicator;
    osg::ref_ptr<SurfaceMt> surfaceMt;
    osg::ref_ptr<osg::Group> cScanImages;

    osg::ref_ptr<osg::Geode> backgroundGeode;
    osg::ref_ptr<PointMt> probeIndicator;
    OsgSceneOptions m_sceneOptions;
};
