#pragma once

#include <QVector3D>
#include <QString>
#include <QList>

#pragma pack(push)
#pragma pack(1)
/*! 三角形。 与 STL 二进制格式一致 */
struct Triangle {
    QVector3D normal;
    QVector3D v1;
    QVector3D v2;
    QVector3D v3;
    quint16 attribute;
};
#pragma pack(pop)

/*! 直线：点+方向 */
struct Ray {
    QVector3D p1;
    QVector3D dir;
};


/*! \class 以离散三角形表达的曲面 */
class TriangulatedSurface
{
public:
    TriangulatedSurface(const QString &path = QString());
    TriangulatedSurface(const QList<Triangle> &tris);

    Q_REQUIRED_RESULT QVector<QPair<QVector<QVector3D>, QVector<QVector3D> > > IntersectPlaneTriangle(qreal step, const QVector3D &direction) const;
    bool IntersectRay(const Ray R, QVector3D &I, qreal &deltaDeg) const;
    bool ExportVertices(const QString &path);

    QList<Triangle> GetTriangles() {return triangles;}
    static bool IntersectRayTriangle(const Ray ray, const Triangle triangle, QVector3D &intersection, qreal &deltaDeg);
    static QVector3D LinePlaneIntersection(const QVector3D &line1, const QVector3D &line2, const QVector3D &plane, const QVector3D &normal);
    static void SortLine(QVector<QVector3D> &line, const QVector3D &direction);

private:
    void LoadStl(const QString &path);

private:
    QList<Triangle> triangles;
};
