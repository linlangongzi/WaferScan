#include "BSplineSurface.h"
#include "MyMath.h"
#include <QDebug>

BSplineSurface::BSplineSurface()
{}

/*!
 * \brief BSplineSurface::BSplineSurface    构造一个b样条，含有节点矢量和控制点
 * \param orderU                            u方向基函数阶次
 * \param orderV                            v方向基函数阶次
 * \param knotVectorU                       u方向节点矢量
 * \param knotVectorV                       v方向节点矢量
 * \param controlPoints                       控制点
 */
BSplineSurface::BSplineSurface(int orderU, int orderV, QVector<qreal> knotVectorU, QVector<qreal> knotVectorV, Matrix controlPoints)
{
    bsplineU = BSpline(knotVectorU, orderU);
    bsplineV = BSpline(knotVectorV, orderV);
    if (controlPoints.size[2] = 3) {
        ctrlPoints = controlPoints;                                             // 点的维度必须是三维
    }
}

/*!
 * \brief BSplineSurface::BSplineSurface    b样条曲面的构造
 * \param u
 * \param v
 * \param controlPoints
 */
BSplineSurface::BSplineSurface(const BSpline &u, const BSpline &v, Matrix controlPoints)
{
    bsplineU = u;
    bsplineV = v;
    if (controlPoints.size[2] = 3) {
        ctrlPoints = controlPoints;
    }
}

/*!
 * \brief BSplineSurface::SurfacePoint      计算指定参数下的函数值
 * \param u                                 参数u
 * \param v                                 参数v
 * \return                                  返回指定参数下的函数值
 */
QVector3D BSplineSurface::PointOnSurface(qreal u, qreal v)
{
    u = qBound<qreal>(0, u, 1);
    v = qBound<qreal>(0, v, 1);
    int spanU = bsplineU.FindSpan(u);
    QVector<qreal> basisU = bsplineU.BasisFunc(u, spanU);           //计算u所在区间的各个基函数
    int spanV = bsplineV.FindSpan(v);
    QVector<qreal> basisV = bsplineV.BasisFunc(v, spanV);           //计算v所在区间的各个基函数

    int uind = spanU - bsplineU.order;                              //uind为u所在区间有效的控制点序列的起点索引号
    QVector3D temp;                     //临时保存u参数层面上的控制点的和，即sum(basis*Pctrl)
    QVector3D SP;

    for (int l = 0; l <= bsplineV.order; l++) {
        int vind = spanV - bsplineV.order + l;                      //vind为v所在区间有效的控制点序列的起点索引号
        for (int k = 0; k <= bsplineU.order; ++k) {
            for (int i = 0; i < 3; ++i) {
                temp[i] +=  basisU[k] * ctrlPoints(uind + k, vind, i);
            }
        }
        SP += basisV[l] * temp;
    }
    return SP;
}

/*!
 * \brief BSplineSurface::SurfaceDerivs     计算指定参数下的某坐标轴对应的偏导数
 * \param u                                 参数u
 * \param v                                 参数v
 * \param diffOrder                         偏导数阶次
 * \param axis                              指定的坐标轴
 * \return                                  返回偏导数，为( diffOrder + 1 ) * ( diffOrder + 1 ) 阶矩阵
 *                                              行为v偏导数，列为u偏导数
 */
cv::Mat_<qreal> BSplineSurface::SurfaceDerivative(qreal u, qreal v, int diffOrder, int axis)
{
    u = qBound<qreal>(0, u, 1);
    v = qBound<qreal>(0, v, 1);
    Matrix der = Matrix(diffOrder + 1, diffOrder + 1);
//    QVector<QVector<qreal> > der(diffOrder + 1, QVector<qreal>(diffOrder + 1, 0));                 //将其设置成( diffOrder + 1 ) * ( diffOrder + 1 )

    const int DiffOrderU = qMin(diffOrder, bsplineU.order);                                       //实际计算的导数阶次
    const int DiffOrderV = qMin(diffOrder, bsplineV.order);

    const int spanU = bsplineU.FindSpan(u);
    Matrix dersU = bsplineU.DerivativeBasis(u, spanU, diffOrder);         //计算u方向基函数的导数
    const int spanV = bsplineV.FindSpan(v);
    Matrix dersV = bsplineV.DerivativeBasis(v, spanV, diffOrder);         //计算v方向基函数的导数

    for (int k = 0; k <= DiffOrderU; k++) {
        QVector<qreal> temp(bsplineV.order + 1, 0);
        for (int s = 0; s <= bsplineV.order; s++) {
            temp[s] = 0;
            for (int r = 0; r <= bsplineU.order; r++) {
                temp[s] += dersU(k, r) * ctrlPoints(spanU - bsplineU.order + r, spanV - bsplineV.order + s, axis);
            }
        }
        int dd = qMin(diffOrder - k, DiffOrderV);
        for (int l = 0; l <= dd; l++) {
            der(k, l) = 0;
            for (int s = 0; s <= bsplineV.order; s++) {
                der(k, l) += dersV(l, s) * temp[s];
            }
        }
    }
    return der;
}
/*!
 * \brief BSplineSurface::NormVector        计算指定点的经过归一化后的法向量
 * \param u                                 参数u
 * \param v                                 参数v
 * \return
 */
QVector3D BSplineSurface::NormVector(qreal u, qreal v)
{

    const Matrix dx = SurfaceDerivative(u, v, 1, Qt::XAxis);        //计算x的1阶偏导数
    const Matrix dy = SurfaceDerivative(u, v, 1, Qt::YAxis);
    const Matrix dz = SurfaceDerivative(u, v, 1, Qt::ZAxis);
    const QVector3D du(dx(1, 0), dy(1, 0), dz(1, 0));
    const QVector3D dv(dx(0, 1), dy(0, 1), dz(0, 1));
    QVector3D norm = QVector3D::crossProduct(du, dv);
    return norm.normalized();
}

/*!
 * \brief BSplineSurface::FromInterpolation     不指定边界导矢的曲面拟合
 * \param orderU
 * \param orderV
 * \param points
 * \return
 */
BSplineSurface BSplineSurface::FromInterpolation(int orderU, int orderV, const Matrix &points)
{
    BSplineSurface surface;
    BSpline &bsplineU = surface.bsplineU;
    BSpline &bsplineV = surface.bsplineV;
    bsplineU.order = orderU;
    bsplineV.order = orderV;
    int lengthOfInterPointsInU = points.size[0] - 1;                            /* points的第一维：插值点在u方向的个数 */
//            points.size() - 1;                         //插值点在u方向的个数
    int lengthOfInterPointsInV = points.size[1] - 1;                            /* points的第二维：插值点在v方向的个数 */
    const int dimension = points.size[2] - 1;                                   /* points的第三维：点的维度，用通道数表示 */
    int sizes[3] = {lengthOfInterPointsInU + 1, lengthOfInterPointsInV + 1, dimension + 1};
    int lengthOfCtrlPointsInU = lengthOfInterPointsInU;                     //不指定边界导矢的情况下，控制点与插值点个数相同
    int lengthOfCtrlPointsInV = lengthOfInterPointsInV;

    /*以下计算knotsInterInU，即插值点对应的节点*/
    int numV = lengthOfInterPointsInV + 1;                                              //记录v方向有效的曲线条数
    QVector<qreal> knotsInterInU(lengthOfInterPointsInU + 1, 0);
    knotsInterInU[lengthOfInterPointsInU] = 1;
    for (int l = 0; l <= lengthOfInterPointsInV; l++) {                                  //固定v方向
        qreal distanceTotal = 0;
        QVector<qreal> distanceAll(lengthOfInterPointsInU, 0);                           //用来存放固定v方向后，在u方向上的各个弦长
        for (int k = 1; k <= lengthOfInterPointsInU; k++) {                              //计算各段弦长
            qreal distancePow = 0;
            for (int j = 0; j <= dimension; j++) {
                distancePow += qPow(points(k, l, j) - points(k - 1, l, j), 2);
            }
            distanceAll[k - 1] = qSqrt(distancePow);
            distanceTotal += distanceAll[k - 1];
        }
        if (distanceTotal == 0) {                                                       //若某一次定v方向没有长度，则总条数numV自减一次
            numV--;
        } else{
            qreal d = 0;
            for (int k = 1; k < lengthOfInterPointsInU; k++) {
                d += distanceAll[k - 1];
                knotsInterInU[k] += + d / distanceTotal;                                //后一个knotsInterInU[k]是上一轮计算的值
            }
        }
    }
    for (int k = 1; k < lengthOfInterPointsInU; k++) {                                  //knotsInterInU是经过了每一条定v线计算出来的和，因此要除以num
        knotsInterInU[k] = knotsInterInU[k] / numV;
    }

    /*以下计算knotsInterInV，即插值点对应的节点*/
    int numU = lengthOfInterPointsInU + 1;                                              //记录v方向有效的曲线条数
    QVector<qreal> knotsInterInV(lengthOfInterPointsInV + 1, 0);
    knotsInterInV[lengthOfInterPointsInV] = 1.0;
    for (int l = 0; l <= lengthOfInterPointsInU; l++) {                                 //固定u方向
        qreal distanceTotal = 0;
        QVector<qreal> distanceAll(lengthOfInterPointsInV, 0);                          //用来存放固定u方向后，在v方向上的各个弦长
        for (int k = 1; k <= lengthOfInterPointsInV; k++) {                             //计算各段弦长
            qreal distance_pow = 0;
            for (int j = 0; j <= dimension; j++) {
                distance_pow += qPow(points(l, k, j) - points(l, k - 1, j), 2);
            }
            distanceAll[k - 1] = qSqrt(distance_pow);
            distanceTotal += distanceAll[k - 1];
        }
        if (distanceTotal == 0) {                                                       //若某一次定u方向没有长度，则总次数numU自减一次
            numU--;
        } else {
            qreal d = 0;
            for (int k = 1; k < lengthOfInterPointsInV; k++) {
                d += distanceAll[k - 1];
                knotsInterInV[k] += d / distanceTotal;                                  //后一个knotsInterInV[k]是上一轮计算的值
            }
        }
    }
    for (int k = 1; k < lengthOfInterPointsInV; k++) {
        knotsInterInV[k] = knotsInterInV[k] / numU;                                     //knotsInterInU是经过了每一条定v线计算出来的和，因此要除以num
    }

    /*配置节点矢量knotsInU*/
    int lengthOfKnotsInU = lengthOfInterPointsInU + bsplineU.order + 1;                 //节点矢量的长度为lengthOfKnotsInU+1
    bsplineU.knots.resize(lengthOfKnotsInU + 1);
    for (int i = 0; i <= lengthOfKnotsInU; i++) {
        if (i <= bsplineU.order) {                                                      //前orderInU+1个节点是0
            bsplineU.knots[i] = 0;
        } else if (i >= lengthOfKnotsInU - bsplineU.order) {                            //后orderInU+1个节点是1
            bsplineU.knots[i] = 1.0;
        } else {
            qreal sum_temp = 0;
            for (int k = 1; k <= bsplineU.order; k++) {
                sum_temp += knotsInterInU[i - k];
            }
            bsplineU.knots[i] = 1.0 / bsplineU.order * sum_temp;
        }
    }

    /*配置节点矢量knotsInV*/
    int lengthOfKnotsInV = lengthOfInterPointsInV + bsplineV.order + 1;                //节点矢量的长度为lengthOfKnotsInV+1
    bsplineV.knots.resize(lengthOfKnotsInV + 1);
    for (int i = 0; i <= lengthOfKnotsInV; i++ ) {
        if (i <= bsplineV.order) {                                                     //前orderInV+1个节点是0
            bsplineV.knots[i] = 0;
        } else if (i >= lengthOfKnotsInV - bsplineV.order) {                           //后orderInV+1个节点是1
            bsplineV.knots[i] = 1.0;
        } else {
            qreal sum_temp = 0;
            for (int k = 1; k <= bsplineV.order; k++) {
                sum_temp += knotsInterInV[i - k];
            }
            bsplineV.knots[i] = 1.0 / bsplineV.order * sum_temp;
        }
    }

    /*InterPoints = Au * middlePoints, middlePoints = Av * ctrlPoint  */
    /*建立系数矩阵Au, 求解middlePoints  */
    QVector<QVector<qreal> > Au(lengthOfCtrlPointsInU + 1, QVector<qreal>(lengthOfCtrlPointsInU + 1, 0));
    for (int i = 0; i <= lengthOfCtrlPointsInU; i++) {
        int span = bsplineU.FindSpan(knotsInterInU[i]);
        QVector<qreal> basis = bsplineU.BasisFunc(knotsInterInU[i], span);
        for (int j = 0; j <= bsplineU.order; j++) {
            Au[i][j + (span - bsplineU.order)] = basis[j];
        }
    }
    /*利用高斯消元法求解中间控制点: 对各个Axis分别进行求解  */
    Matrix middlePoints = Matrix(3, sizes);
//    QVector<QVector<QVector3D > > middlePoints(lengthOfCtrlPointsInU + 1,       //中间控制点变量
//                                               QVector<QVector3D >(lengthOfCtrlPointsInV+1,
//                                               QVector3D(0, 0, 0)));
    surface.ctrlPoints = middlePoints;                                          //对控制点初始化，用来指定控制点的维度
    for (int i = 0; i <= lengthOfCtrlPointsInV; i++) {                          //提取各个v方向单独计算
        for (int dim = 0; dim <= dimension; dim++) {                            //提取点的各axis单独计算
            QVector<qreal> b(lengthOfCtrlPointsInU+1, 0);                        //将固定的v，固定点维度后的插值点数据提取到b
            for (int j = 0; j <= lengthOfCtrlPointsInU; j++) {
                b[j] = points(j, i, dim);
            }
            QVector<qreal> x = Gauss(Au, b);
            for (int j = 0; j <= lengthOfCtrlPointsInU; j++) {                  //将计算后的值返回给middlePoints
                middlePoints(j, i, dim) = x[j];
            }
        }
    }

    /*建立系数矩阵Av，求解ctrlPoints, middlePoints = Av * ctrlPoint  */
    QVector<QVector<qreal> > Av(lengthOfCtrlPointsInV + 1, QVector<qreal>(lengthOfCtrlPointsInV + 1, 0));
    for (int i = 0; i <= lengthOfCtrlPointsInV; i++) {
        int span = bsplineV.FindSpan(knotsInterInV[i]);
        QVector<qreal> basis = bsplineV.BasisFunc(knotsInterInV[i], span);
        for (int j = 0; j <= bsplineV.order; j++) {
            Av[i][j + (span - bsplineV.order)] = basis[j];
        }
    }
    for (int i = 0; i <= lengthOfCtrlPointsInU; i++) {
        for (int dim = 0; dim <= dimension; dim++) {
            QVector<qreal> b(lengthOfInterPointsInV + 1, 0);
            for (int j = 0; j <= lengthOfInterPointsInV; j++) {
                b[j] = middlePoints(i, j, dim);
            }
            QVector<qreal> x = Gauss(Av, b);
            for (int j = 0; j <= lengthOfInterPointsInV; j++) {
                surface.ctrlPoints(i, j, dim) = x[j];
            }
        }
    }
    return surface;  //得到一个B样条曲面的节点矢量及控制点
}

/*!
 * \brief BSplineSurface::SurfaceExtend 曲面延展
 * \param edge                          曲面待延展的方向
 * \param extendLength                  曲面延展的长度
 * \return                              延展后的曲面
 */
BSplineSurface BSplineSurface::SurfaceExtend(Qt::Edge edge, qreal extendLength, int orderU, int orderV)
{
    /* 计算u = 0.5的等参数线的长度，计算v的细分程度 */
    qreal LVmax = 0;
    qreal u = 0.5;
    qreal v = 0;
    QVector3D pointPrevious = this->PointOnSurface(u, v);
    QVector3D pointCurrent;
    while (v < 1) {
        v = v + 0.0001;
        pointCurrent = this->PointOnSurface(u, v);
        LVmax += pointPrevious.distanceToPoint(pointCurrent);
        pointPrevious = pointCurrent;
    }
    const int numOfV = int(LVmax / extendLength);                               /* v方向的细分程度为numOfV + 1 */
    /* 计算v = 0.5的等参数线的长度，计算u的细分程度 */
    qreal LUmax = 0;
    v = 0.5;
    u = 0;
    pointPrevious = this->PointOnSurface(u, v);
    while (u < 1) {
        u = u + 0.0001;
        pointCurrent = this->PointOnSurface(u, v);
        LUmax += pointPrevious.distanceToPoint(pointCurrent);
        pointPrevious = pointCurrent;
    }
    const int numOfU = int(LUmax / extendLength);                               /* u方向的细分程度为numOfU + 1 */
    /* 曲面重采样，采样点间隔为extendLength */
    int sizes[3] = {numOfU + 1, numOfV + 1, 3};
    Matrix samplingPoints(3, sizes);
    for (int row = 0; row < sizes[0]; row++) {
        qreal utemp = 1.0 * row / (sizes[0] - 1);
        for (int col = 0; col < sizes[1]; col++) {
            qreal vtemp = 1.0 * col / (sizes[1] - 1);
            const QVector3D p = PointOnSurface(utemp, vtemp);
            for (int dim = 0; dim < 3; dim++) {
                samplingPoints(row, col, dim) = p[dim];
            }
        }
    }
    /* 曲面延展 */
    Matrix interpolationPointsOfNewSur;                                         /* 新曲面的插值点 */
    Matrix extendLine;                                                          /* 延展增加的点序列 */
    int rowIn = 0, colIn = 0;                                                   /* 新曲面插值点构造时所需要的行、列序号补偿 */
    int rowLine1 = 0, rowLine2 = 0, colLine1 = 0, colLine2 = 0;                 /* 延展增加的点序列的计算时所需要的行、列序号补偿 */
    switch (edge) {
    case Qt::BottomEdge : {                                                     /* bottomEdge 是v = 0时的边界 */
        int size1[3] = {numOfU + 1, numOfV + 2, ctrlPoints.size[2]};
        int size2[3] = {numOfU + 1, 1, ctrlPoints.size[2]};
        interpolationPointsOfNewSur = Matrix(3, size1);
        extendLine = Matrix(3, size2);
        rowIn = 0;
        colIn = -1;
        rowLine1 = 0;
        rowLine2 = 0;
        colLine1 = 0;
        colLine2 = colLine2 + 1;
        break;
    }
    case Qt::TopEdge : {                                                        /* topEdge 是v = 1时的边界 */
        int size1[3] = {numOfU + 1, numOfV + 2, ctrlPoints.size[2]};
        int size2[3] = {numOfU + 1, 1, ctrlPoints.size[2]};
        interpolationPointsOfNewSur = Matrix(3, size1);
        extendLine = Matrix(3, size2);
        rowIn = 0;
        colIn = 0;
        rowLine1 = 0;
        rowLine2 = 0;
        colLine1 = samplingPoints.size[1] - 1;
        colLine2 = colLine1 - 1;
        break;
    }
    case Qt::LeftEdge : {                                                       /* leftEdge 是u = 0时的边界 */
        int size1[3] = {numOfU + 2, numOfV + 1, ctrlPoints.size[2]};
        int size2[3] = {1, numOfV + 1, ctrlPoints.size[2]};
        interpolationPointsOfNewSur = Matrix(3, size1);
        extendLine = Matrix(3, size2);
        rowIn = -1;
        colIn = 0;
        rowLine1 = 0;
        rowLine2 = rowLine1 + 1;
        colLine1 = 0;
        colLine2 = 0;
        break;
    }
    case Qt::RightEdge : {                                                      /* rightEdge 是u = 1时的边界 */
        int size1[3] = {numOfU + 2, numOfV + 1, ctrlPoints.size[2]};
        int size2[3] = {1, numOfV + 1, ctrlPoints.size[2]};
        interpolationPointsOfNewSur = Matrix(3, size1);
        extendLine = Matrix(3, size2);
        rowIn = 0;
        colIn = 0;
        rowLine1 = samplingPoints.size[0] - 1;
        rowLine2 = rowLine1 - 1;
        colLine1 = 0;
        colLine2 = 0;
        break;
    }
    default :
        break;
    }
    /* 计算延展的点序列 */
    for (int row = 0; row < extendLine.size[0]; row++) {
        for (int col = 0; col < extendLine.size[1]; col++) {
            qreal length = 0;
            QVector<qreal> dir(samplingPoints.size[2]);
            for (int dim = 0; dim < samplingPoints.size[2]; dim++) {
                 dir[dim] =  samplingPoints(row + rowLine1, col + colLine1, dim)
                                            - samplingPoints(row + rowLine2, col + colLine2, dim);
                 length += qPow(dir[dim], 2);
            }
//            qDebug() << dir;
            for (int dim = 0; dim < samplingPoints.size[2]; dim++) {
                extendLine(row, col, dim) = dir[dim] / qSqrt(length) * extendLength
                                            + samplingPoints(row + rowLine1, col + colLine1, dim);
                dir[dim] = extendLine(row, col, dim);
            }
//            qDebug() << dir;
        }
    }
    /* 构造新曲面的插值点 */
    for (int col = 0; col < interpolationPointsOfNewSur.size[1]; col++) {
        for (int row = 0; row < interpolationPointsOfNewSur.size[0]; row++){
            for (int dim = 0; dim < interpolationPointsOfNewSur.size[2]; dim++) {
                if (col == 0 && edge == Qt::BottomEdge) {
                    interpolationPointsOfNewSur(row, col, dim) = extendLine(row, 0, dim);
                } else if(col == interpolationPointsOfNewSur.size[1] - 1 && edge == Qt::TopEdge){
                    interpolationPointsOfNewSur(row, col, dim) = extendLine(row, 0, dim);
                } else if(row == 0 && edge == Qt::LeftEdge){
                    interpolationPointsOfNewSur(row, col, dim) = extendLine(0, col, dim);
                } else if(row == interpolationPointsOfNewSur.size[0] - 1 && edge == Qt::RightEdge){
                    interpolationPointsOfNewSur(row, col, dim) = extendLine(0, col, dim);
                } else {
                    interpolationPointsOfNewSur(row, col, dim) = samplingPoints(row + rowIn, col + colIn, dim);
                }
            }
        }
    }
//    return interpolationPointsOfNewSur;
    return FromInterpolation(orderU, orderV, interpolationPointsOfNewSur);      /* 进行曲面插值 */
}
