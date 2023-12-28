#ifndef PATHPLANNING_H
#define PATHPLANNING_H

#include "BSplineCurve.h"
#include "BSplineSurface.h"
#include <QObject>


/*!
 * \brief 对一个给定的B样条曲面，做路径规划
 *        路径规划的结果是给出TCP的轨迹，以csv的形式输出
 */
class PathPlanning : public QObject
{
public:
    typedef cv::Mat_<qreal> Matrix;
    enum ScanDirection{                                                         //每条线的扫描方向，沿u、v正方向为正
        forward,
        backward
    };
    enum FeedType{                                                              //进给方式
        isoArc,
        isoParametric
    };

    PathPlanning();
    PathPlanning(BSplineSurface sur);

    QVector<QVector<QVector3D> > GetPathDirection();
    void SetSurface(BSplineSurface sur);
    QVector<QVector<QVector3D> > CalculatePath(qreal feedinterval,
                                               qreal scanInterval,
                                               FeedType feedType = isoParametric,
                                               qreal incidence = 0,
                                               qreal azimuth = 0,
                                               Qt::Edge firstLine = Qt::BottomEdge,
                                               ScanDirection dir = forward);
private:
    QVector<QVector<QVector3D> > GetPath();

    void PathPlanningByIsoArc(qreal feedInterval,                               //等弧长进给
                              qreal scanInterval,
                              qreal incidence = 0,
                              qreal azimuth = 0,
                              Qt::Edge firstLine = Qt::BottomEdge,
                              ScanDirection dir = forward);
    void PathPlanningByIsoParametric(qreal feedInterval,                        //等参数进给
                                     qreal scanInterval,
                                     qreal incidence = 0,
                                     qreal azimuth = 0,
                                     Qt::Edge firstLine = Qt::BottomEdge,
                                     ScanDirection dir = forward);
    QVector<QVector<qreal> > feed(QVector<QVector<qreal> > pointsList, qreal feed, Qt::Edge edge);
    QVector<QVector<qreal> > CurveSubdivide(QVector<QVector<qreal> > pointsList, qreal interval);


private:
    BSplineSurface surface;
    QVector<QVector<QVector3D> > path;                                          /* 扫查路径：外层为扫描线索引，内层为扫描线对应的点索引 */
    QVector<QVector<QVector3D> > pathDirection;                                 /* 扫描路径的方向 */
};

#endif // PATHPLANNING_H
