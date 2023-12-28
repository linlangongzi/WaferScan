#include "TriangulatedSurface.h"
#include <allrising/Util.h>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QProgressDialog>
#include <QTextStream>
#include <QtMath>
#include <functional>
#include <QtConcurrent>

QDataStream &operator >>(QDataStream &stream, Triangle &vector)
{
    stream >> vector.normal >> vector.v1 >> vector.v2 >> vector.v3 >> vector.attribute;
    return stream;
}

/*! \class Bound
 * 这个结构体表达在一个指定矢量上，若干点的投影的最大值和最小值 */
struct Bound {
    Bound(const QVector3D &v) : normal(v.normalized()), minDist(qInf()), maxDist(-qInf()) {}
    /*! 使用一个点的投影，更新最大值和最小值 */
    void unite(const QVector3D &v)
    {
        const float dist = QVector3D::dotProduct(v, normal);
        maxDist = qMax(maxDist, dist);
        minDist = qMin(minDist, dist);
    }

    QVector3D normal;                                                           /*!< 方向矢量 */
    float minDist;                                                              /*!< 投影最小值 */
    float maxDist;                                                              /*!< 投影最大值 */
};

/*! 将一个矢量添加到矢量列表。当被添加的值已存在时不再重复添加 */
static inline bool AddVector(QVector<QVector3D> &vectors, const QVector3D &v)
{
    if (!qIsFinite(v)) {
        return false;
    }
    for (auto it = vectors.cbegin(); it != vectors.cend(); ++it) {
        if (it->distanceToPoint(v) < 0.1) {
            return false;
        }
    }
    vectors << v;
    return true;
}

TriangulatedSurface::TriangulatedSurface(const QString &path)
{
    LoadStl(path);
}

TriangulatedSurface::TriangulatedSurface(const QList<Triangle> &tris)
{
    triangles = tris;
}

/*!
 * \brief TriangulatedSurface::IntersectPlaneTriangle
 * \return 返回平面与STL三角片元集合的交点及其法向量
 */
QVector<QPair<QVector<QVector3D>, QVector<QVector3D> > > TriangulatedSurface::IntersectPlaneTriangle(qreal step, const QVector3D &direction) const
{
    Bound bound(direction);

    if (triangles.isEmpty()) {
        qDebug() << "TriangulateSurface: triangles are empty";
        return QVector<QPair<QVector<QVector3D>, QVector<QVector3D> > >();                                  //返回空向量
    }
    foreach (const Triangle &t, triangles) {
        bound.unite(t.v1);
        bound.unite(t.v2);
        bound.unite(t.v3);
    }

    qDebug() << "Iterating from" << bound.minDist << "to" << bound.maxDist;

    /* 沿进给方向生成若干平面，与STL相交 */
    QList<QVector3D> planes;
    for (float dist = bound.minDist + step / 2; dist <= bound.maxDist; dist += step) {
        const QVector3D plane = bound.normal * dist;                                  /* 平面的位置矢量 */
        planes << plane;
    }

    auto intersect = [this, &bound] (const QVector3D &plane) {
        /* 遍历所有三角形，搜索三角形与当前平面的交点 */
        QVector<QVector3D> points;
        QVector<QVector3D> normals;                                             //法向量
        for (const Triangle &t: triangles) {
            /* 顶点到平面上一点的矢量，与平面法向量的叉积。该叉积的符号用于判断顶点在平面的哪一侧 */
            const float d1 = QVector3D::dotProduct(bound.normal, t.v1 - plane);
            const float d2 = QVector3D::dotProduct(bound.normal, t.v2 - plane);
            const float d3 = QVector3D::dotProduct(bound.normal, t.v3 - plane);
            /* 对于不同侧的顶点，计算两顶点所在直线与平面的交点，将其添加到交点列表 */

            if (d1 * d2 <= 0 && AddVector(points, LinePlaneIntersection(t.v1, t.v2, plane, bound.normal))) {
                normals.append(t.normal);
            }
            if (d2 * d3 <= 0 && AddVector(points, LinePlaneIntersection(t.v2, t.v3, plane, bound.normal))) {
                normals.append(t.normal);
            }
            if (d3 * d1 <= 0 && AddVector(points, LinePlaneIntersection(t.v3, t.v1, plane, bound.normal))) {
                normals.append(t.normal);
            }
        }
        return QPair<QVector<QVector3D>, QVector<QVector3D> >(points, normals);
    };
    typedef std::function<QPair<QVector<QVector3D>, QVector<QVector3D> >(const QVector3D &)> MapFunc;
    QFuture<QPair<QVector<QVector3D>, QVector<QVector3D> > > result = QtConcurrent::mapped(planes, MapFunc(intersect));
    return result.results().toVector();
}

/*! 求直线R与三角形T的交点I, http://geomalgorithms.com/a06-_intersect-2.html */
bool TriangulatedSurface::IntersectRayTriangle(const Ray ray, const Triangle triangle, QVector3D &intersection, qreal &deltaDeg)
{
    const qreal d = QVector3D::dotProduct(ray.dir, triangle.normal);
    if (qFuzzyIsNull(d)) {                                                      /* 直线与三角形所在平面平行 */
        return false;
    }
    const qreal r = -QVector3D::dotProduct(ray.p1 - triangle.v1, triangle.normal) / d;
    intersection = ray.p1 + r * ray.dir;                                        /*!< 算出直线与平面的交点 */
    if (qFuzzyCompare(intersection, triangle.v1) || qFuzzyCompare(intersection, triangle.v2) || qFuzzyCompare(intersection, triangle.v3)) {
        return true;
    }

    const QVector3D u = triangle.v2 - triangle.v1;
    const QVector3D v = triangle.v3 - triangle.v1;
    const QVector3D w = intersection - triangle.v1;
    const qreal uv = QVector3D::dotProduct(u, v);
    const qreal wv = QVector3D::dotProduct(w, v);
    const qreal vv = QVector3D::dotProduct(v, v);
    const qreal wu = QVector3D::dotProduct(w, u);
    const qreal uu = QVector3D::dotProduct(u, u);
    const qreal s = (uv * wv - vv * wu) / (uv * uv - uu * vv);
    const qreal t = (uv * wu - uu * wv) / (uv * uv - uu * vv);

    deltaDeg = qRadiansToDegrees(qAcos(qAbs(QVector3D::dotProduct(ray.dir.normalized(), triangle.normal.normalized()))));

    const qreal st = s + t;
    if ((qFuzzyIsNull(s) || s >= 0) && (qFuzzyIsNull(t) || t >= 0) && (qFuzzyCompare(st, 1) || st <= 1)) {
        return true;
    }
    return false;
}

/*! 将曲面各顶点以csv格式导出至文件 */
bool TriangulatedSurface::ExportVertices(const QString &path)
{
    QFile csvFile(path);
    if (!csvFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QTextStream csv(&csvFile);
    QVector<QVector3D> points;
    foreach (Triangle t, triangles) {
        AddVector(points, t.v1);
        AddVector(points, t.v2);
        AddVector(points, t.v3);
    }
    qDebug() << "the number of the pointOut: " << points.size();
    foreach (QVector3D p, points) {
        csv << p << endl;
    }

    return true;
}

/*! 计算直线与曲面的交点 */
bool TriangulatedSurface::IntersectRay(const Ray R, QVector3D &I, qreal &deltaDeg) const
{
    for (int i = 0; i < triangles.size(); i++) {
        bool flag = IntersectRayTriangle(R, triangles[i], I, deltaDeg);
        if (flag == true) {
            return true;
        }
    }
    return false;
}

QVector3D TriangulatedSurface::LinePlaneIntersection(const QVector3D &line1, const QVector3D &line2, const QVector3D &plane, const QVector3D &normal)
{
    const QVector3D lineDirection = line2 - line1;
    const float s1 = QVector3D::dotProduct(normal, plane - line1) / QVector3D::dotProduct(normal, lineDirection);
    return line1 + lineDirection * s1;
}

/*! 对点列进行排序 */
void TriangulatedSurface::SortLine(QVector<QVector3D> &line, const QVector3D &direction)
{
    auto sorter = [&](const QVector3D &v1, const QVector3D &v2)->bool {         /* lambda, Closure 闭包 */
        qreal p1 = QVector3D::dotProduct(v1, direction);
        qreal p2 = QVector3D::dotProduct(v2, direction);
        return p1 < p2;
    };
    qSort(line.begin(), line.end(), sorter);
}

/*! 读取STL文件，导出三角片元List */
void TriangulatedSurface::LoadStl(const QString &path) {
    QFile fileSTL(path);
    if (!fileSTL.open(QIODevice::ReadOnly)) {
        return;
    }
    QProgressDialog progress;
    progress.setLabelText(QString("正在读取STL文件：%1").arg(path));
    progress.setWindowModality(Qt::ApplicationModal);
    if (fileSTL.size() > 3 * 1024000) {                                         //文件大于3M的话，需要显示读取进度
        progress.setMinimumDuration(0);
    }
    qDebug() << "STL file size is: " << fileSTL.size();

    QTextStream stream(&fileSTL);

    /* 处理stlname */
    QString line = stream.readLine().toLocal8Bit();
    QString name = line.mid(6);                                                 //从第6位字符开始为stdyywr名
    qDebug() << "stl file name is: "<< name;
    line = stream.readLine();


    triangles.clear();
    Triangle tri;
    /* 提取三角片元 */
    int counter = 0;
    while (!stream.atEnd() && line != "endsolid" && !progress.wasCanceled()) {
        if (fileSTL.pos() < fileSTL.size()) {
            progress.setValue(qreal(fileSTL.pos()) / qreal(fileSTL.size()) * 100);
        }
        const QStringList elementList = line.split(" ");
        const int size = elementList.size();
        if (size >= 4 && elementList[size - 4] == "normal") {
            QVector3D norm = QVector3D(elementList[size - 3].toDouble(),
                                 elementList[size - 2].toDouble(),
                                 elementList[size - 1].toDouble());
            tri.normal = norm;
            counter = 0;
        } else if (size >= 4 && elementList[size - 4] == "vertex") {
            QVector3D point = QVector3D(elementList[size - 3].toDouble(),
                                        elementList[size - 2].toDouble(),
                                        elementList[size - 1].toDouble());
            switch(counter) {
            case 0:
                tri.v1 = point;
                break;
            case 1:
                tri.v2 = point;
                break;
            case 2:
                tri.v3 = point;
                triangles.append(tri);
                break;
            default:
                break;
            }
            counter++;
        }
        line = stream.readLine();
    }
    qDebug() << "Triangles size: " << triangles.size();
}
