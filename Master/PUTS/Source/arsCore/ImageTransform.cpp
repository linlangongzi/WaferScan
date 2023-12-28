#include <QtMath>
#include "ImageTransform.h"

const int typeId = qRegisterMetaType<ImageTransform>("ImageTransform");

ImageTransform::ImageTransform() :
    source(),
    target(),
    grid(),
    forward(),
    inverse(),
    scale(QSizeF(1, 1))
{
    SetCyclularType(CYCLULAR_NONE, 0);
}

ImageTransform::ImageTransform(const ImageTransform &other) :
    source(other.source),
    target(other.target),
    grid(other.grid),
    forward(other.forward),
    inverse(other.inverse),
    scale(other.scale)
{
    SetCyclularType(other.cyclularType, other.cyclularCenter);
}

ImageTransform::ImageTransform(const QRectF &sourceRect, const QSize targetRect) :
    source(sourceRect),
    target(targetRect),
    grid( sourceRect.width() / (targetRect.width() + 1), sourceRect.height() / (targetRect.height() + 1) ),
    scale(QSizeF(1, 1))
{
    SetCyclularType(CYCLULAR_NONE, 0);
    UpdateTransforms();
}

ImageTransform::ImageTransform(const QRectF &sourceRect, const QSizeF gridSize) :
    source(sourceRect),
    target( qCeil( sourceRect.width() / gridSize.width() ), qCeil( sourceRect.height() / gridSize.height() ) ),
    grid(gridSize),
    scale(QSizeF(1, 1))
{
    SetCyclularType(CYCLULAR_NONE, 0);
    UpdateTransforms();
}

ImageTransform::ImageTransform(const QSize targetRect, const QSizeF gridSize, const QPointF &sourceOffset) :
    source( sourceOffset, QSizeF( targetRect.width() * gridSize.width(), targetRect.height() * gridSize.height() ) ),
    target(targetRect),
    grid(gridSize),
    scale(QSizeF(1, 1))
{
    SetCyclularType(CYCLULAR_NONE, 0);
    UpdateTransforms();
}

const QRectF &ImageTransform::SourceRect() const
{
    return source;
}

const QSize &ImageTransform::TargetRect() const
{
    return target;
}

const QSizeF &ImageTransform::GridSize() const
{
    return grid;
}

float ImageTransform::GridArea() const
{
    return grid.width() * grid.height();
}

const QTransform &ImageTransform::ForwardTransform() const
{
    return forward;
}

const QTransform &ImageTransform::InverseTransform() const
{
    return inverse;
}

bool ImageTransform::IsValid() const
{
    return source.isValid() && target.isValid() && grid.isValid();
}

QPointF ImageTransform::MapToSource(const QPointF &point) const
{
#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
    if (cyclularType == CYCLULAR_THETA_R) {
        float theta = qDegreesToRadians(point.x());
        float r = (cyclularCenter - point.y()) / grid.width();
        float x = r * qCos(theta);
        float y = r * qSin(theta);
        return QPointF(x, y) + sourceOffset;
    }
#endif
    return forward.map(point);
}

QPoint ImageTransform::MapToGrid(float x, float y) const
{
    return forward.map( QPointF(x, y) ).toPoint();
}

QPoint ImageTransform::MapToGrid(const QPointF &sourcePoint) const
{
    return forward.map(sourcePoint).toPoint();
}

QSize ImageTransform::MapToGrid(const QSizeF &sourceSize) const
{
    QPoint point = forward.map( QPoint( sourceSize.width(), sourceSize.height() ) );
    return QSize( point.x(), point.y() );
}

QRect ImageTransform::MapToGrid(const QRectF &sourceRect) const
{
    QPoint topLeft = forward.map( sourceRect.topLeft() ).toPoint();
    QPoint bottomRight = forward.map( sourceRect.bottomRight() ).toPoint();
    return QRect(topLeft, bottomRight);
}

QPolygon ImageTransform::MapToGrid(const QPolygonF &sourcePolygon) const
{
    return forward.map(sourcePolygon).toPolygon();
}

QPoint ImageTransform::MapToGridBound(const QPoint &point) const
{
    return BoundToGrid( MapToGrid(point) );
}

QPointF ImageTransform::MapFromGrid(const QPointF &gridPoint) const
{
    return inverse.map(gridPoint);
}

QSizeF ImageTransform::MapFromGrid(const QSize &gridSize) const
{
    QPointF point = inverse.map( QPointF( gridSize.width(), gridSize.height() ) );
    return QSizeF( point.x(), point.y() );
}

QRectF ImageTransform::MapFromGrid(const QRectF &gridRect) const
{
    QPointF topLeft = inverse.map( gridRect.topLeft() );
    QPointF bottomRight = inverse.map( gridRect.bottomRight() );
    return QRectF(topLeft, bottomRight);
}

QPolygonF ImageTransform::MapFromGrid(const QPolygonF &gridPolygon) const
{
    return inverse.map(gridPolygon);
}

QPointF ImageTransform::MapFromGridBound(const QPoint &point) const
{
    return BoundToSource( MapFromGrid(point) );
}

bool ImageTransform::SourceContains(const QPointF &point) const
{
    return source.contains(point);
}

bool ImageTransform::GridContains(const QPoint &point) const
{
    return ( uint( point.x() ) < uint( target.width() ) ) && ( uint( point.y() ) < uint( target.height() ) );
}

QPointF ImageTransform::BoundToSource(const QPointF &point) const
{
    return QPointF(
        qBound<qreal>( source.left(), point.x(), source.right() ),
        qBound<qreal>( source.top(), point.y(), source.bottom() )
    );
}

QPoint ImageTransform::BoundToGrid(const QPoint &point) const
{
    return QPoint(
        qBound<int>( 0, point.x(), target.width() ),
        qBound<int>( 0, point.y(), target.height() )
    );
}

qreal ImageTransform::AreaToGrid(qreal sourceArea) const
{
    return sourceArea / grid.width() / grid.height();
}

qreal ImageTransform::AreaFromGrid(qreal gridArea) const
{
    return gridArea * grid.width() * grid.height();
}

bool ImageTransform::operator ==(const ImageTransform &other) const
{
    return (source == other.source) && (grid == other.grid);
}

bool ImageTransform::operator !=(const ImageTransform &other) const
{
    return !(*this == other);
}

#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
QJsonObject ImageTransform::ToJson() const
{
    QJsonObject obj;
    obj["XOrigin"] = SourceRect().x();
    obj["YOrigin"] = SourceRect().y();
    obj["Width"] = SourceRect().width();
    obj["Height"] = SourceRect().height();
    obj["GridHeight"] = GridSize().height();
    obj["GridWidth"] = GridSize().width();
    obj["CyclularType"] = cyclularType == CYCLULAR_THETA_R ? "THETA_R" : "NONE";
    obj["CyclularCenter"] = cyclularCenter;
    obj["XScale"] = scale.width();
    obj["YScale"] = scale.height();
    return obj;
}

ImageTransform ImageTransform::FromJson(const QJsonObject &obj)
{
    QSizeF scale( obj["XScale"].toDouble(1.0), obj["YScale"].toDouble(1.0) );
    QRectF rect( obj["XOrigin"].toDouble(), obj["YOrigin"].toDouble(), obj["Width"].toDouble(), obj["Height"].toDouble() );
    QSizeF size( obj["GridWidth"].toDouble(), obj["GridHeight"].toDouble() );
    CyclularType type = obj["CyclularType"].toString() == "THETA_R" ? CYCLULAR_THETA_R : CYCLULAR_NONE;
    float center = obj["CyclularCenter"].toDouble();
    ImageTransform transform(rect, size);
    transform.SetCyclularType(type, center);
    transform.SetScale(scale);
    return transform;
}
#endif
void ImageTransform::SetCyclularType(ImageTransform::CyclularType newType, float center)
{
    cyclularType = newType;
    cyclularCenter = center;
    if (newType == CYCLULAR_NONE) {
        sourceOffset = QPointF(0, 0);
    } else {
        sourceOffset = QPointF(float(target.width()) / 2, float(target.height()) / 2);
    }
}

void ImageTransform::UpdateTransforms()
{
    forward = QTransform();
    forward.scale( 1 / grid.width(), 1 / grid.height() );
    forward.translate( -source.left(), -source.top() );
    inverse = forward.inverted();
}

QDebug operator <<(QDebug debug, const ImageTransform &transform)
{
    return debug << "ImageTransform(" << transform.SourceRect() << "->" << transform.TargetRect() << ", Grid" << transform.GridSize() << ")";
}
