#pragma once

#include <QComboBox>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <Motion/MotionPath.h>
#include <osg/Group>
#include "OriginIndicator.h"
#include "PathPlanning.h"
#include "SurfaceMt.h"

namespace Ui {
class PathPlanningWizard;
}

class PathPlanningWizard : public QDialog
{
    Q_OBJECT

public:
    PathPlanningWizard(QList<Triangle> tris = QList<Triangle>(), QWidget *parent = 0);
    PathPlanningWizard(QVector<QVector<QVector3D> > interpolations, QWidget *parent = 0);
    ~PathPlanningWizard();
    MotionPath GetMotionPath() {return motionPath;}

    /* 参数更新 */
    void Update();

    /* 绘制路径 */
    void DrawPath(const QVector<QVector3D> &path, QVector<QVector3D> direction = QVector<QVector3D>());
    void DrawPath(const PathPlanning::Matrix &path);
    void DrawPath(const QVector<QVector<QVector3D> > &path, const QVector<QVector<QVector3D> > &direction);
    void DrawMyText(osg::Group *parent, const QString &text, const QVector3D &pos, qreal fontSize = 2);
    void ClearPath();
    void ClearSurfaceMts();

private slots:
    void on_previous_clicked();
    void on_next_clicked();
    void on_cancel_clicked();
    void on_update_clicked();
    void on_openFile_clicked();

    void on_fromSample_clicked();
    void on_fromCloudOrCAD_clicked();

    void on_getPathPlanningType_currentIndexChanged(const QString &value);

private:
    Ui::PathPlanningWizard *ui;

    //配置参数
    unsigned int resampleU;
    unsigned int resampleV;

    bool surfaceNormType;                                                       /*  */
    PathPlanning::FeedType pathPlanningType;                                    /*  */
    Qt::Edge pathPlanningPosition;                                              /*  */
    PathPlanning::ScanDirection scanDirection;                                  /*  */

    PathPlanning *pathPlanning;
    QList<Triangle> triangles;
    QVector<QVector3D> pointCloud;
    BSplineSurface::Matrix interpolationPoints;
    MotionPath motionPath;

    //osgContainer
    osg::ref_ptr<osg::Group> root = NULL;
    osg::ref_ptr<osg::Group> points;                                            //保存路径规划、仿形测量、曲面拟合所有的点pointMt
    osg::ref_ptr<osg::Node> pointGeo;                                           //点几何，读取spere.STL生成一个球形node，用于显示点
    osg::ref_ptr<osg::Group> surfaceMts;
    osg::ref_ptr<OriginIndicator> indicator;



};
