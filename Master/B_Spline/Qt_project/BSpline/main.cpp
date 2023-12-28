#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include "BSplineCurve.h"
#include "BSplineSurface.h"
#include "PathPlanning.h"
#include "MyMath.h"
#include "arsCore/TriangulatedSurface.h"
#include "arsCore/QtExtend.h"
#include <QTime>
#include <QDialog>

#include <opencv2/opencv.hpp>
typedef cv::Mat_<qreal> Matrix;


#include <QTime>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

/* 导入STL文件 */
        QString pathTopSTL = QFileDialog::getOpenFileName(NULL, "open top surface STL file", QString(), "STL file(*.stl)");     /* 载入STL文件 */
//        QString pathTopSTL("F:/work/B_Spline/MatlabWorkspace/dataInput/blade_right_top.STL");
        TriangulatedSurface topSurSTL(pathTopSTL);

/* STL曲面切分,构造插值点 */
        qreal step = 5;                                                             /* 切分间隔 */
        QVector3D dir = QVector3D(1, 0, 0);                                         /* 切分前进方向 */
        QVector3D direction = QVector3D(0, -1, 0);
        QVector<QVector<QVector3D> > lines = topSurSTL.IntersectPlaneTriangle(step, dir); /* 对STL文件进行细分，返回细分线 */
        int numOfSubdivide = 15;                                                /* 每条线细分点个数 */
        int count = 0;
        int sizes[3] = { numOfSubdivide, lines.count(), 3};
        Matrix interpolationPoints(3, sizes);                                   /* interpolation 每一列对应一条切线，是期望的等v线 */
        for (int index1 = 0; index1 < lines.count(); index1++) {
            TriangulatedSurface::SortLine(lines[index1], direction);            /* 排序 */
            Matrix pointsCut(lines[index1].count(), 3);
            for (int index = 0; index < lines.at(index1).count(); index++) {
                pointsCut[index][0] = lines[index1][index][0];
                pointsCut[index][1] = lines[index1][index][1];
                pointsCut[index][2] = lines[index1][index][2];
            }
            Matrix points = BSplineCurve::CurveSubdivide(pointsCut, numOfSubdivide);  /* 对切分线进行细分 */
            count++;
            qDebug("Processing %d / %d", count, lines.count());                     /* 显示计算进度 */
//            qDebug("points length: %d", points.rows);
            for (int index = 0; index < sizes[0]; index++) {
                interpolationPoints(index, index1, 0) = points(index, 0);
                interpolationPoints(index, index1, 1) = points(index, 1);
                interpolationPoints(index, index1, 2) = points(index, 2);
//                qDebug("point%d (%F, %F, %F)",
//                       index,
//                       interpolationPoints(index1, index, 0),
//                       interpolationPoints(index1, index, 1),
//                       interpolationPoints(index1, index, 2));
            }

        }
        qDebug("finish to readSTL");
        qDebug("the point of the (u,v) = (0,0) in interpolation is (%F, %F, %F)",
                interpolationPoints(0, 0, 0),
                interpolationPoints(0, 0, 1),
                interpolationPoints(0, 0, 2));
        qDebug("the point of the (u,v) = (1,0) in interpolation is (%F, %F, %F)",
               interpolationPoints(numOfSubdivide - 1, 0, 0),
               interpolationPoints(numOfSubdivide - 1, 0, 1),
               interpolationPoints(numOfSubdivide - 1, 0, 2));
        qDebug("the point of the (u,v) = (0,1) in interpolation is (%F, %F, %F)",
               interpolationPoints(0, lines.count() - 1, 0),
               interpolationPoints(0, lines.count() - 1, 1),
               interpolationPoints(0, lines.count() - 1, 2));
        qDebug("the point of the (u,v) = (1,1) in interpolation is (%F, %F, %F)",
                interpolationPoints(numOfSubdivide - 1, lines.count() - 1, 0),
                interpolationPoints(numOfSubdivide - 1, lines.count() - 1, 1),
                interpolationPoints(numOfSubdivide - 1, lines.count() - 1, 2));


/* 曲面拟合 */

    qDebug() << "start suface interpolation";
    BSplineSurface surface = BSplineSurface::FromInterpolation(3, 3, interpolationPoints);
    qDebug() << "the point of the (u,v) = (0,0) on surface is " << surface.PointOnSurface(0, 0);
    qDebug() << "the point of the (u,v) = (1,0) on surface is " << surface.PointOnSurface(1, 0);
    qDebug() << "the point of the (u,v) = (0,1) on surface is " << surface.PointOnSurface(0, 1);
    qDebug() << "the point of the (u,v) = (1,1) on surface is " << surface.PointOnSurface(1, 1);
/* 路径规划 */
    qreal feedInterval = 2;
    qreal scanInterval = 2;
    PathPlanning pathPlanning(surface);
    QVector<QVector<QVector3D> > path = pathPlanning.CalculatePath(feedInterval, scanInterval, PathPlanning::isoParametric);
    QVector<QVector<QVector3D> > pathNorm = pathPlanning.GetPathDirection();


    qDebug() << "计算完成";
    //QString pathOut = QFileDialog::getSaveFileName(NULL, "save intersection lines from STL", QString(), "Comma Seperated Values (*.csv)");
    QString pathX = QString("pathX.csv");
    QString pathY = QString("pathY.csv");
    QString pathZ = QString("pathZ.csv");
    QString thickness = QString("thickness.csv");
    QString num("num.csv");
    QFile csvPathX(pathX);
    QFile csvPathY(pathY);
    QFile csvPathZ(pathZ);
    QFile csvThickness(thickness);
    QFile csvNum(num);
    if (!csvPathX.open(QIODevice::WriteOnly)) {
        return 0;
    }
    if (!csvPathY.open(QIODevice::WriteOnly)) {
        return 0;
    }
    if (!csvPathZ.open(QIODevice::WriteOnly)) {
        return 0;
    }
    if (!csvThickness.open(QIODevice::WriteOnly)) {
        return 0;
    }
    if (!csvNum.open(QIODevice::WriteOnly)) {
        return 0;
    }
    QTextStream sX(&csvPathX);
    QTextStream sY(&csvPathY);
    QTextStream sZ(&csvPathZ);
    QTextStream sNum(&csvNum);

//    输出结果到csv
    for (int index = 0; index < path.count(); index++) {
        count = 0;
        for (int index1 = 0; index1 < path.at(index).count(); index1++) {
            sX << path.at(index).at(index1).x() << ",";
            sY << path.at(index).at(index1).y() << ",";
            sZ << path.at(index).at(index1).z() << ",";
        }
        sX << endl;
        sY << endl;
        sZ << endl;
        sNum << path.at(index).count() << endl;
    }

    qDebug() <<"保存完成";

    return 0;
}
