#include "Motion/MotionPath.h"
#include "InverseSolver.h"
#include "PathPlanning.h"
#include "qmath.h"
#include <QDebug>
#include <QFile>
#include <QtConcurrent>

/*!
 * \brief UniquePointCloud  对点云进行排序并去重
 */
QVector<QVector3D> UniquePointLine(QVector<QVector3D> pointCloud)
{
    QVector<QVector3D> newPointCloud(1, pointCloud.first());
    for (QVector3D p : pointCloud) {
        if (newPointCloud.last().distanceToPoint(p) >= 0.1) {
            newPointCloud.append(p);
        }
    }
    return newPointCloud;
}

int FindNearestPoint(QVector3D point, QVector<QVector3D> pointCloud)
{
    int pos = 0;
    qreal dL = 100;
    for(int i = 0; i <= pointCloud.size(); ++i) {
        if (dL > pointCloud[i].distanceToPoint(point)) {
            dL = pointCloud[i].distanceToPoint(point);
            pos = i;
        }
    }
    return pos;
}

PathPlanning::PathPlanning()
{
}

PathPlanning::PathPlanning(BSplineSurface sur)
{
    surface = sur;
}

/*!
 * \brief PathPlanning::GetPath             获得规划路径，即扫描点序列
 * \return
 */
QVector<QVector<QVector3D> > PathPlanning::GetPath(unsigned int startIgnore, unsigned int endIgnore)
{
    QVector<QVector<QVector3D> > newPath;
    if (path.isEmpty()) {
        return newPath;
    }
    for (uint i = startIgnore; i < path.size() - endIgnore; ++i) {
        newPath.append(path[i]);
    }
    return newPath;
}

MotionPath PathPlanning::GenerateMotionPath(qreal waterPath, qreal speed, unsigned int startIgnore, unsigned int endIgnore)
{
    MotionPath mp;                                                              //一个扫描计划
    QList<int> axex;
    axex << 0 << 1 << 2 << 3 << 4;
    MotionAxisList axisList(axex);
    for (uint lineIndex = startIgnore; lineIndex < path.size() - endIgnore; ++lineIndex) {
        MotionElement ele;                                                      //一条扫描线
        ele.type = MotionElement::Cam;
        ele.setAxisList(axisList);
//        qDebug() << "Kinematics New line" << lineIndex;
        for (int posIndex = 0; posIndex < path[lineIndex].size(); ++posIndex) {
            QVector3D direction = pathDirection[lineIndex][posIndex];
            if (direction.z() < 0) {
                direction = -direction;
            }
            QVector<qreal> jointPos = InverseSolver::Inverse(path[lineIndex][posIndex], direction, waterPath);
//            qDebug() << "point No. " << posIndex << "point: " << path[lineIndex][posIndex]
//                     << "norm: " << pathDirection[lineIndex][posIndex]
//                     << ", pose: " << jointPos;
            MotionPose motionPose;                                              //一条扫描线上的点
            for (int index = 0; index < 5; ++index) {
                if (index == 2 || index == 3) {                                   //z,b轴为反向
                    motionPose.append(-jointPos[index]);
                } else {
                    motionPose.append(jointPos[index]);
                }
            }
            ele.pos.append(motionPose);
        }
        ele.speed = speed;
        ele.stepSpeed = speed;
        ele.stepInterval = 1;
        mp.elements.append(ele);
    }
    mp.interval = 1;
    return mp;
}
/*!
 * \brief PathPlanning::GetPathNormVector   获得规划路径对应的扫描点声束方向
 * \return
 */
QVector<QVector<QVector3D> > PathPlanning::GetPathDirection(unsigned int startIgnore, unsigned int endIgnore)
{
    QVector<QVector<QVector3D> > direction;
    if (pathDirection.isEmpty()) {
        return direction;
    }
    for (uint i = startIgnore; i < pathDirection.size() - endIgnore; ++i) {
        direction.append(pathDirection[i]);
    }
    return direction;
}
/*!
 * \brief PathPlanning::SetSurface
                        设置曲面
 */
void PathPlanning::SetSurface(BSplineSurface sur)
{
    surface = sur;
}
/*!
 * \brief PathPlanning::CalculatePath
                        计算并返回扫描计划
 */
MotionPath PathPlanning::CalculatePath(qreal feedInterval,
                                        qreal scanInterval,
                                        PathPlanning::FeedType feedType,
                                        qreal incidence,
                                        qreal azimuth,
                                        Qt::Edge firstLine,
                                        PathPlanning::ScanDirection dir, bool surNormType,
                                       qreal waterPath, qreal speed,
                                       unsigned int startIgnore, unsigned int endIgnore,
                                       QVector3D feedDirection, QVector3D sortDirection)
{
    switch (feedType){
    case IsoArc:
        qDebug() << "Into IsoArc";
        PathPlanningByIsoArc(feedInterval, scanInterval, incidence, azimuth, firstLine, dir, surNormType);
        return GenerateMotionPath(waterPath, speed, startIgnore, endIgnore);
    case IsoParametric:
        qDebug() << "Into IsoParametric";
        PathPlanningByIsoParametric(feedInterval, scanInterval, incidence, azimuth, firstLine, dir, surNormType);
        return GenerateMotionPath(waterPath, speed, startIgnore, endIgnore);
    case IsoPlane:
        qDebug() << "Into IsoPlane";
        PathPlanningByIsoPlane(feedInterval, scanInterval, incidence, azimuth, feedDirection, sortDirection, dir, surNormType);
        return GenerateMotionPath(waterPath, speed, startIgnore, endIgnore);
    default:
        break;
    }
    return MotionPath();
}
/*!
 * \brief PathPlanning::CreateInterpolationsByStl
 *                      读取STL文件，返回一个插值点矩阵，不可直接返回B样条曲面，因为切分得到的第一条线和最后一条线可能存在较大的扭曲
 *                      配合SlipMatrix()使用，删除不想要的数据点
 * \param path          stl文件路径
 * \param direction     切分进给方向
 * \param step          切分步进间隔
 * \param num           每条切分线的插值点数
 * \return
 */
PathPlanning::Matrix PathPlanning::CreateInterpolationsByStl(
        const QString &path, qreal step, QVector3D directionStep,
        QVector3D directionSort, int numInLine)
{
    TriangulatedSurface surSTL(path);
    return CreateInterpolationsByTriangles(surSTL.GetTriangles(), step, directionStep, directionSort, numInLine);
}

PathPlanning::Matrix PathPlanning::CreateInterpolationsByTriangles(QList<Triangle> tris, qreal step, QVector3D directionStep, QVector3D directionSort, int numInLine)
{
    TriangulatedSurface surSTL(tris);
    QVector<QPair<QVector<QVector3D>, QVector<QVector3D> > > cutLines = surSTL.IntersectPlaneTriangle(step, directionStep);
    if (cutLines.isEmpty()) {
        return Matrix();
    }
    int sizes[3] = {numInLine, cutLines.count(), 3};
    Matrix interpolationPoints(3, sizes);                                   /*! interpolation 每一列对应一条切线，是期望的等v线 */
    QList<int> indexes;
    for (int lineIndex = 0; lineIndex < cutLines.count(); lineIndex++) {
        indexes << lineIndex;
    }

    auto interpolate = [&] (int lineIndex) {
        QVector<QVector3D> line = cutLines[lineIndex].first;
        TriangulatedSurface::SortLine(line, directionSort);            /* 排序 */
        line = UniquePointLine(line);
        Matrix pointsCut(line.count(), 3);
        for (int index = 0; index < line.count(); index++) {
            pointsCut[index][0] = line[index][0];
            pointsCut[index][1] = line[index][1];
            pointsCut[index][2] = line[index][2];
        }
        Matrix points = BSplineCurve::CurveSubdivide(pointsCut, numInLine);  /* 对切分线进行细分 */
        qDebug("Cut STL surface %d / %d", lineIndex + 1, cutLines.count());                     /* 显示计算进度 */
        for (int index = 0; index < numInLine; index++) {
            interpolationPoints(index, lineIndex, 0) = points(index, 0);
            interpolationPoints(index, lineIndex, 1) = points(index, 1);
            interpolationPoints(index, lineIndex, 2) = points(index, 2);
        }
        return true;
    };
    typedef std::function<bool(int)> MapFunc;
    QFuture<void> result = QtConcurrent::mapped(indexes, MapFunc(interpolate));
    result.waitForFinished();
    return interpolationPoints;
}

PathPlanning::Matrix PathPlanning::CreateInterpolationsByCsv(const QString &path)
{
    QFile fileCsv(path);
    if (!fileCsv.open(QIODevice::ReadOnly)) {
        return Matrix();
    }
    QTextStream csv(&fileCsv);
    QString line = csv.readLine();
    int numInLine = line.toInt();                                               //第一条第一个元素表示每条扫描线点的个数
    QVector<QVector<QVector3D> > samplePoints;
    while (1) {
        QVector<QVector3D> linePoints;
        for (int index = 0; index < numInLine; ++index) {
            if (csv.atEnd()) {
                break;
            }
            line = csv.readLine();
            QStringList stringList = line.split(",");
            QVector3D point;
            for(int i = 0; i < 3; ++i) {
                point[i] = stringList.value(i).toDouble();                      //QVector::value(int)可保证不越界
            }
            linePoints.append(point);
        }
        if (csv.atEnd()) {
            break;
        }
        samplePoints.append(linePoints);                                        /* 将append放在.atEnd判断后可以避免出现最后一条线少几个点的情况，
                                                                                    如果少几个点，则忽略这条线 */
    }
    int sizes[] = {numInLine, samplePoints.size(), 3};
    Matrix interpolationPoints(3, sizes);                                   /*! interpolation 每一列对应一条线 */
    for (int row = 0; row < sizes[1]; ++row) {
        for (int col = 0; col < sizes[0]; ++col) {
            QVector3D point = samplePoints[row][col];
            for (int dim = 0; dim < 3; ++dim) {
                interpolationPoints(col, row, dim) = point[dim];
            }
        }
    }
    return interpolationPoints;
}
/*!
 * \brief PathPlanning::ClipMatrix      插值点截取，删掉前几行和最后几行，配合CreateInterpolationsByStl()使用
 * \param numFirst                      需要删掉的前几行的行数
 * \param numLast                       需要删掉的后几行的行数
 * \return
 */
PathPlanning::Matrix PathPlanning::ClipInterpolationMatrix(Matrix interpolation, int numFirst, int numLast)
{
    int size[3] = {interpolation.size[0], interpolation.size[1] - numFirst - numLast, 3};
    if (size[1] <= 3) {                                    //删除线过多，留下的线少于3条无法进行曲面拟合
        qDebug() << "Clip matrix too much";
        return Matrix();
    }
    Matrix newInterpolation(3, size);
    for (int row = 0; row < size[0]; ++row){
        for (int col = 0; col <size[1]; ++col) {
            for (int dim = 0; dim < 3; ++dim) {
                newInterpolation(row, col, dim) = interpolation(row, col  + numFirst, dim);
            }
        }
    }
    return newInterpolation;
}

/*!
 * \brief PathPlanning::PathPlanningByIsoArc
                        等弧长进给路径规划
 */
void PathPlanning::PathPlanningByIsoArc(qreal feedInterval,
                                        qreal scanInterval,
                                        qreal incidence,
                                        qreal azimuth,
                                        Qt::Edge firstLine,
                                        PathPlanning::ScanDirection dir,
                                        bool normType)
{
    incidence = qDegreesToRadians(incidence);
    azimuth = qDegreesToRadians(azimuth);

    /*计算参数曲面沿V方向的最大曲线长度Lvmax，决定沿V方向的细分个数n = Lvmax / JogInterval + 1*/
    int numRef = surface.GetCtrlPoints().size[1];                                                     //沿v方向的曲线的条数，用来计算寻找最大长度Lvmax
    qreal Lmax = 0;
    for (int i = 0; i <= numRef; i++) {
        const qreal u = 1.0 * i / numRef;
        qreal dL = 0;   //临时弧长
        qreal v = 0;
        QVector3D pointPrevious, pointCurrent;
        if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
            pointPrevious = surface.PointOnSurface(v, u);
        } else {
            pointPrevious = surface.PointOnSurface(u, v);
        }
        //计算一条v方向曲线的长度
        while (v < 1) {
            v = v + 0.001;
            if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
                pointCurrent = surface.PointOnSurface(v, u);
            } else {
                pointCurrent = surface.PointOnSurface(u, v);
            }
            dL += pointCurrent.distanceToPoint(pointPrevious);
            pointPrevious = pointCurrent;
        }
        Lmax = Lmax > dL ? Lmax : dL;                                                        //取最大曲线长度
    }
    const int numOfScanLine = int(Lmax / feedInterval);                         //扫描线条数为numOfScanLine + 1
    qreal dFeed = 1.0 / ( rate * numOfScanLine);                                  // 步进过程的参数步长

    /*计算参数曲面沿u方向的最大曲线长度Lumax，设计每条扫描线的扫描点个数 */
    numRef = surface.GetCtrlPoints().size[0];
    Lmax = 0;
    for (int i = 0; i <= numRef; i++) {
        const qreal v = 1.0 * i / numRef;
        qreal dL = 0;   //临时弧长
        qreal u = 0;
        QVector3D pointPrevious, pointCurrent;
        if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
            pointPrevious = surface.PointOnSurface(v, u);
        } else {
            pointPrevious = surface.PointOnSurface(u,v);
        }
        //计算一条u方向曲线的长度
        while (u < 1) {
            u = u + 0.001;
            if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
                pointCurrent = surface.PointOnSurface(v, u);
            } else {
                pointCurrent = surface.PointOnSurface(u, v);
            }
            dL += pointPrevious.distanceToPoint(pointCurrent);
            pointPrevious = pointCurrent;
        }
        Lmax = Lmax > dL ? Lmax : dL;                                           //取最大曲线长度
    }
    const int numOfScanPoints = int(Lmax / scanInterval);                       //每条扫描线的点的个数为numOfScanPoints + 1
    qreal dScan = 1.0 / ( rate * numOfScanPoints);                              //计算细分弧长时的参数步长
    qreal roughScanInterval = Lmax / 10;                                        //10个粗扫点
    qreal dRoughScan = 1.0 / (10 * rate);                                               //计算粗分弧长时的参数步长

    /* 以下开始进行路径规划，沿u方向扫描，v方向进给 */
    /* 设计第一条扫描线 */
    QVector<QVector<qreal> > roughScanLine(2, QVector<qreal>(21, 0.0));         /* 粗扫描点，一行表示u,一行表示v，根据第一条扫描线的位置决定哪一行是u */
    for (int i = 0; i <= 20; i++) {          //第一条扫描线为v = 0
        if (firstLine == Qt::BottomEdge || firstLine == Qt::LeftEdge) {                         /* 从v=0或u=0开始作为第一条扫描线 */
            roughScanLine[0][i] = (dir == Forward) ? (i / 20.0) : (1 - i / 20.0);
            roughScanLine[1][i] = 0.0;
        } else {                                                                /* 从v=1或u=1开始作为第一条扫描线 */
            roughScanLine[0][i] = (dir == Forward) ? (i / 20.0) : (1 - i / 20.0);
            roughScanLine[1][i] = 1.0;
        }
    }

    roughScanLine = CurveSubdivide(roughScanLine, roughScanInterval, dRoughScan);           /* 对第一条扫描线进行粗分，确定进给参考点 */

    path.clear();
    pathDirection.clear();
    int ScanLineNo = 0; //测试用数据，用来记录扫描线
    /*不断进给，计算各扫描线*/
    while (1) {
        /*设置退出规划完成的条件*/
        //找到scanLine中v的最小值vmin
        if (firstLine == Qt::BottomEdge || firstLine == Qt::LeftEdge){                          /* 进给直到v>=1或者u>=1 */
            qreal vmin = roughScanLine[1][0];
            for (int i = 1; i < roughScanLine[0].size(); i++) {
                vmin = qMin(vmin, roughScanLine[1][i]);
            }
            if (vmin >= 1) {     //当粗扫描线的v的最小值不小于1时，说明扫描覆盖了所有曲面，退出规划
                 break;
            }
        } else if (firstLine == Qt::TopEdge || firstLine == Qt::RightEdge) {                    /* 进给直到v<=0或者u<=0 */
            qreal vmax = roughScanLine[1][0];
            for (int i = 1; i < roughScanLine[0].size(); i++) {
                vmax = qMax(vmax, roughScanLine[1][i]);
            }
            if (vmax <= 0) {     //当粗扫描线的v的最大值不大于0时，说明扫描覆盖了所有曲面，退出规划
                 break;
            }
        }
        /*开始进行规划 */
        //扫描细分
        QVector<QVector3D> lineTemp;                                                        //临时存放一条扫描线(x, y, z)
        QVector<QVector3D> normTemp;                                                        //临时存放一条扫描线的法向量
        QVector<QVector3D> directionTemp;                                                   //临时存放一条扫描线的声束指向
        QVector<QVector<qreal> > scanLine = CurveSubdivide(roughScanLine, scanInterval, dScan);    /*!< 对当前扫描线(u, v)进行细分，得到输出扫描线scanLine：(u, v)的点序列*/
        for (int i = 0; i <= (scanLine[0].size() - 1); i++) {
            QVector3D point;
            if (firstLine == Qt::BottomEdge || firstLine == Qt::TopEdge) {
                point = surface.PointOnSurface(scanLine[0][i], scanLine[1][i]);
                normTemp.append(normType ? surface.NormVector(scanLine[0][i], scanLine[1][i]) : -surface.NormVector(scanLine[0][i], scanLine[1][i]));                //将点的法向量保存到扫描线的法向量列表中
            } else {
                point = surface.PointOnSurface(scanLine[1][i], scanLine[0][i]);
                normTemp.append(normType ? surface.NormVector(scanLine[1][i], scanLine[0][i]) : -surface.NormVector(scanLine[1][i], scanLine[0][i]));                //将点的法向量保存到扫描线的法向量列表中
            }
            lineTemp.append(point);                                                         //将点坐标保存到扫描线的点列表中
            if (i >= 1) {
                QVector3D Ltemp = point - lineTemp[i - 1];                                  //切向量：上一个扫描点到下一个扫描点的向量，采用的差分法计算切向量
                Ltemp.normalize();                                                          //将切向量归一化
                const QVector3D Mtemp = QVector3D::crossProduct(Ltemp, normTemp[i - 1]);    //切向量与法向量的公法向量，这三个向量及对点扫描点构成随动坐标系
                QVector3D direction = qSin(incidence) * (qSin(azimuth) * Mtemp + qCos(azimuth) * Ltemp)
                                 + qCos(incidence) * normTemp[i - 1];                       /*!< 采用入射角和方向角确定声束在随动坐标系中的方向*/
                directionTemp.append(direction);                                            //将扫描点的声束方向添加到扫描线的声束方向列表中
            }
        }
        lineTemp.removeLast();                                                              //移除扫描线中最后一个扫描点，因为该扫描点无法利用差分计算切向量
        path.append(lineTemp);                                                              //将一条新的扫描线添加到路径中
        pathDirection.append(directionTemp);                                                //将一条新的扫描线的声束方向添加到路径声束方向中
        //扫描进给
        roughScanLine = feed(roughScanLine, feedInterval, firstLine, dFeed);                                                /*!< 扫描线进给*/
        ScanLineNo++;
        qDebug() << "第" << ScanLineNo << "条线点数为：" << lineTemp.size();
    }
    ScanLineNo++;
    /*最后一条扫描线，即所有点的v=0*/
    QVector<QVector3D> lineTemp;                                                            //临时存放一条扫描线(x, y, z)
    QVector<QVector3D> normTemp;                                                            //临时存放一条扫描线的法向量
    QVector<QVector3D> directionTemp;                                                       //临时存放一条扫描线的声束指向
    QVector<QVector<qreal> > scanLine = CurveSubdivide(roughScanLine, scanInterval, dScan);        //对当前扫描线(u, v)进行细分，得到输出扫描线scanLine：(u, v)的点序列
    for (int i = 0; i <= (scanLine[0].size() - 1); i++) {
        QVector3D point;
        if (firstLine == Qt::BottomEdge || firstLine == Qt::TopEdge) {
            point = surface.PointOnSurface(scanLine[0][i], scanLine[1][i]);
            normTemp.append(normType ? surface.NormVector(scanLine[0][i], scanLine[1][i]) : -surface.NormVector(scanLine[0][i], scanLine[1][i]));                //将点的法向量保存到扫描线的法向量列表中
        } else {
            point = surface.PointOnSurface(scanLine[1][i], scanLine[0][i]);
            normTemp.append(normType ? surface.NormVector(scanLine[1][i], scanLine[0][i]) : -surface.NormVector(scanLine[1][i], scanLine[0][i]));                //将点的法向量保存到扫描线的法向量列表中
        }
        lineTemp.append(point);                                                             //将点坐标保存到扫描线的点列表中
        if (i >= 1) {
            QVector3D Ltemp = point - lineTemp[i - 1];                                      //切向量：上一个扫描点到下一个扫描点的向量，采用的差分法计算切向量
            Ltemp.normalize();                                                              //将切向量归一化
            const QVector3D Mtemp = QVector3D::crossProduct(Ltemp, normTemp[i - 1]);        //切向量与法向量的公法向量，这三个向量及对点扫描点构成随动坐标系
            QVector3D direction = qSin(incidence) * (qSin(azimuth) * Mtemp + qCos(azimuth) * Ltemp)
                             + qCos(incidence) * normTemp[i - 1];                           //采用入射角和方向角确定声束在随动坐标系中的方向
            directionTemp.append(direction);                                            //将扫描点的声束方向添加到扫描线的声束方向列表中
        }
    }
    lineTemp.removeLast();                                                              //移除扫描线中最后一个扫描点，因为该扫描点无法利用差分计算切向量
    path.append(lineTemp);                                                              //将一条新的扫描线添加到路径中
    pathDirection.append(directionTemp);                                                //将一条新的扫描线的声束方向添加到路径声束方向中
    qDebug() << "第" << ScanLineNo << "条线点数为：" << lineTemp.size();
}
/*!
 * \brief PathPlanning::PathPlanningByIsoParametric
                        等参数进给路径规划
 */
void PathPlanning::PathPlanningByIsoParametric(qreal feedInterval,
                                               qreal scanInterval,
                                               qreal incidence,
                                               qreal azimuth,
                                               Qt::Edge firstLine,
                                               PathPlanning::ScanDirection dir,
                                               bool normType)
{
    incidence = qDegreesToRadians(incidence);
    azimuth = qDegreesToRadians(azimuth);

    /*计算参数曲面沿V方向的最大曲线长度Lvmax，决定沿V方向的细分个数n = Lvmax / JogInterval + 1*/
    int numRef = surface.GetCtrlPoints().size[1];                                                     //沿v方向的曲线的条数，用来计算寻找最大长度Lvmax
    qreal Lmax = 0;
    for (int i = 0; i <= numRef; i++) {
        const qreal u = 1.0 * i / numRef;
        qreal dL = 0;   //临时弧长
        qreal v = 0;
        QVector3D pointPrevious, pointCurrent;
        if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
            pointPrevious = surface.PointOnSurface(v, u);
        } else {
            pointPrevious = surface.PointOnSurface(u, v);
        }
        //计算一条v方向曲线的长度
        while (v < 1) {
            v = v + 0.001;
            if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
                pointCurrent = surface.PointOnSurface(v, u);
            } else {
                pointCurrent = surface.PointOnSurface(u, v);
            }
            dL += pointCurrent.distanceToPoint(pointPrevious);
            pointPrevious = pointCurrent;
        }
        Lmax = Lmax > dL ? Lmax : dL;                                                        //取最大曲线长度
    }
    const int numOfScanLine = int(Lmax / feedInterval);                        //扫描线条数为numOfScanLine + 1

    /*计算参数曲面沿u方向的最大曲线长度Lumax，设计每条扫描线的扫描点个数 */
    numRef = surface.GetCtrlPoints().size[0];
    Lmax = 0;
    for (int i = 0; i <= numRef; i++) {
        const qreal v = 1.0 * i / numRef;
        qreal dL = 0;   //临时弧长
        qreal u = 0;
        QVector3D pointPrevious, pointCurrent;
        if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
            pointPrevious = surface.PointOnSurface(v, u);
        } else {
            pointPrevious = surface.PointOnSurface(u,v);
        }
        //计算一条u方向曲线的长度
        while (u < 1) {
            u = u + 0.001;
            if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
                pointCurrent = surface.PointOnSurface(v, u);
            } else {
                pointCurrent = surface.PointOnSurface(u, v);
            }
            dL += pointPrevious.distanceToPoint(pointCurrent);
            pointPrevious = pointCurrent;
        }
        Lmax = Lmax > dL ? Lmax : dL;                                           //取最大曲线长度
    }
    const int numOfScanPoints = int(Lmax / scanInterval);                       //每条扫描线的点的个数为numOfScanPoints + 1
    qreal duv = 1.0 / ( rate * numOfScanPoints);                                //计算弧长时的参数步长

    //沿v方向进给，计算各扫描线的扫描点，按等弧长细分
    path.clear();
    pathDirection.clear();
    for (int i = 0; i <= numOfScanLine; i++) {
        QVector<QVector3D> lineTemp(numOfScanPoints + 1);                                            //临时存放一条扫描线(x, y, z)
        QVector<QVector3D> normTemp(numOfScanPoints + 1);                                            //临时存放一条扫描线的法向量
        QVector<QVector3D> directionTemp(numOfScanPoints + 1);                                       //临时存放一条扫描线的声束方向

        /*对第一条扫描线进行等弧长细分*/
        qreal u = (dir == Forward ? 0 : 1);                                     //确定扫描线起点
        const qreal v = ((firstLine == Qt::BottomEdge || firstLine == Qt::LeftEdge) ?
                        (1.0 * i / numOfScanLine) : (1 - 1.0 * i / numOfScanLine));  //确定进给方向                             //固定v，沿u方向扫描

        /*计算扫描线总弧长 */
        QVector3D pointPrevious, pointCurrent;
        if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
            pointPrevious = surface.PointOnSurface(v, u);
        } else {
            pointPrevious = surface.PointOnSurface(u,v);
        }
        //计算一条v方向曲线的长度
        qreal dL = 0;   //临时弧长
        qreal u0 = 0;
        while (u0 < 1) {
            u0 += duv;
            if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
                pointCurrent = surface.PointOnSurface(v, u0);
            } else {
                pointCurrent = surface.PointOnSurface(u0, v);
            }
            dL += pointPrevious.distanceToPoint(pointCurrent);
            pointPrevious = pointCurrent;
        }
        qreal interval = dL / numOfScanPoints;                                //相邻两个扫描点之间的间隔

        /* 计算扫描点 */
        dL = 0;
        int count = 1;                                                          //临时存放一条扫描线点的个数，随着细分进行，逐渐递增

        //第一个点
        if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
            lineTemp[count - 1] = surface.PointOnSurface(v, u);
            normTemp[count - 1] = normType ? surface.NormVector(v, u) : -surface.NormVector(v, u);
        } else {
            lineTemp[count - 1] = surface.PointOnSurface(u, v);
            normTemp[count - 1] = normType ? surface.NormVector(u, v) : -surface.NormVector(u, v);
        }
        QVector3D Ltemp;
        QVector3D Mtemp;//切向量与法向量的公法向量，这三个向量构成随动坐标系
        QVector3D direction;             /*!< 采用入射角和方向角确定声束在随动坐标系中的方向*/
        count++;

        while (u <= 1 && u >= 0 && count <= numOfScanPoints) {
            QVector3D pointPrevious, pointCurrent;
            if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
                pointPrevious = surface.PointOnSurface(v, u);
            } else {
                pointPrevious = surface.PointOnSurface(u, v);
            }
            while (dL <= interval * (count - 1) && u <= 1 && u >= 0) {                        //固定v，沿u方向计算曲线弧长，每dL的弧长，记录一个点
                u += duv * (dir == Forward ? 1 : -1);
                if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
                    pointCurrent = surface.PointOnSurface(v, u);
                } else {
                    pointCurrent = surface.PointOnSurface(u, v);
                }
                dL += pointCurrent.distanceToPoint(pointPrevious);
                pointPrevious = pointCurrent;
            }
            const QVector3D point = pointPrevious;                              //记录下扫描点
            lineTemp[count - 1] = point;                                        //将扫描点添加到扫描点列表中
            if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
                normTemp[count - 1] = normType ? surface.NormVector(v, u) : -surface.NormVector(v, u);                 //将法向量添加到扫描点法向量列表中
            } else {
                normTemp[count - 1] = normType ? surface.NormVector(u, v) : -surface.NormVector(u, v);                 //将法向量添加到扫描点法向量列表中
            }
            if (count >= 2) {
                Ltemp = lineTemp[count - 1] - lineTemp[count - 2];                        //切向量，上一个扫描点指向下一个扫描点的向量
                Ltemp.normalize();                                                              //将切向量归一化
                Mtemp = QVector3D::crossProduct(Ltemp, normTemp[count - 1]);//切向量与法向量的公法向量，这三个向量构成随动坐标系
                direction = qSin(incidence) * (qSin(azimuth) * Mtemp + qCos(azimuth) * Ltemp)
                                 + qCos(incidence) * normTemp[count - 1];             /*!< 采用入射角和方向角确定声束在随动坐标系中的方向*/
                directionTemp[count - 2] = direction;                                                //将声束方向添加到扫描线声束方向列表中
            }
            count++;                                                                  //扫描线上的扫描点个数加1
        }
        //最后一个点
        u = (dir == Forward ? 1 : 0);
        if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
            lineTemp[count - 1] = surface.PointOnSurface(v, u);
            normTemp[count - 1] = normType ? surface.NormVector(v, u) : -surface.NormVector(v, u);
        } else {
            lineTemp[count - 1] = surface.PointOnSurface(u, v);
            normTemp[count - 1] = normType ? surface.NormVector(u, v) : -surface.NormVector(u, v);
        }
        Ltemp = lineTemp[count - 1] - lineTemp[count - 2];                        //切向量，上一个扫描点指向下一个扫描点的向量
        Ltemp.normalize();                                                              //将切向量归一化
        Mtemp = QVector3D::crossProduct(Ltemp, normTemp[count - 1]);//切向量与法向量的公法向量，这三个向量构成随动坐标系
        direction = qSin(incidence) * (qSin(azimuth) * Mtemp + qCos(azimuth) * Ltemp)
                    + qCos(incidence) * normTemp[count - 1];             /*!< 采用入射角和方向角确定声束在随动坐标系中的方向*/
        directionTemp[count - 2] = direction;                                                //将声束方向添加到扫描线声束方向列表中
        directionTemp[count - 1] = direction;                                                //将声束方向添加到扫描线声束方向列表中

        /* 添加路径及方向 */
        path.append(lineTemp);                                                                  //将生成的单条扫描线添加到路径中
        pathDirection.append(directionTemp);                                                    //将生成的单条扫描线的声束方向添加到路径声束方向中
        qDebug() << "第" << i + 1 << "/" << numOfScanLine << "条线点数为：" << count + 1;
    }
}

void PathPlanning::PathPlanningByIsoPlane(qreal feedInterval, qreal scanInterval,
                                          qreal incidence, qreal azimuth,
                                          QVector3D feedDirection, QVector3D sortDirection,
                                          PathPlanning::ScanDirection dir, bool normType)
{
    incidence = qDegreesToRadians(incidence);
    azimuth = qDegreesToRadians(azimuth);
    TriangulatedSurface surSTL(triangles);
    if (triangles.isEmpty()) {
        qDebug() << "Pathplanning: triangles are empty";
    }
//    feedDirection = QVector3D(1, 0, 0);
//    sortDirection = QVector3D(0, 1, 0);
    QVector<QPair<QVector<QVector3D>, QVector<QVector3D> > > cutLines = surSTL.IntersectPlaneTriangle(feedInterval, feedDirection);
    if (cutLines.isEmpty()) {
        qDebug() << "Pathplanning: Cut lines is empty";
        return;
    }
    path.clear();
    pathDirection.clear();
    QList<int> indexes;
    for (int lineIndex = 0; lineIndex < cutLines.count(); lineIndex++) {
        indexes << lineIndex;
    }

    auto interpolate = [&] (int lineIndex) {
        QVector<QVector3D> line = cutLines[lineIndex].first;
        TriangulatedSurface::SortLine(line, sortDirection);                      /* 排序 */
        line = UniquePointLine(line);
        BSplineCurve::Matrix lineMatrix(line.size(), 3);
        for (int i = 0; i < line.size(); ++i) {
            lineMatrix(i, 0) = line[i].x();
            lineMatrix(i, 1) = line[i].y();
            lineMatrix(i, 2) = line[i].z();
        }
        BSplineCurve curve = BSplineCurve::FromInterpolation(3, lineMatrix);
        qreal u = 0;
        QVector3D pointPrevious = curve.PointOnCurve(u);
        qreal curveLength = 0;
        while (u <= 1 && u >= 0) {
            u += 0.001;
            QVector3D pointCurrent = curve.PointOnCurve(u);
            curveLength += pointCurrent.distanceToPoint(pointPrevious);
            pointPrevious = pointCurrent;
        }
        int numInLine = int(curveLength / scanInterval) + 1;
        qreal du = 1.0 / (numInLine * 20);

        if (dir == PathPlanning::Backward) {                                    //处理扫描线正反
            u = 1;
        } else {
            u = 0;
        }
        int count = 0;
        pointPrevious = curve.PointOnCurve(u);
        qreal dL = 0;
        QVector<QVector3D> pathLine;
        QVector<QVector3D> pathLineNorm;

        while(u <=1 && u >= 0) {
            while(dL < scanInterval * count && u <= 1 && u >= 0) {
                if (dir == PathPlanning::Backward) {                                    //处理扫描线正反
                    u -= du;
                } else {
                    u += du;
                }
                QVector3D pointCurrent = curve.PointOnCurve(u);
                dL += pointCurrent.distanceToPoint(pointPrevious);
                pointPrevious = pointCurrent;
            }
            pathLine.append(pointPrevious);
            const int pos = FindNearestPoint(pointPrevious, line);
            if (count >= 1) {
                QVector3D Ntemp = cutLines[lineIndex].second.value(pos).normalized();
                QVector3D Ltemp = pathLine[count] - pathLine[count - 1];
                Ltemp.normalize();
                QVector3D Mtemp = QVector3D::crossProduct(Ltemp, Ntemp);
                QVector3D direction = qSin(incidence) * (qSin(azimuth) * Mtemp + qCos(azimuth) * Ltemp)
                        + qCos(incidence) * Ntemp;             /*!< 采用入射角和方向角确定声束在随动坐标系中的方向*/
                if (normType == false) {                                            //处理法向量的正反
                    pathLineNorm.append(-direction);
                } else {
                    pathLineNorm.append(direction);
                }
            }
            count++;
        }
        pathLineNorm.append(pathLineNorm.last());                               //补充最后一个点的法向量
        qDebug() << "Pathplanning by IsoPlane: " << lineIndex << "/" << indexes.size();
        return QPair<QVector<QVector3D>, QVector<QVector3D> >(pathLine, pathLineNorm);
    };

    typedef std::function<QPair<QVector<QVector3D>, QVector<QVector3D> >(int)> MapFunc;
    QFuture<QPair<QVector<QVector3D>, QVector<QVector3D> > > result = QtConcurrent::mapped(indexes, MapFunc(interpolate));
    result.waitForFinished();
    for (QPair<QVector<QVector3D>, QVector<QVector3D> > t : result.results().toVector()) {
        path.append(t.first);
        pathDirection.append(t.second);
    }
}

/*!
 * \brief PathPlanning::feed                给定一条扫描点序列，预测下一条扫描点序列
 * \param pointsList                        一个点序列(u, v)，需要借助曲面将其转换成坐标值
 * \return                                  返回下一条扫描点序列(u, v)
 */
QVector<QVector<qreal> > PathPlanning::feed(QVector<QVector<qreal> > pointsList, qreal feed, Qt::Edge edge, qreal step)
{
    QVector<QVector<qreal> > nextList(2, QVector<qreal>(pointsList[0].size(), 0.0));    //声明下一条扫描线, 与当前扫描线点数相同
    /*对当前扫描线上的点沿(u, v) = (0, 1)方向进给feedInterval */
    for (int i = 0; i < pointsList[0].size(); i++) {                                    //共pointsList[0].size()个点，分别进行沿v方向的进给
        qreal utemp = pointsList[0][i];                                                 //临时变量，用来对弧长进行积分
        qreal vtemp = pointsList[1][i];
        qreal dl = 0.0;
        qreal dv;// du;
        if ( edge == Qt::BottomEdge || edge == Qt::LeftEdge){                         /* 确定进给的方向 */
//            du = 0.0;                                                         /* (du, dv)即进给的方向上的步长 */
            dv = step;                                                        /* 即沿正方向进给 */
        } else {
//            du = 0.0;
            dv = -step;
        }
        QVector3D pointPrevious, pointCurrent;
        if (edge == Qt::BottomEdge || edge == Qt::TopEdge) {
            pointPrevious = surface.PointOnSurface(utemp, vtemp);
        } else {
            pointPrevious = surface.PointOnSurface(vtemp, utemp);
        }
        while (dl < feed && vtemp <= 1 && vtemp >= 0) {
            vtemp += dv;
            if (edge == Qt::BottomEdge || edge == Qt::TopEdge) {
                pointCurrent = surface.PointOnSurface(utemp, vtemp);
            } else {
                pointCurrent = surface.PointOnSurface(vtemp, utemp);
            }
            dl += pointCurrent.distanceToPoint(pointPrevious);
            pointPrevious = pointCurrent;
        }
        nextList[0][i] = utemp;                                                         //将进给后的点保存下来
        nextList[1][i] = vtemp;
    }
    nextList[0][0] = pointsList[0][0];                                                  //修正起点参数u，防止进给过程中出现收缩
    nextList[0][pointsList[0].size() - 1] = pointsList[0][pointsList[0].size() - 1];

    return nextList;
}

/*!
 * \brief PathPlanning::CurveSubdivide      给定一条点序列，对其进行等弧长细分，细分间隔为L
 * \param pointsList                        一个点序列(u, v)，需要借助曲面将其转换成坐标值
 * \param interval                          细分弧长间隔
 * \return                                  返回细分后的点序列(u, v)
 */
QVector<QVector<qreal> > PathPlanning::CurveSubdivide(QVector<QVector<qreal> > pointsList, qreal interval, qreal step)
{
    /*对（u, v)做曲线拟合*/
    Matrix uvCurve(pointsList[0].size(), 2);
    for (int i = 0; i < pointsList[0].size(); i++) {
        uvCurve(i, 0) = pointsList[0][i];
        uvCurve(i, 1) = pointsList[1][i];
    }
    int p = 2;
    BSplineCurve curve = BSplineCurve::FromInterpolation(p, uvCurve);    //将(u, v)拟合成一条p次曲线curve

    /* 对曲线进行细分, 细分间隔为dt */

    QVector<QVector<qreal> > uvOut(2, QVector<qreal>());    //声明返回值变量

    qreal t = 0.0;
    qreal dt = step;
    qreal utemp = curve.PointOnCurve(t, 0);
    qreal vtemp = curve.PointOnCurve(t, 1);
    QVector3D pointPrevious = surface.PointOnSurface(utemp, vtemp);
    /*计算弧长，进行细分*/
    while (t < 1) {
        qreal dl = 0.0;
        utemp = qBound<qreal>(0, utemp, 1);
        vtemp = qBound<qreal>(0, vtemp, 1);
        uvOut[0].append(utemp);                                                     //保存u值
        uvOut[1].append(vtemp);                                                     //保存v值
        while (dl < interval && t < 1) {                                            //计算每一段弧长interval
            t += dt;
            t = t > 1 ? 1 : t;
            utemp = curve.PointOnCurve(t, 0);
            vtemp = curve.PointOnCurve(t, 1);
            const QVector3D pointCurrent = surface.PointOnSurface(utemp, vtemp);
            dl += pointCurrent.distanceToPoint(pointPrevious);
            pointPrevious = pointCurrent;
        }
    }
    /*最后一个点, 不能保证最后一个点与倒数第二个点间隔为interval*/
    t = 1.0;                                                                        //补上最后一个点t = 1
    utemp = curve.PointOnCurve(t, 0);
    vtemp = curve.PointOnCurve(t, 1);
    uvOut[0].append(utemp);                                                         //保存u值，最后一个点
    uvOut[1].append(vtemp);                                                         //保存v值

    return uvOut;
}

