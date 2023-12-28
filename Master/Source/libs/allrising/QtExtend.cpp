#include "QtExtend.h"
#include <QTextStream>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

uint qHash(const QPoint &key, uint seed)
{
    uint h1 = qHash( key.x() );
    uint h2 = qHash( key.y() );
    return ( (h1 << 16) | (h2 >> 16) ) ^ h2 ^ seed;
}

QTextStream &operator <<(QTextStream &stream, const QVector3D &vector)
{
    stream << vector.x() << ", " << vector.y() << ", " << vector.z();
    return stream;
}

uint qHash(const QVector3D &key, uint seed)
{
    return qHash(key.x(), seed) + qHash(key.y(), seed) + qHash(key.z(), seed);
}

bool qIsFinite(const QPointF &v)
{
    return qIsFinite(v.x()) && qIsFinite(v.y());
}

bool qIsFinite(const QVector2D &v)
{
    return qIsFinite(v.x()) && qIsFinite(v.y());
}

bool qIsFinite(const QVector3D &v)
{
    return qIsFinite(v.x()) && qIsFinite(v.y()) && qIsFinite(v.z());
}

bool qIsFinite(const QVector4D &v)
{
    return qIsFinite(v.x()) && qIsFinite(v.y()) && qIsFinite(v.z()) && qIsFinite(v.w());
}

bool qIsInf(const QPointF &v)
{
    return qIsInf(v.x()) && qIsInf(v.y());
}

bool qIsInf(const QVector2D &v)
{
    return qIsInf(v.x()) && qIsInf(v.y());
}

bool qIsInf(const QVector3D &v)
{
    return qIsInf(v.x()) && qIsInf(v.y()) && qIsInf(v.z());
}

bool qIsInf(const QVector4D &v)
{
    return qIsInf(v.x()) && qIsInf(v.y()) && qIsInf(v.z()) && qIsInf(v.w());
}

bool qIsNaN(const QPointF &v)
{
    return qIsNaN(v.x()) && qIsNaN(v.y());
}

bool qIsNaN(const QVector2D &v)
{
    return qIsNaN(v.x()) && qIsNaN(v.y());
}

bool qIsNaN(const QVector3D &v)
{
    return qIsNaN(v.x()) && qIsNaN(v.y()) && qIsNaN(v.z());
}

bool qIsNaN(const QVector4D &v)
{
    return qIsNaN(v.x()) && qIsNaN(v.y()) && qIsNaN(v.z()) && qIsNaN(v.w());
}
