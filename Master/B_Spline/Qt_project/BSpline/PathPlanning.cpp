#include "PathPlanning.h"
#include "qmath.h"
#include <qdebug.h>

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
QVector<QVector<QVector3D> > PathPlanning::GetPath()
{
    return path;
}
/*!
 * \brief PathPlanning::GetPathNormVector   获得规划路径对应的扫描点声束方向
 * \return
 */
QVector<QVector<QVector3D> > PathPlanning::GetPathDirection()
{
    return pathDirection;
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
                        计算并返回规划路径
 */
QVector<QVector<QVector3D> > PathPlanning::CalculatePath(qreal feedinterval,
                                                         qreal scanInterval,
                                                         PathPlanning::FeedType feedType,
                                                         qreal incidence,
                                                         qreal azimuth,
                                                         Qt::Edge firstLine,
                                                         PathPlanning::ScanDirection dir)
{
    switch (feedType){
    case isoArc:
        PathPlanningByIsoArc(feedinterval, scanInterval, incidence, azimuth, firstLine, dir);
        return GetPath();
    case isoParametric:
        PathPlanningByIsoParametric(feedinterval, scanInterval, incidence, azimuth, firstLine, dir);
        return GetPath();
    default:
        break;
    }
    return QVector<QVector<QVector3D> >();
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
                                        PathPlanning::ScanDirection dir)
{
    qreal roughScanInterval = 10 * scanInterval;
    /*计算参数曲面沿V方向的最大曲线长度Lvmax，估计扫描线个数 */
    int numTemp = surface.GetCtrlPoints().size[0];                             //沿v方向的曲线的条数，用来计算寻找最大长度Lvmax
    qreal Lmax = 0;
    for (int i = 0; i <= numTemp; i++) {
        const qreal u = 1.0 * i / numTemp;
        qreal dL = 0;   //临时弧长
        qreal v = 0;
        QVector3D pointPrevious, pointCurrent;
        if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
            pointPrevious = surface.PointOnSurface(v, u);
        } else {
            pointPrevious = surface.PointOnSurface(u,v);
        }
        //计算一条v方向曲线的长度
        while (v < 1) {
            v = v + 0.0001;
            if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
                pointCurrent = surface.PointOnSurface(v, u);
            } else {
                pointCurrent = surface.PointOnSurface(u, v);
            }
            dL += pointPrevious.distanceToPoint(pointCurrent);
            pointPrevious = pointCurrent;
        }
        Lmax = Lmax > dL ? Lmax : dL;                                                        //取最大曲线长度
    }
    const int numOfScanLine = int(Lmax / feedInterval);                        //扫描线条数为numOfScanLine + 1



    /* 以下开始进行路径规划，沿u方向扫描，v方向进给 */
    /* 设计第一条扫描线 */
    QVector<QVector<qreal> > roughScanLine(2, QVector<qreal>(21, 0.0));         /* 粗扫描点，一行表示u,一行表示v，根据第一条扫描线的位置决定哪一行是u */
    for (int i = 0; i <= 20; i++) {          //第一条扫描线为v = 0
        if (firstLine == Qt::BottomEdge || firstLine == Qt::LeftEdge) {                         /* 从v=0或u=0开始作为第一条扫描线 */
            roughScanLine[0][i] = (dir == forward) ? (i / 20.0) : (1 - i / 20.0);
            roughScanLine[1][i] = 0.0;
        } else {                                                                /* 从v=1或u=1开始作为第一条扫描线 */
            roughScanLine[0][i] = (dir == forward) ? (i / 20.0) : (1 - i / 20.0);
            roughScanLine[1][i] = 1.0;
        }
    }

    roughScanLine = CurveSubdivide(roughScanLine, roughScanInterval);           /* 对第一条扫描线进行粗分，确定进给参考点 */

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
        QVector<QVector<qreal> > scanLine = CurveSubdivide(roughScanLine, scanInterval);    /*!< 对当前扫描线(u, v)进行细分，得到输出扫描线scanLine：(u, v)的点序列*/
        for (int i = 0; i <= (scanLine[0].size() - 1); i++) {
            QVector3D point;
            if (firstLine == Qt::BottomEdge || firstLine == Qt::TopEdge) {
                point = surface.PointOnSurface(scanLine[0][i], scanLine[1][i]);
            } else {
                point = surface.PointOnSurface(scanLine[1][i], scanLine[0][i]);
            }
            lineTemp.append(point);                                                         //将点坐标保存到扫描线的点列表中
            normTemp.append(surface.NormVector(scanLine[0][i], scanLine[1][i]));            //将点的法向量保存到扫描线的法向量列表中
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
        roughScanLine = feed(roughScanLine, feedInterval, firstLine);                                                /*!< 扫描线进给*/
        ScanLineNo++;
//        emit PathPlanningProcessing(numOfScanLine + 1, ScanLineNo);
        qDebug() << "第" << ScanLineNo << "条线点数为：" << lineTemp.size();
    }
    ScanLineNo++;
    /*最后一条扫描线，即所有点的v=0*/
    QVector<QVector3D> lineTemp;                                                            //临时存放一条扫描线(x, y, z)
    QVector<QVector3D> normTemp;                                                            //临时存放一条扫描线的法向量
    QVector<QVector3D> directionTemp;                                                       //临时存放一条扫描线的声束指向
    QVector<QVector<qreal> > scanLine = CurveSubdivide(roughScanLine, scanInterval);        //对当前扫描线(u, v)进行细分，得到输出扫描线scanLine：(u, v)的点序列
    for (int i = 0; i <= (scanLine[0].size() - 1); i++) {
        const QVector3D point = surface.PointOnSurface(scanLine[0][i], scanLine[1][i]);
        lineTemp.append(point);                                                             //将点坐标保存到扫描线的点列表中
        normTemp.append(surface.NormVector(scanLine[0][i], scanLine[1][i]));                //将点的法向量保存到扫描线的法向量列表中
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
                                               PathPlanning::ScanDirection dir)
{
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
    qreal duv = 1.0 / ( 15.0 * numOfScanPoints);                                  //计算弧长时的参数步长

    //沿v方向进给，计算各扫描线的扫描点，按等弧长细分
    path.clear();
    pathDirection.clear();
    for (int i = 0; i <= numOfScanLine; i++) {
        QVector<QVector3D> lineTemp(numOfScanPoints + 1);                                            //临时存放一条扫描线(x, y, z)
        QVector<QVector3D> normTemp(numOfScanPoints + 1);                                            //临时存放一条扫描线的法向量
        QVector<QVector3D> directionTemp(numOfScanPoints + 1);                                       //临时存放一条扫描线的声束方向

        /*对第一条扫描线进行等弧长细分*/
        qreal u = (dir == forward ? 0 : 1);                                     //确定扫描线起点
        const qreal v = ((firstLine == Qt::BottomEdge || firstLine == Qt::LeftEdge) ? (1.0 * i / numOfScanLine) : (1 - 1.0 * i / numOfScanLine));  //确定进给方向                             //固定v，沿u方向扫描

        /*计算扫描线总弧长 */
        QVector3D pointPrevious, pointCurrent;
        if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
            pointPrevious = surface.PointOnSurface(v, u);
        } else {
            pointPrevious = surface.PointOnSurface(u,v);
        }
        //计算一条v方向曲线的长度
        qreal dL = 0;   //临时弧长
        while (u < 1) {
            u += duv * (dir == forward ? 1 : -1);                        //扫描线方向
            if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
                pointCurrent = surface.PointOnSurface(v, u);
            } else {
                pointCurrent = surface.PointOnSurface(u, v);
            }
            dL += pointPrevious.distanceToPoint(pointCurrent);
            pointPrevious = pointCurrent;
        }
        Lmax = Lmax > dL ? Lmax : dL;                                           //取最大曲线长度
        qreal interval = Lmax / numOfScanPoints;                                //相邻两个扫描点之间的间隔

        /* 计算扫描点 */
        dL = 0;
        int count = 1;                                                          //临时存放一条扫描线点的个数，随着细分进行，逐渐递增
        while (u <= 1 && u >= 0 && count <= numOfScanPoints) {
            QVector3D pointPrevious, pointCurrent;
            if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
                pointPrevious = surface.PointOnSurface(v, u);
            } else {
                pointPrevious = surface.PointOnSurface(u, v);
            }
            while (dL <= interval * count && u <= 1 && u >= 0) {                        //固定v，沿u方向计算曲线弧长，每dL的弧长，记录一个点
                u += duv * (dir == forward ? 1 : -1);
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
                normTemp[count - 1] = surface.NormVector(v, u);                 //将法向量添加到扫描点法向量列表中
            } else {
                normTemp[count - 1] = surface.NormVector(v, u);                 //将法向量添加到扫描点法向量列表中
            }
            if (count >= 2) {
                QVector3D Ltemp = lineTemp[count - 1] - lineTemp[count - 2];                        //切向量，上一个扫描点指向下一个扫描点的向量
                Ltemp.normalize();                                                              //将切向量归一化
                const QVector3D Mtemp = QVector3D::crossProduct(Ltemp, normTemp[count - 1]);//切向量与法向量的公法向量，这三个向量构成随动坐标系
                const QVector3D direction = qSin(incidence) * (qSin(azimuth) * Mtemp + qCos(azimuth) * Ltemp)
                                 + qCos(incidence) * normTemp[count - 1];             /*!< 采用入射角和方向角确定声束在随动坐标系中的方向*/
                directionTemp[count - 1] = direction;                                                //将声束方向添加到扫描线声束方向列表中
            }
            count++;                                                                  //扫描线上的扫描点个数加1
        }
        //最后一个点
        u = (dir == forward ? 1 : 0);
        if (firstLine == Qt::LeftEdge || firstLine == Qt::RightEdge) {
            lineTemp[count - 1] = surface.PointOnSurface(v, u);
            normTemp[count - 1] = surface.NormVector(v, u);
        } else {
            lineTemp[count - 1] = surface.PointOnSurface(u, v);
            normTemp[count - 1] = surface.NormVector(u, v);
        }
        QVector3D Ltemp = lineTemp[count - 1] - lineTemp[count - 2];                        //切向量，上一个扫描点指向下一个扫描点的向量
        Ltemp.normalize();                                                              //将切向量归一化
        const QVector3D Mtemp = QVector3D::crossProduct(Ltemp, normTemp[count - 1]);//切向量与法向量的公法向量，这三个向量构成随动坐标系
        const QVector3D direction = qSin(incidence) * (qSin(azimuth) * Mtemp + qCos(azimuth) * Ltemp)
                         + qCos(incidence) * normTemp[count - 1];             /*!< 采用入射角和方向角确定声束在随动坐标系中的方向*/
        directionTemp[count - 1] = direction;                                                //将声束方向添加到扫描线声束方向列表中

        /* 添加路径及方向 */
        path.append(lineTemp);                                                                  //将生成的单条扫描线添加到路径中
        pathDirection.append(directionTemp);                                                    //将生成的单条扫描线的声束方向添加到路径声束方向中
//        emit PathPlanningProcessing(numOfScanLine + 1, i + 1);
        qDebug() << "第" << i + 1 << "/" << numOfScanLine << "条线点数为：" << count + 1;
    }
}
/*!
 * \brief PathPlanning::feed                给定一条扫描点序列，预测下一条扫描点序列
 * \param pointsList                        一个点序列(u, v)，需要借助曲面将其转换成坐标值
 * \return                                  返回下一条扫描点序列(u, v)
 */
QVector<QVector<qreal> > PathPlanning::feed(QVector<QVector<qreal> > pointsList, qreal feed, Qt::Edge edge)
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
            dv = 0.0001;                                                        /* 即沿正方向进给 */
        } else {
//            du = 0.0;
            dv = -0.0001;
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
QVector<QVector<qreal> > PathPlanning::CurveSubdivide(QVector<QVector<qreal> > pointsList, qreal interval)
{
    /*对（u, v)做曲线拟合*/
    //BSplineCurveInterpolation类只支持3维，因此uvCurve被修正成3D曲线，本质还是二维曲线
    Matrix uvCurve(pointsList[0].size(), 2);
    for (int i = 0; i < pointsList[0].size(); i++) {
        uvCurve(i, 0) = pointsList[0][i];
        uvCurve(i, 1) = pointsList[1][i];
//        uvCurve[i] = QVector3D(pointsList[0][i], pointsList[1][i], 0.0);
    }
    int p = 2;
    BSplineCurve curve = BSplineCurve::FromInterpolation(p, uvCurve);    //将(u, v)拟合成一条p次曲线curve

    /* 对曲线进行细分, 细分间隔为dt */

    QVector<QVector<qreal> > uvOut(2, QVector<qreal>());    //声明返回值变量

    qreal t = 0.0;
    qreal dt = 0.0002;
    qreal utemp = curve.PointOnCurve(t, 0);
    qreal vtemp = curve.PointOnCurve(t, 1);
    QVector3D pointPrevious = surface.PointOnSurface(utemp, vtemp);
    /*计算弧长，进行细分*/
    while (t < 1) {
        qreal dl = 0.0;
        utemp = utemp > 1 ? 1 : utemp;                                              //对utemp, vtemp进行控制，防止越界[0, 1]
        utemp = utemp < 0 ? 0 : utemp;
        vtemp = vtemp > 1 ? 1 : vtemp;
        vtemp = vtemp < 0 ? 0 : vtemp;
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

