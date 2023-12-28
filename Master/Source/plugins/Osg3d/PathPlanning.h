#pragma once

#include "BSplineCurve.h"
#include "BSplineSurface.h"
#include "ars3d/TriangulatedSurface.h"
#include <QObject>

class MotionPath;

/*!
 * \brief 对一个给定的B样条曲面，做路径规划
 *        路径规划的结果是给出TCP的轨迹，以csv的形式输出
 */
class PathPlanning : public QObject
{
public:
    typedef cv::Mat_<qreal> Matrix;
    enum ScanDirection {                                                         //每条线的扫描方向，沿u、v正方向为正
        Forward,
        Backward
    };
    enum FeedType {                                                              //进给方式
        IsoArc,
        IsoParametric,
        IsoPlane
    };
    enum State {
        StateGetInterpolationState,
        StateClipInterpolation,
        StateSurfaceInterpolation,
        StateSurfaceExtend,
        StateSurfaceResample,
        StateCalculatePath,
    };

    PathPlanning();
    PathPlanning(BSplineSurface sur);

    void SetSurface(BSplineSurface sur);
    void SetTriangles(QList<Triangle> tris) {triangles = tris;}
    MotionPath CalculatePath(qreal feedInterval,
                            qreal scanInterval,
                            FeedType feedType = IsoParametric,
                            qreal incidence = 0,
                            qreal azimuth = 0,
                            Qt::Edge firstLine = Qt::BottomEdge,
                            ScanDirection dir = Forward,
                            bool surNormType = true, qreal waterPath = 100, qreal speed = 10000,
                            unsigned int startIgnore = 0, unsigned int endIgnore = 0,
                            QVector3D feedDirection = QVector3D(1, 0, 0),
                            QVector3D sortDirection = QVector3D(0, 1, 0));
    static Matrix CreateInterpolationsByStl(const QString &path, qreal step, QVector3D directionStep,
                                            QVector3D directionSort, int numInLine);
    static Matrix CreateInterpolationsByTriangles(QList<Triangle> tris, qreal step, QVector3D directionStep,
                                                  QVector3D directionSort, int numInLine);
    static Matrix CreateInterpolationsByCsv(const QString &path);               //从CSV文件中获取插值点
    static Matrix CreateInterpolationsBySample(const QString &path);            //从仿形结果中创建插值点
    static Matrix ClipInterpolationMatrix(Matrix interpolation, int numFirst, int numLast);

    QVector<QVector<QVector3D> > GetPathDirection(unsigned int startIgnore = 0, unsigned int endIgnore = 0);
    QVector<QVector<QVector3D> > GetPath(unsigned int startIgnore = 0, unsigned int endIgnore = 0);

private:
    MotionPath GenerateMotionPath(qreal waterPath = 100, qreal speed = 10000, unsigned int startIgnore = 0, unsigned int endIgnore = 0);

    void PathPlanningByIsoArc(qreal feedInterval,                               //等弧长进给
                              qreal scanInterval,
                              qreal incidence = 0,
                              qreal azimuth = 0,
                              Qt::Edge firstLine = Qt::BottomEdge,
                              ScanDirection dir = Forward, bool normType = true);
    void PathPlanningByIsoParametric(qreal feedInterval,                        //等参数进给
                                     qreal scanInterval,
                                     qreal incidence = 0,
                                     qreal azimuth = 0,
                                     Qt::Edge firstLine = Qt::BottomEdge,
                                     ScanDirection dir = Forward, bool normType = true);
    void PathPlanningByIsoPlane(qreal feedInterval, qreal scanInterval,
                                qreal incidence = 0, qreal azimuth = 0,
                                QVector3D feedDirection = QVector3D(1, 0,0),
                                QVector3D sortDirection = QVector3D(0, 1, 0),
                                ScanDirection dir = Forward, bool normType = true);
    QVector<QVector<qreal> > feed(QVector<QVector<qreal> > pointsList, qreal feed, Qt::Edge edge, qreal step);
    QVector<QVector<qreal> > CurveSubdivide(QVector<QVector<qreal> > pointsList, qreal interval, qreal step);


private:
    BSplineSurface surface;
    QList<Triangle> triangles;
    QVector<QVector<QVector3D> > path;                                          /* 扫查路径：外层为扫描线索引，内层为扫描线对应的点索引 */
    QVector<QVector<QVector3D> > pathDirection;                                 /* 扫描路径的方向 */
    bool surfaceNormType;                                                       /* 曲面法向量方向，朝内还是朝外 */
    const qreal rate = 20.0;
};
