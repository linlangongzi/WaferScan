#include "UTDefect.h"
#include <QVector2D>

/*!
 * \brief 获取缺陷的表面积，即外边缘面积减内边缘面积
 * 若内边缘面积大于外边缘，则返回0
 * \return 面积
 */
double UTDefect::GetArea() const
{
    return area;
}

/*!
 * \brief 获取特征矩形。特征矩形是可以包围缺陷外边缘的最小矩形。
 * \return 矩形边缘
 */
QPolygonF UTDefect::GetMinRect() const
{
    return minRect;
}

/*!
 * \brief 获取特征矩形的长度，即较大的边长
 * \return 长度
 */
double UTDefect::GetLength() const
{
    if (minRect.size() < 4) {
        return 0;
    }
    QVector2D p1(minRect[0]);
    QVector2D p2(minRect[1]);
    QVector2D p3(minRect[2]);
    return qMax( p1.distanceToPoint(p2), p2.distanceToPoint(p3) );
}

/*!
 * \brief 获取特征矩形的宽度，即较小的边长
 * \return 长度
 */
double UTDefect::GetWidth() const
{
    if (minRect.size() < 4) {
        return 0;
    }
    QVector2D p1(minRect[0]);
    QVector2D p2(minRect[1]);
    QVector2D p3(minRect[2]);
    return qMin( p1.distanceToPoint(p2), p2.distanceToPoint(p3) );
}

/*!
 * \brief 缺陷的特征长度，为特征矩形的对角线长度
 * \return 特征长度
 */
double UTDefect::GetCharacteristicSize() const
{
    if (minRect.size() < 4) {
        return 0;
    }
    return QVector2D(minRect[0]).distanceToPoint( QVector2D(minRect[2]) );
}

/*!
 * \brief 获取特征矩形中心的位置，即特征矩形对角线的交点
 * \return 中心的坐标向量
 */
QVector2D UTDefect::GetCenter() const
{
    if (minRect.size() < 4) {
        return QVector2D();
    }
    return ( QVector2D(minRect[0]) + QVector2D(minRect[2]) ) * 0.5;
}

QList<QImage> UTDefect::screenshots() const
{
    return m_screenshots;
}

void UTDefect::setScreenshots(const QList<QImage> &newImages)
{
    m_screenshots = newImages;
}

/*!
 * \brief 将缺陷对象转换为Json对象
 * \return Json对象
 */
QJsonValue UTDefect::toJson() const
{
    QJsonObject obj;
    obj["MinRect"] = convertToJson(minRect);
    obj["Area"] = GetArea();
    obj["PixelCount"] = pixelCount;
    obj["Length"] = GetLength();
    obj["Width"] = GetWidth();
    obj["CharacteristicSize"] = GetCharacteristicSize();
    QJsonObject center;
    center["X"] = GetCenter().x();
    center["Y"] = GetCenter().y();
    obj["Center"] = center;

    QRectF rect = minRect.boundingRect();
    obj["MinX"] = rect.left();
    obj["MinY"] = rect.top();
    obj["MaxX"] = rect.right();
    obj["MaxY"] = rect.bottom();
    obj["MarkRect"] = convertToJson(markRect);
    obj["MaxAmp"] = maxAmp;
    obj["MinAmp"] = minAmp;
    obj["MinDepth"] = minDepth;
    obj["MaxDepth"] = maxDepth;
    obj["AverageDepth"] = averageDepth;
    obj["BottomDepth"] = bottomDepth;
    obj["Image"] = convertToJson(image);
    obj["Screenshots"] = convertToJson(m_screenshots);
    obj["Percentage"] = convertToJson(percentage);
    return obj;
}

/*!
 * \brief 由Json对象构造缺陷对象
 * \param obj Json对象
 * \return 构造得到的缺陷对象
 */
UTDefect UTDefect::fromJson(const QJsonValue &value)
{
    UTDefect defect;
    QJsonObject obj = value.toObject();
    defect.minRect = convertFromJson<QPolygonF>(obj["MinRect"]);
    defect.area = obj["Area"].toDouble();
    defect.pixelCount = obj["PixelCount"].toDouble();
    defect.averageDepth = obj["AverageDepth"].toDouble();
    defect.minDepth =  obj["MinAmp"].toDouble();
    defect.maxDepth = obj["MaxDepth"].toDouble();
    defect.bottomDepth = obj["BottomDepth"].toDouble();
    defect.minAmp = obj["MinAmp"].toDouble();
    defect.maxAmp = obj["MaxAmp"].toDouble();
    defect.markRect = convertFromJson<QRect>(obj["MarkRect"]);
    defect.image = convertFromJson<QImage>(obj["Image"]);
    defect.m_screenshots = convertFromJson<QList<QImage>>(obj["Screenshots"]);
    defect.percentage = convertFromJson<qreal>(obj["Percentage"]);
    return defect;
}

void WritePoint(QVariantMap &m, const QString &key, const QPointF &point)
{
    m[key] = QString("(%1, %2)").arg(point.x()).arg(point.y());
    m[key + "X"] = point.x();
    m[key + "Y"] = point.y();
}

QVariantMap UTDefect::ToVariantMap(const QPointF &origin) const
{
    QVariantMap m;
    m["Area"] = GetArea();
    m["Length"] = GetLength();
    m["Width"] = GetWidth();
    m["CharacteristicSize"] = GetCharacteristicSize();
    WritePoint(m, "Center", GetCenter().toPointF() - origin);
    m["MinAmp"] = minAmp;
    m["MaxAmp"] = maxAmp;
    m["MinDepth"] = minDepth;
    m["MaxDepth"] = maxDepth;
    m["AverageDepth"] = averageDepth;
    m["MinDepth"] = minDepth;
    m["MaxDepth"] = maxDepth;
    m["BottomDepth"] = bottomDepth;
    m["PixelCount"] = pixelCount;
    m["Percentage"] = percentage;
    return m;
}
