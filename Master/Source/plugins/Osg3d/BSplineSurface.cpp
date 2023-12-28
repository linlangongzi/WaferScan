#include "BSplineCurve.h"
#include "BSplineSurface.h"
#include "MyMath.h"
#include <QDebug>
#include <QMatrix>
#include <QMatrix4x4>
/*!
 * \brief FindNearestPoint  在点云中找到point的最近点，并返回其在QVector中的位置
 *                          算法复杂度为o(n)
 */
int FindNearestPoint(QVector3D point, QVector<QVector3D> pointCloud, QVector<bool> &flag)
{
    qreal min = qInf();
    int pos = -1;

    if (flag.size() != pointCloud.size()) {
        return pos;
    }

    for (int i = 0; i < pointCloud.size(); ++i) {
        const qreal dis = pointCloud[i].distanceToPoint(point);
        if (dis < min && flag[i] == false) {
            min = dis;
            pos = i;
        }
    }
    flag[pos] = true;
    return pos;
}

bool CompareQVector3D(const QVector3D &p1, const QVector3D &p2)
{
    if (p1.length() < p2.length()) {
        return true;
    } else {
        return false;
    }
}
/*!
 * \brief UniquePointCloud  对点云进行排序并去重
 */
QVector<QVector3D> UniquePointCloud(QVector<QVector3D> pointCloud)
{
    qSort(pointCloud.begin(), pointCloud.end(), CompareQVector3D);
    QVector<QVector3D> newPointCloud(1, pointCloud.first());
    for (QVector3D p : pointCloud) {
        if (newPointCloud.last().distanceToPoint(p) >= 0.1) {
            newPointCloud.append(p);
        }
    }
    return newPointCloud;
}

/*!
 * \brief QVectorToMatrix   将QVector<QVector3D> 转换成Matrix
 */
BSplineSurface::Matrix QVectorToMatrix(QVector<QVector3D> line)
{
    BSplineSurface::Matrix newLine(line.size(), 3);
    for (int i = 0; i < line.size(); ++i) {
        QVector3D p = line.value(i);
        newLine(i, 0) = p.x();
        newLine(i, 1) = p.y();
        newLine(i, 2) = p.z();
    }
    return newLine;
}

/*!
 * \brief MatrixToQVector   将Matrix转换成QVector<QVector3D>
 */
QVector<QVector3D> MatrixToQVector(BSplineSurface::Matrix line)
{
    QVector<QVector3D> newLine(line.rows);
    for (int i = 0; i < line.rows; ++i) {
        newLine[i] = QVector3D(line(i, 0), line(i, 1), line(i, 2));
    }
    return newLine;
}

BSplineSurface::BSplineSurface()
{
    isValid = false;
}

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
    if (controlPoints.size[2] == 3) {
        ctrlPoints = controlPoints;                                             // 点的维度必须是三维
        isValid = true;
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
    if (controlPoints.size[2] == 3) {
        ctrlPoints = controlPoints;
        isValid = true;
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

    int indexU = spanU - bsplineU.order;                              //uind为u所在区间有效的控制点序列的起点索引号
    QVector3D temp;
    QVector3D SP(0, 0, 0);
    for (int l = 0; l <= bsplineV.order; l++) {
        temp = QVector3D(0, 0, 0);
        int indexV = spanV - bsplineV.order + l;                      //vind为v所在区间有效的控制点序列的起点索引号
        for (int k = 0; k <= bsplineU.order; ++k) {
            for (int dim = 0; dim < 3; ++dim) {
                temp[dim] += basisU[k] * ctrlPoints(indexU + k, indexV, dim);
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
            if (!qIsFinite(basis[j])){       //判断是否是一个有效的数
                return BSplineSurface();
            }
            Au[i][j + (span - bsplineU.order)] = basis[j];
        }
        if (qFuzzyCompare(Au[i][i], 0)) {                                       //主元为零的情况，返回一个空曲面
            return BSplineSurface();
        }
    }
    /*利用高斯消元法求解中间控制点: 对各个Axis分别进行求解  */
    Matrix middlePoints = Matrix(3, sizes);                                     //中间控制点变量
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
            if (!qIsFinite(basis[j])){       //判断是否是一个有效的数
                return BSplineSurface();
            }
            Av[i][j + (span - bsplineV.order)] = basis[j];
        }
        if (qFuzzyCompare(Av[i][i], 0)) {                                       //主元为零的情况，返回一个空曲面
            return BSplineSurface();
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
    surface.isValid = true;
    return surface;  //得到一个B样条曲面的节点矢量及控制点
}

/*!
 * \brief BSplineSurface::FromInterpolationByCloud  将点云拟合成曲面
 */
BSplineSurface BSplineSurface::FromInterpolationByCloud(QVector<QVector3D> pointCloud,
                                                        QVector3D direction, qreal yaw, int numU, int numV,
                                                        int orderU, int orderV)
{
    //点云去重
    pointCloud = UniquePointCloud(pointCloud);

    //计算旋转矩阵，将direction旋转到与Z轴平行，再将点云沿新direction方向投影
    qreal a = 0,b = 0;
    direction.normalize();
    if (direction.z() < 0) {                                                    //将直线方向向量转化为z分量为正
        direction = -direction;
    }
    if (direction.z() == 0) {
        b = 90;
    } else {
        b = qRadiansToDegrees(atan(direction.x() / direction.z()));
    }
    a = qRadiansToDegrees(asin(direction.y()));
    QMatrix4x4 ra, rb, rc;
    ra.rotate(a, 1, 0, 0);
    rb.rotate(-b, 0, 1, 0);
    rc.rotate(yaw, 0, 0, 1);
    QMatrix4x4 rotate;
    rotate = ra * rb * rc;
    qreal minX = qInf();
    qreal minY = qInf();
    qreal maxX = -qInf();
    qreal maxY = -qInf();
    //对点云进行旋转变换，将其沿原direction方向的投影转换到xy平面
    //计算xy的最小矩形包围盒
    QVector<QVector3D> pointCloudXY(pointCloud.size());
    for (int i = 0; i < pointCloud.size(); ++i){
        QVector3D p = rotate * pointCloud[i];
        pointCloudXY[i] = QVector3D(p.x(), p.y(), 0);
        minX = p.x() > minX ? minX : p.x();
        maxX = p.x() < maxX ? maxX : p.x();
        minY = p.y() > minY ? minY : p.y();
        maxY = p.y() < maxY ? maxY : p.y();
    }
    //求最小曲线包围盒，分别为四条曲线，将点云包络起来
    QVector<QVector3D> bottom(numV + 1);
    QVector<QVector3D> top(numV + 1);
    QVector<QVector3D> right(numU + 1);
    QVector<QVector3D> left(numU + 1);
    QVector<bool> flag(pointCloudXY.size(), false);                               //记录每个点是否被使用
    for (int col = 0; col <= numV; ++col) {
        qreal x = minX + (maxX - minX) / numV * col;

        bottom[col] = QVector3D(x, minY, 0);
        int pos = FindNearestPoint(bottom[col], pointCloudXY, flag);
        bottom[col] = pointCloudXY[pos];

        top[col] = QVector3D(x, maxY, 0);
        pos = FindNearestPoint(top[col], pointCloudXY, flag);
        top[col] = pointCloudXY[pos];
    }
    for (int row = 0; row <= numU; ++row) {
        qreal y = minY + (maxY - minY) / numU * row;

        right[row] = QVector3D(maxX, y, 0);
        int pos = FindNearestPoint(right[row], pointCloudXY, flag);
        right[row] = pointCloudXY[pos];

        left[row] = QVector3D(minX, y, 0);
        pos = FindNearestPoint(left[row], pointCloudXY, flag);
        left[row] = pointCloudXY[pos];
    }
    //对四条线点进行重新均匀划分
    bottom = MatrixToQVector(BSplineCurve::CurveSubdivide(QVectorToMatrix(bottom), bottom.size()));
    top = MatrixToQVector(BSplineCurve::CurveSubdivide(QVectorToMatrix(top), top.size()));
    right = MatrixToQVector(BSplineCurve::CurveSubdivide(QVectorToMatrix(right), right.size()));
    left = MatrixToQVector(BSplineCurve::CurveSubdivide(QVectorToMatrix(left), left.size()));

    //利用曲线包围盒生成参考点阵列
    QVector<QVector<QVector3D> > interpPoints(numU + 1, QVector<QVector3D>(numV + 1));
    //横向收缩变换
    for (int row = 0; row <= numU; ++row) {
        for (int col = 0; col <= numV; ++col) {
            interpPoints[row][col][0] = (left[row] + (right[row] - left[row]) / numV * col).x();
        }
    }
    //纵向收缩变换
    for (int row = 0; row <= numU; ++row) {
        for (int col = 0; col <= numV; ++col) {
            interpPoints[row][col][1] = (bottom[col] + (top[col] - bottom[col]) / numU * row).y();
        }
    }
    //求参考点阵列到xy平面的点云的最近点
    int size[3] = {numU + 1, numV + 1, 3};
    Matrix samplePoints(3, size);
    flag = QVector<bool>(pointCloudXY.size(), false);                            //记录每个点是否被使用
    for (int row = 0; row <= numU; ++row) {
        for (int col = 0; col <= numV; ++col) {
            int pos = FindNearestPoint(interpPoints[row][col], pointCloudXY, flag);
            QVector3D p = pointCloud.value(pos);
            for (int dim = 0; dim < 3; ++dim) {
                samplePoints(row, col, dim) =  p[dim];
            }
        }
    }
    return FromInterpolation(orderU, orderV, samplePoints);
}

/*!
 * \brief BSplineSurface::SurfaceExtend 曲面延展
 * \param edge                          曲面待延展的方向
 * \param extendLength                  曲面延展的长度
 * \return                              延展后的曲面
 */
BSplineSurface BSplineSurface::SurfaceExtend(BSplineSurface sur, Qt::Edge edge, qreal extendLength, int orderU, int orderV)
{
    /* 计算u = 0.5的等参数线的长度，计算v的细分程度 */
    qreal LVmax = 0;
    qreal u = 0.5;
    qreal v = 0;
    qreal duv = 0.0001;
    QVector3D pointPrevious = sur.PointOnSurface(u, v);
    QVector3D pointCurrent;
    while (v < 1) {
        v = v + duv;
        pointCurrent = sur.PointOnSurface(u, v);
        LVmax += pointPrevious.distanceToPoint(pointCurrent);
        pointPrevious = pointCurrent;
    }
    const int numOfV = int(LVmax / extendLength);                               /* v方向的细分程度为numOfV + 1 */
    /* 计算v = 0.5的等参数线的长度，计算u的细分程度 */
    qreal LUmax = 0;
    v = 0.5;
    u = 0;
    pointPrevious = sur.PointOnSurface(u, v);
    while (u < 1) {
        u = u + duv;
        pointCurrent = sur.PointOnSurface(u, v);
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
            const QVector3D p = sur.PointOnSurface(utemp, vtemp);
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
        int size1[3] = {numOfU + 1, numOfV + 2, 3};
        int size2[3] = {numOfU + 1, 1, 3};
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
        int size1[3] = {numOfU + 1, numOfV + 2, 3};
        int size2[3] = {numOfU + 1, 1, 3};
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
        int size1[3] = {numOfU + 2, numOfV + 1, 3};
        int size2[3] = {1, numOfV + 1, 3};
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
        int size1[3] = {numOfU + 2, numOfV + 1, 3};
        int size2[3] = {1, numOfV + 1, 3};
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
    return SurfaceResample(FromInterpolation(orderU, orderV, interpolationPointsOfNewSur),
                           sur.GetCtrlPoints().size[0], sur.GetCtrlPoints().size[1], orderU, orderV);
}

BSplineSurface BSplineSurface::SurfaceResample(BSplineSurface sur, int rowDensity, int colDensity, int orderU, int orderV)
{
    //进行等参数采样
    int size[3] = {rowDensity, colDensity, 3};
    Matrix interpolation(3, size);
    for (int row = 0; row < rowDensity; ++row) {
        qreal u = 1.0 * row / (rowDensity - 1);
        for (int col = 0; col < colDensity; ++col){
            qreal v = 1.0 * col / (colDensity - 1);
            const QVector3D point = sur.PointOnSurface(u, v);
            for (int dim = 0; dim < 3; ++dim) {
                interpolation(row, col, dim) = point[dim];
            }
        }
    }
    return BSplineSurface::FromInterpolation(orderU, orderV, interpolation);
}


/*!
 * \brief CScanSurface::IntersectRaySurface 计算直线与gridSurface网格中最近的点，采用4分法
 * \param linePoint
 * \param lineDir
 * \return
 */
bool BSplineSurface::IntersectRaySurface(BSplineSurface &sur, QVector3D linePoint, QVector3D lineDir, qreal &u, qreal &v)
{
    qreal uMin = 0;
    qreal vMin = 0;
    qreal uMax = 1;
    qreal vMax = 1;
    int counter = 0;
    qreal dmin = sur.PointOnSurface(uMin, vMin).distanceToLine(linePoint, lineDir);
    while (dmin > 0.1 && counter < 30) {
        counter++;
        qreal u1 = (uMin + 0.25 * (uMax - uMin));
        qreal v1 = (vMin + 0.25 * (vMax - vMin));
        qreal d1 = sur.PointOnSurface(u1, v1).distanceToLine(linePoint, lineDir);
        qreal u2 = (uMin + 0.75 * (uMax - uMin));
        qreal v2 = (vMin + 0.25 * (vMax - vMin));
        qreal d2 = sur.PointOnSurface(u2, v2).distanceToLine(linePoint, lineDir);
        qreal u3 = (uMin + 0.75 * (uMax - uMin));
        qreal v3 = (vMin + 0.75 * (vMax - vMin));
        qreal d3 = sur.PointOnSurface(u3, v3).distanceToLine(linePoint, lineDir);
        qreal u4 = (uMin + 0.25 * (uMax - uMin));
        qreal v4 = (vMin + 0.75 * (vMax - vMin));
        qreal d4 = sur.PointOnSurface(u4, v4).distanceToLine(linePoint, lineDir);
        if (d1 <= d2 && d1 <= d3 && d1 <= d4) {
            uMax = (uMax + uMin) / 2;
            vMax = (vMax + vMin) / 2;
            dmin = d1;
        } else if (d2 <= d1 && d2 <= d3 && d2 <= d4) {
            uMin = (uMax + uMin) / 2;
            vMax = (vMax + vMin) / 2;
            dmin = d1;
        } else if (d3 <= d1 && d3 <= d2 && d3 <= d4) {
            uMin = (uMax + uMin) / 2;
            vMin = (vMax + vMin) / 2;
            dmin = d1;
        } else {
            vMin = (vMax + vMin) / 2;
            uMax = (uMin + uMax) / 2;
            dmin = d1;
        }
    }
    if (counter == 30 && dmin > 1) {
        qDebug() << "No intersection of the line and surface";
        return false;
    }
    u = (uMin + uMax) / 2;
    v = (vMin + vMax) / 2;
    return true;
}
/*!
 * \brief BSplineSurface::IntersectRaySurfaceByNewTon 利用曲面上点的参数坐标的拟牛顿迭代法求解
 *  将直线进行旋转变换使其与Z轴平行。利用相同的旋转矩阵，将曲面控制点进行旋转，计算直线的投影(x,y)在新曲面上点的反解
 * \return
 */
bool BSplineSurface::IntersectRaySurfaceByNewTon(BSplineSurface &sur, QVector3D linePoint, QVector3D lineDir, qreal &u, qreal &v)
{
    //计算旋转矩阵，将直线旋转到与Z轴重合
    qreal a,b;
    lineDir.normalize();
    if (lineDir.z() < 0) {                                                       //将直线方向向量转化为z分量为正
        lineDir = -lineDir;
    }
    if (lineDir.z() == 0) {
        b = 90;
    } else {
        b = qRadiansToDegrees(atan(lineDir.x() / lineDir.z()));
    }
    a = qRadiansToDegrees(asin(lineDir.y()));
    QMatrix4x4 ra, rb;
    ra.rotate(a, 1, 0, 0);
    rb.rotate(-b, 0, 1, 0);
    QMatrix4x4 rotate;
    rotate = ra * rb;
    //对曲面控制点进行旋转变换
    Matrix ctrlPoints = sur.GetCtrlPoints();
    Matrix newCtrlPoints = Matrix(3, ctrlPoints.size);
    for (int row = 0; row < ctrlPoints.size[0]; ++row) {
        for (int col = 0; col < ctrlPoints.size[1]; ++col) {
            QVector3D pointOri(ctrlPoints(row, col, 0), ctrlPoints(row, col, 1), ctrlPoints(row, col, 2));
            QVector3D pointPost = rotate * pointOri;
            for (int dim = 0; dim < 3; ++dim) {
                newCtrlPoints(row, col, dim) = pointPost[dim];
            }
        }
    }
    BSplineSurface surf(sur.GetOrderU(), sur.GetOrderV(), sur.GetKnotsU(), sur.GetKnotsV(), newCtrlPoints);
    //对直线进行旋转变换
    linePoint = rotate * linePoint;
    //利用曲面上的点的反解,该算法未使用z坐标，因此没有问题。
    return BSplineSurface::InversePointOnSurfaceByNewTon(surf, linePoint, u, v);
}

/*!
 * \brief CScanSurface::InversePointOnSurface   计算曲面上的点对应的参数(u,v)
 * \param point
 * \return
 */
bool BSplineSurface::InversePointOnSurface(BSplineSurface &sur, const QVector3D &point, qreal &u, qreal &v)
{
    //四叉树网格的范围
    qreal uMin = 0;
    qreal vMin = 0;
    qreal uMax = 1;
    qreal vMax = 1;
    int counter = 0;
    qreal dmin = sur.PointOnSurface(uMin, vMin).distanceToPoint(point);
    while (dmin > 0.1 && counter < 30) {                                //网格达到最小
        counter++;
        qreal u1 = (uMin + 0.25 * (uMax - uMin));
        qreal v1 = (vMin + 0.25 * (vMax - vMin));
        qreal d1 = sur.PointOnSurface(u1, v1).distanceToPoint(point);
        qreal u2 = (uMin + 0.75 * (uMax - uMin));
        qreal v2 = (vMin + 0.25 * (vMax - vMin));
        qreal d2 = sur.PointOnSurface(u2, v2).distanceToPoint(point);
        qreal u3 = (uMin + 0.75 * (uMax - uMin));
        qreal v3 = (vMin + 0.75 * (vMax - vMin));
        qreal d3 = sur.PointOnSurface(u3, v3).distanceToPoint(point);
        qreal u4 = (uMin + 0.25 * (uMax - uMin));
        qreal v4 = (vMin + 0.75 * (vMax - vMin));
        qreal d4 = sur.PointOnSurface(u4, v4).distanceToPoint(point);
        if (d1 <= d2 && d1 <= d3 && d1 <= d4) {
            uMax = (uMax + uMin) / 2;
            vMax = (vMax + vMin) / 2;
            dmin = d1;
        } else if (d2 <= d1 && d2 <= d3 && d2 <= d4) {
            uMin = (uMax + uMin) / 2;
            vMax = (vMax + vMin) / 2;
            dmin = d2;
        } else if (d3 <= d1 && d3 <= d2 && d3 <= d4) {
            uMin = (uMax + uMin) / 2;
            vMin = (vMax + vMin) / 2;
            dmin = d3;
        } else {
            vMin = (vMax + vMin) / 2;
            uMax = (uMin + uMax) / 2;
            dmin = d4;
        }
    }
    if (counter == 30 && dmin > 1) {
        qDebug() << "Fail to calculate the inverse solution";
        return false;
    }
    u = (uMin + uMax) / 2;
    v = (vMin + vMax) / 2;
    qDebug() << "Inverse intersetion point is: " << sur.PointOnSurface((uMin + uMax) / 2, (vMin + vMax) / 2);
    return true;
}
/*!
 * \brief BSplineSurface::InversePointOnSurfaceByNewTon 采用拟牛顿法反求曲面上点的参数坐标
 * \param surface
 * \param point
 * \param u
 * \param v
 * \return
 */
bool BSplineSurface::InversePointOnSurfaceByNewTon(BSplineSurface &surface, const QVector3D &point, qreal &u, qreal &v)
{
    qreal x0 = point.x();
    qreal y0 = point.y();
    qreal uk = u;
    qreal vk = v;
    QVector3D point2 = surface.PointOnSurface(uk, vk);
    qreal f1k = point2.x() - x0;
    qreal f2k = point2.y() - y0;
    Matrix df1 = surface.SurfaceDerivative(uk, vk, 1, Qt::XAxis);
    Matrix df2 = surface.SurfaceDerivative(uk, vk, 1, Qt::YAxis);
    QMatrix Ak(df1(0,1), df1(1,0),
               df2(0,1), df2(1,0), 0, 0);
    int count = 0;
    QPointF sk(1,1);
    while(count < 15 && point2.distanceToPoint(point) > 0.01 &&sk.manhattanLength() > 1e-5) {
        QMatrix AkInv = Ak.inverted();
        qreal uk1 = uk - AkInv.m11() * f1k - AkInv.m12() * f2k;
        qreal vk1 = vk - AkInv.m21() * f1k - AkInv.m22() * f2k;
        point2 = surface.PointOnSurface(uk1, vk1);
        qreal f1k1 = point2.x() - x0;
        qreal f2k1 = point2.y() - y0;
        QPointF yk(f1k1 - f1k, f2k1 - f2k);
        sk = QPointF(uk1- uk, vk1 - vk);
        QPointF Akxsk(Ak.m11()*sk.x()+Ak.m12()*sk.y(), Ak.m21()*sk.x() + Ak.m22()*sk.y());
        QPointF skT = QPointF(sk.x(), sk.y()) / (sk.x() * sk.x() + sk.y() * sk.y());
        QMatrix dAk((yk.x() - Akxsk.x()) * skT.x(), (yk.x() - Akxsk.x()) * skT.y(),
                    (yk.y() - Akxsk.y()) * skT.x(), (yk.y() - Akxsk.y()) * skT.y(),
                    0, 0);
        QMatrix Ak1(Ak.m11() + dAk.m11(), Ak.m12() + dAk.m12(),
                    Ak.m21() + dAk.m21(), Ak.m22() + dAk.m22(), 0, 0);
        //准备下一轮迭代
        Ak = Ak1;
        uk = qBound<qreal>(0, uk1, 1);
        vk = qBound<qreal>(0, vk1, 1);
        f1k = f1k1;
        f2k = f2k1;
        count++;
    }
    if (QVector3D(point2.x(), point2.y(), 0).distanceToPoint(QVector3D(point.x(), point.y(), 0)) < 0.1) {
        u = uk;
        v = vk;
        return true;
    }
    return false;
}
