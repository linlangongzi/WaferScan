#include <QtMath>
#include "ImageOptions.h"
#include <allrising/Util.h>

REGISTER_JSON_CONVERTER(ImageOptions);

const QSharedDataPointer<ImageOptions::Data> ImageOptions::dummy(new Data);

ImageOptions::ImageOptions() :
    d(dummy)
{
}

ImageOptions::ImageOptions(const QRectF &sourceRect, const QSize targetRect) :
    d(new Data)
{
    setTransform(sourceRect, targetRect);
    updateTransforms();
}

ImageOptions::ImageOptions(const QRectF &sourceRect, const QSizeF gridSize) :
    d(new Data)
{
    setTransform(sourceRect, gridSize);
}

ImageOptions::ImageOptions(const QSize targetRect, const QSizeF gridSize, const QPointF &sourceOffset) :
    d(new Data)
{
    setTransform(targetRect, gridSize, sourceOffset);
}

const QRectF &ImageOptions::sourceRect() const
{
    return d->source;
}

const QSize &ImageOptions::targetRect() const
{
    return d->target;
}

const QSizeF &ImageOptions::gridSize() const
{
    return d->grid;
}

float ImageOptions::gridArea() const
{
    return d->grid.width() * d->grid.height();
}

const QTransform &ImageOptions::ForwardTransform() const
{
    return d->forward;
}

const QTransform &ImageOptions::InverseTransform() const
{
    return d->inverse;
}

void ImageOptions::setTransform(const QRectF &sourceRect, const QSize targetRect)
{
    d->source = sourceRect;
    d->target = targetRect;
    qreal gridWidth = sourceRect.width() / (targetRect.width() + 1);
    qreal gridHeight = sourceRect.height() / (targetRect.height() + 1);
    d->grid = QSizeF(gridWidth, gridHeight);

    updateTransforms();
}

void ImageOptions::setTransform(const QRectF &sourceRect, const QSizeF gridSize)
{
    d->source = sourceRect;
    int targetWidth = qCeil(sourceRect.width() / gridSize.width());
    int targetHeight = qCeil(sourceRect.height() / gridSize.height());
    d->target = QSize(targetWidth, targetHeight);
    d->grid = gridSize;

    updateTransforms();
}

void ImageOptions::setTransform(const QSize targetRect, const QSizeF gridSize, const QPointF &sourceOffset)
{
    qreal sourceWidth = targetRect.width() * gridSize.width();
    qreal sourceHeight = targetRect.height() * gridSize.height();
    d->source = QRectF(sourceOffset, QSizeF(sourceWidth, sourceHeight));
    d->target = targetRect;
    d->grid = gridSize;

    updateTransforms();
}

bool ImageOptions::IsValid() const
{
    return d->source.isValid() && d->target.isValid() && d->grid.isValid() && d->axisX >= 0 && d->axisY >= 0;
}

QPointF ImageOptions::MapToSource(const QPointF &point) const
{
    if (d->coordinateType == CoordThetaR) {
        float theta = qDegreesToRadians(point.x());
        float r = (d->cyclularCenter - point.y()) / d->grid.width();
        float x = r * qCos(theta);
        float y = r * qSin(theta);
        return QPointF(x, y) + d->sourceOffset;
    }
    return d->forward.map(point);
}

QPointF ImageOptions::MapToSource(const UTDataFrame &frame, int channel) const
{
    qreal x = 0;
    qreal y = 0;
    if (d->coordinateType == CoordRobotic) {
        const auto pos = frame.UpdatePosition();
        if (uint(d->axisX) < 6) {
            x = pos.first[d->axisX];
        }
        if (uint(d->axisY) < 6) {
            y = pos.first[d->axisY];
        }
    } else {
        x = frame.GetPose(d->axisX) - d->backlash.x() * frame.Context.IsReverse(d->axisX);
        y = frame.GetPose(d->axisY) - d->backlash.y() * frame.Context.IsReverse(d->axisY);
    }
    if (uint(channel) < uint(d->offset.count())) {
        x += d->offset[channel].value(d->axisX);
        y += d->offset[channel].value(d->axisY);
    }
    return QPointF(x, y);
}

QPoint ImageOptions::MapToGrid(float x, float y) const
{
    return d->forward.map( QPointF(x, y) ).toPoint();
}

QPoint ImageOptions::MapToGrid(const QPointF &sourcePoint) const
{
    return d->forward.map(sourcePoint).toPoint();
}

QSize ImageOptions::MapToGrid(const QSizeF &sourceSize) const
{
    QPoint point = d->forward.map( QPoint( sourceSize.width(), sourceSize.height() ) );
    return QSize( point.x(), point.y() );
}

QRect ImageOptions::MapToGrid(const QRectF &sourceRect) const
{
    QPoint topLeft = d->forward.map( sourceRect.topLeft() ).toPoint();
    QPoint bottomRight = d->forward.map( sourceRect.bottomRight() ).toPoint();
    return QRect(topLeft, bottomRight);
}

QPolygon ImageOptions::MapToGrid(const QPolygonF &sourcePolygon) const
{
    return d->forward.map(sourcePolygon).toPolygon();
}

QPoint ImageOptions::MapToGridBound(const QPoint &point) const
{
    return BoundToGrid( MapToGrid(point) );
}

QPointF ImageOptions::MapFromGrid(const QPointF &gridPoint) const
{
    return d->inverse.map(gridPoint);
}

QSizeF ImageOptions::MapFromGrid(const QSize &gridSize) const
{
    QPointF point = d->inverse.map( QPointF( gridSize.width(), gridSize.height() ) );
    return QSizeF( point.x(), point.y() );
}

QRectF ImageOptions::MapFromGrid(const QRectF &gridRect) const
{
    QPointF topLeft = d->inverse.map( gridRect.topLeft() );
    QPointF bottomRight = d->inverse.map( gridRect.bottomRight() );
    return QRectF(topLeft, bottomRight);
}

QPolygonF ImageOptions::MapFromGrid(const QPolygonF &gridPolygon) const
{
    return d->inverse.map(gridPolygon);
}

QPointF ImageOptions::MapFromGridBound(const QPoint &point) const
{
    return BoundToSource( MapFromGrid(point) );
}

bool ImageOptions::SourceContains(const QPointF &point) const
{
    return d->source.contains(point);
}

bool ImageOptions::GridContains(const QPoint &point) const
{
    return (uint(point.x()) < uint(d->target.width())) && (uint(point.y()) < uint(d->target.height()));
}

QPointF ImageOptions::BoundToSource(const QPointF &point) const
{
    return QPointF(
        qBound<qreal>(d->source.left(), point.x(), d->source.right()),
        qBound<qreal>(d->source.top(), point.y(), d->source.bottom())
    );
}

QPoint ImageOptions::BoundToGrid(const QPoint &point) const
{
    return QPoint(
        qBound<int>(0, point.x(), d->target.width()),
        qBound<int>(0, point.y(), d->target.height())
    );
}

qreal ImageOptions::AreaToGrid(qreal sourceArea) const
{
    return sourceArea / d->grid.width() / d->grid.height();
}

qreal ImageOptions::AreaFromGrid(qreal gridArea) const
{
    return gridArea * d->grid.width() * d->grid.height();
}

void ImageOptions::setCyclularCenter(float center)
{
    d->cyclularCenter = center;
    updateCyclular();
}

void ImageOptions::setCoordinateType(ImageOptions::CoordinateType newType)
{
    d->coordinateType = newType;
    updateCyclular();
}

void ImageOptions::setFeatureIndex(int index)
{
    d->featureIndex = index;
}

void ImageOptions::setReducePolicy(ImageOptions::ReducePolicy reduce)
{
    d->reducePolicy = reduce;
}

bool ImageOptions::isDirectionEnabled(bool isReversed) const
{
    if (d->direction == DirectionAny) {
        return true;
    } else if (d->direction == DirectionForward) {
        return !isReversed;
    } else if (d->direction == DirectionReverse) {
        return isReversed;
    }
    return false;
}

QJsonValue ImageOptions::toJson() const
{
    QJsonObject obj;
    obj["SourceRect"] = convertToJson(sourceRect());
    obj["Grid"] = convertToJson(gridSize());
    obj["coordinateType"] = EnumToKey(d->coordinateType);
    obj["CyclularCenter"] = d->cyclularCenter;
    obj["AxisX"] = d->axisX;
    obj["AxisY"] = d->axisY;
    obj["Scale"] = convertToJson(d->scale);
    obj["Channels"] = d->channels.toJson();
    obj["Backlash"] = convertToJson(d->backlash);
    obj["FeatureIndex"] = d->featureIndex;
    obj["ReducePolicy"] = EnumToKey(d->reducePolicy);
    obj["DirectionFilter"] = EnumToKey(d->direction);
    obj["ChannelOffset"] = convertToJson(d->offset);
    return obj;
}

ImageOptions ImageOptions::fromJson(const QJsonValue &json)
{
    if (!json.isObject()) {
        return {};
    }
    const QJsonObject obj = json.toObject();
    QRectF sourceRect = convertFromJson<QRectF>(obj["SourceRect"]);
    if (!obj.contains("SourceRect")) {
        sourceRect = QRectF(obj["XOrigin"].toDouble(), obj["YOrigin"].toDouble(), obj["Width"].toDouble(), obj["Height"].toDouble());
    }
    QSizeF gridSize = convertFromJson<QSizeF>(obj["Grid"]);
    if (!obj.contains("Grid")) {
        gridSize = QSizeF(obj["GridWidth"].toDouble(1.0), obj["GridHeight"].toDouble(1.0));
    }

    ImageOptions transform(sourceRect, gridSize);
    transform.d->coordinateType = KeyToEnum(obj["coordinateType"].toString(), CoordNormal);
    if (obj.contains("CyclularType")) {                                         /* COMPAT */
        transform.d->coordinateType = obj["CyclularType"].toString() == "THETA_R" ? CoordThetaR : CoordNormal;
    }
    transform.d->cyclularCenter = obj["CyclularCenter"].toDouble();
    transform.d->axisX = obj["AxisX"].toInt(-1);
    transform.d->axisY = obj["AxisY"].toInt(-1);
    transform.d->scale = obj.contains("Scale") ? JsonToSizeF(obj["Scale"]) : QSizeF(obj["XScale"].toDouble(1.0), obj["YScale"].toDouble(1.0));
    transform.d->channels = convertFromJson<MotionAxisList>(obj["Channels"]);
    transform.d->backlash = convertFromJson<QPointF>(obj["Backlash"]);
    transform.d->featureIndex = obj["FeatureIndex"].toInt();
    transform.d->reducePolicy = KeyToEnum(obj["ReducePolicy"].toString(), ReduceNone);
    transform.d->direction = KeyToEnum(obj["DirectionFilter"].toString(), DirectionAny);
    transform.d->offset = convertFromJson<QVector<QVector<float> > >(obj["ChannelOffset"]);
    transform.updateTransforms();
    return transform;
}

bool ImageOptions::operator ==(const ImageOptions &other) const
{
    return (d == other.d) || MetaType::isGadgetsEqual(*this, other);
}

bool ImageOptions::operator !=(const ImageOptions &other) const
{
    return (d != other.d) && MetaType::isGadgetsDifferent(*this, other);
}

void ImageOptions::updateTransforms()
{
    d->forward = QTransform();
    d->forward.scale( 1 / d->grid.width(), 1 / d->grid.height() );
    d->forward.translate( -d->source.left(), -d->source.top() );
    d->inverse = d->forward.inverted();
    updateCyclular();
}

void ImageOptions::updateCyclular()
{
    if (d->coordinateType == CoordThetaR) {
        d->sourceOffset = QPointF(qreal(d->target.width()) / 2, qreal(d->target.height()) / 2);
    } else {
        d->sourceOffset = QPointF(0, 0);
    }
}

QDebug operator <<(QDebug debug, const ImageOptions &transform)
{
    QDebugStateSaver state(debug);
    Q_UNUSED(state);
    debug.nospace() << "ImageOptions("
                    << transform.sourceRect() << "->" << transform.targetRect()
                    << ", Grid " << transform.gridSize()
                    << ", Axis(" << transform.axisX() << "," << transform.axisY()
                    << ", Scale " << transform.scale() << ")"
                    << ", Channels" << transform.channels();
    return debug;
}
