#pragma once

#include <QHash>
#include <QPoint>
#include "allrising_global.h"

class QTextStream;
class QVector2D;
class QVector3D;
class QVector4D;

/*!
 * \brief qHash 是 QHash 模板类要求的函数，对模板中 key 类型的值进行散列运算，给出一个参考值
 * 供散列 QHash 索引使用。默认 Qt5.3.1 没有提供此函数，由于使用了
 * QHash<QPoint, UTDataFrame>，所以必须为 QHash 提供 qHash(const QPoint &) 的
 * 实现。
 * \param key 散列的键
 * \param seed 散列种子，参见 Qt 的 qHash 文档。
 * \return 散列值
 */
ALLRISING_UTILS_EXPORT uint qHash(const QPoint &key, uint seed);
ALLRISING_UTILS_EXPORT uint qHash(const QVector3D &key, uint seed = 0);

ALLRISING_UTILS_EXPORT QTextStream &operator <<(QTextStream &stream, const QVector3D &vector);

ALLRISING_UTILS_EXPORT bool qIsFinite(const QPointF &v);
ALLRISING_UTILS_EXPORT bool qIsFinite(const QVector2D &v);
ALLRISING_UTILS_EXPORT bool qIsFinite(const QVector3D &v);
ALLRISING_UTILS_EXPORT bool qIsFinite(const QVector4D &v);

ALLRISING_UTILS_EXPORT bool qIsInf(const QPointF &v);
ALLRISING_UTILS_EXPORT bool qIsInf(const QVector2D &v);
ALLRISING_UTILS_EXPORT bool qIsInf(const QVector3D &v);
ALLRISING_UTILS_EXPORT bool qIsInf(const QVector4D &v);

ALLRISING_UTILS_EXPORT bool qIsNaN(const QPointF &v);
ALLRISING_UTILS_EXPORT bool qIsNaN(const QVector2D &v);
ALLRISING_UTILS_EXPORT bool qIsNaN(const QVector3D &v);
ALLRISING_UTILS_EXPORT bool qIsNaN(const QVector4D &v);
