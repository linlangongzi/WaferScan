#include <QApplication>
#include <QFileDialog>
#include <QVector3D>
#include <QDebug>

#pragma pack(push)
#pragma pack(1)
/*! STL 三角形，与 STL 二进制格式一致 */
struct Triangle {
    QVector3D normal;
    QVector3D v1;
    QVector3D v2;
    QVector3D v3;
    quint16 attribute;
};
#pragma pack(pop)

/*! \class Bound
 * 这个结构体表达在一个指定矢量上，若干点的投影的最大值和最小值 */
struct Bound {
    Bound(const QVector3D &v) : normal(v.normalized()), minDist(qInf()), maxDist(-qInf()) {}
    /*! 使用一个点的投影，更新最大值和最小值 */
    void unite(const QVector3D &v)
    {
        float dist = QVector3D::dotProduct(v, normal);
        maxDist = qMax(maxDist, dist);
        minDist = qMin(minDist, dist);
    }

    QVector3D normal;                                                           /*!< 方向矢量 */
    float minDist;                                                              /*!< 投影最小值 */
    float maxDist;                                                              /*!< 投影最大值 */
};

QDataStream &operator>>(QDataStream &stream, Triangle &vector)
{
    stream >> vector.normal >> vector.v1 >> vector.v2 >> vector.v3 >> vector.attribute;
    return stream;
}

QTextStream &operator<<(QTextStream &stream, const QVector3D &vector)
{
    stream << vector.x() << ", " << vector.y() << ", " << vector.z() << "\n";
    return stream;
}

QVector3D LinePlaneIntersection(const QVector3D &line1, const QVector3D &line2, const QVector3D &plane, const QVector3D &normal)
{
    QVector3D lineDirection = line2 - line1;
    float s1 = QVector3D::dotProduct(normal, plane - line1) / QVector3D::dotProduct(normal, lineDirection);
    return line1 + lineDirection * s1;
}

uint qHash(const QVector3D &key, uint seed = 0)
{
    return qHash(key.x(), seed) + qHash(key.y(), seed) + qHash(key.z(), seed);
}

/*! 将一个矢量添加到矢量列表。当被添加的值已存在时不再重复添加 */
void AddVector(QList<QVector3D> &vectors, const QVector3D &v)
{
//    static const float criterion = 1e-5;
    for (QList<QVector3D>::ConstIterator it = vectors.cbegin(); it != vectors.cend(); ++it) {
//        if (qAbs(it->x() - v.x()) < criterion && qAbs(it->y() - v.y()) < criterion && qAbs(it->z() - v.z()) < criterion) {
//            return;
//        }
        if (qFuzzyCompare(*it, v)) {
            return;
        }
    }
    vectors << v;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString path = QFileDialog::getOpenFileName(NULL, QString(), QString(), "STL (*.stl)");
    if (path.isEmpty()) {
        return 1;
    }
    QFile stl(path);
    if (!stl.open(QIODevice::ReadOnly)) {                                       /* 尝试打开文件 */
        return 1;
    }
    QString header = stl.read(80);                                              /* 读取和跳过STL头 */
    qDebug() << "STL header: " << header;

    QDataStream stream(&stl);                                                   /* 读取 STL 使用二进制流 */
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);             /* 使用单精度float流 */
    quint32 count;
    stream >> count;                                                            /* 读取三角形数量*/

    /* Sanity Check. 剩余字节数应当与三角形数量一致 */
    if (qint64(count * sizeof(Triangle)) > stl.bytesAvailable()) {
        qCritical() << "Insufficient data length: " << count << "triangles (" << count * sizeof(Triangle) << "bytes) expected, " << stl.bytesAvailable() << "available";
        return 1;
    }

    QList<Triangle> triangles;
    triangles.reserve(count);                                                   /* 预先分配三角形列表所需内存 */
    Triangle t;
    Bound bound(QVector3D(0, 1, 0));                                            /* 顶点范围盒，记录所有顶点在进给矢量上的投影位置的最大值和最小值 */
    for (; count; --count) {
        stream >> t;                                                            /* 读取三角形 */
        triangles << t;                                                         /* 将三角形添加到列表 */

        bound.unite(t.v1);                                                      /* 更新顶点范围 */
        bound.unite(t.v2);
        bound.unite(t.v3);
//        qDebug() << t.normal << t.v1 << t.v2 << t.v3;
    }
    qDebug() << "Finished reading STL";

    QString outputPath = QFileDialog::getSaveFileName(NULL, QString(), QString(), "Comma Seperated Values (*.csv)");
    if (outputPath.isEmpty()) {
        return 1;
    }
    QFile outputFile(outputPath);                                               /* 输出 csv 文件 */
    if (!outputFile.open(QIODevice::WriteOnly)) {                               /* 尝试打开 csv */
        return 1;
    }
    QTextStream csv(&outputFile);                                               /* 作为文本流打开 csv */

    qDebug() << "Iterating from" << bound.minDist << "to" << bound.maxDist;
    float step = 5;                                                             /* 进给的步长 */

    /* 沿进给方向生成若干平面，与STL相交 */
    for (float dist = bound.minDist + step / 2; dist <= bound.maxDist; dist += step) {
        QVector3D plane = bound.normal * dist;                                  /* 平面的位置矢量 */

        /* 遍历所有三角形，搜索三角形与当前平面的交点 */
        QList<QVector3D> points;
        foreach (Triangle t, triangles) {
            /* 顶点到平面上一点的矢量，与平面法向量的叉积。该叉积的符号用于判断顶点在平面的哪一侧 */
            float d1 = QVector3D::dotProduct(bound.normal, t.v1 - plane);
            float d2 = QVector3D::dotProduct(bound.normal, t.v2 - plane);
            float d3 = QVector3D::dotProduct(bound.normal, t.v3 - plane);
            /* 对于不同侧的顶点，计算两顶点所在直线与平面的交点，将其添加到交点列表 */
            if (d1 * d2 <= 0) {
                AddVector(points, LinePlaneIntersection(t.v1, t.v2, plane, bound.normal));
            }
            if (d2 * d3 <= 0) {
                AddVector(points, LinePlaneIntersection(t.v2, t.v3, plane, bound.normal));
            }
            if (d3 * d1 <= 0) {
                AddVector(points, LinePlaneIntersection(t.v3, t.v1, plane, bound.normal));
            }
        }

        csv<<points.count()<<'\n';  //写入每个切分面与STL三角片元交点个数，便于分列处理
        /* 将交点列表写入 csv */
        foreach (QVector3D v, points) {
            csv << v;
        }

        qDebug() << "Intersection points" << plane << bound.normal << points.count();
    }
    csv<<'0'; //切分完成结尾标志

    return 0;
    return a.exec();
}
