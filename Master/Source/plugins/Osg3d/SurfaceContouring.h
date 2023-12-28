#pragma once

#include "BSplineSurface.h"
#include <Motion/MotionPath.h>
#include <QDialog>
#include <Ultrasonic/UTStreamFile.h>
#include "OriginIndicator.h"
#include "Part.h"
#include "PointMt.h"
#include <WaferScan/ScanPlan/ScanPlan.h>
#include <Acquisition/ScanController.h>

namespace Ui {
class SurfaceContouring;
}

/*!
 * \brief The SurfaceAutoReconstruction class   进行曲面自动化重建，由已知曲面预测接下来的曲面并进行扫描获得精确数据
 */
class SurfaceContouring : public QDialog
{
    Q_OBJECT

public:
    explicit SurfaceContouring(QWidget *parent = 0);
    ~SurfaceContouring();

public slots:
    void StartNextScanPlan(MotionPath mp);

private slots:
    void newDataArrived(const UTDataFrame &frame);
    void onStateChanged(ScanPlan::ScanStatus state);
    void onStateChangedForScanController(int state);

    void on_openSampleFile_clicked();
    void on_openCloudPointsFile_clicked();

    void on_sample_clicked();                                                   //采点
    void on_startFeed_clicked();                                                //开始步进一条线
    void on_resetFeed_clicked();                                                //重新步进一条线
    void on_finishConstruction_clicked();                                       //结束采样

    void on_radioFeed_clicked();
    void on_radioCloud_clicked();

    void on_firstCloudMode_clicked();                                           //基于采样点做面的路径规划，采样更多点
    void on_startCloudMode_clicked();                                           //基于点云做面的路径规划，采样更多点

private:
    void AddIndicator();

private:
    Ui::SurfaceContouring *ui;
    QVector<QVector3D> initeInterpolationPoints;                                //初始曲面的插值点
    QVector<QVector<QVector3D> > interpolationPoints;                           //曲面的插值点
    QVector<QVector3D> newLine;                                                 //扫查的下一条线
    QVector<QVector3D> rawLine;                                                 //从探伤仪获取的位置数据，数据量较大
    int sampleIndex = 0;
    bool isRunning = false;

    UTChannel *channel;
    osg::Group *root;
    osg::ref_ptr<osg::Group> pointMts;
    osg::ref_ptr<osg::Group> scanPath;
    osg::ref_ptr<osg::Group> surfaceMts;
    osg::ref_ptr<osg::Group> parts;
    osg::ref_ptr<OriginIndicator> indicator;

    osg::ref_ptr<PointMt> currentPointMt;
    QVector3D currentPoint;                                                     //焦斑的实时位置
};
