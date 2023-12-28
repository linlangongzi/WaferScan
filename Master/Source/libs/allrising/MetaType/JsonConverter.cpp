#include "MetaType.h"
#include "JsonConverter.h"
#include <QColor>
#include <QDateTime>
#include <QJsonDocument>
#include <QKeySequence>
#include <QLoggingCategory>
#include <QMetaProperty>
#include <QMetaType>
#include <QtEndian>
#include <QUuid>
#include <QBuffer>

static Q_LOGGING_CATEGORY(debug, "util.json");

QByteArray JsonToByteArray(const QJsonValue &value, bool indent, QString *errorString)
{
    QJsonDocument doc;
    if (value.isArray()) {
        doc = QJsonDocument(value.toArray());
    } else if (value.isObject()) {
        doc = QJsonDocument(value.toObject());
    } else {
        if (errorString) {
            *errorString = "JsonDocument: unsupported format";
        }
        return QByteArray();
    }
    return doc.toJson(indent ? QJsonDocument::Indented : QJsonDocument::Compact);
}

QJsonValue ByteArrayToJson(const QByteArray &data, bool tryCompressed, QString *errorString)
{
    if (data.isEmpty()) {
        return QJsonValue();
    }
    QJsonParseError err{};
    const QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError) {
        if (tryCompressed && size_t(data.size()) > sizeof(quint32)) {
            auto size = qFromBigEndian<quint32>(reinterpret_cast<const uchar *>(data.data()));
            if (size <= quint32(data.size())) {
                return ByteArrayToJson(qUncompress(data), false);
            }
        }
        if (errorString) {
            *errorString = err.errorString() + " at " + QString::number(err.offset);
        }
        return QJsonValue();
    }
    if (doc.isArray()) {
        return doc.array();
    }
    if (doc.isObject()) {
        return doc.object();
    }
    return QJsonValue();
}

QJsonValue ColorToJson(const QColor &color)
{
    return color.name(QColor::HexArgb);
}

QColor JsonToColor(const QJsonValue &value)
{
    return value.toString();
}

QJsonValue PointFToJson(const QPointF &point)
{
    QJsonObject obj;
    obj["X"] = point.x();
    obj["Y"] = point.y();
    return obj;
}

QJsonValue PointToJson(const QPoint &point)
{
    return PointFToJson( QPointF(point) );
}

QPointF JsonToPointF(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    return {obj["X"].toDouble(), obj["Y"].toDouble()};
}

QPoint JsonToPoint(const QJsonValue &value)
{
    return JsonToPointF(value).toPoint();
}

QJsonValue SizeFToJson(const QSizeF &size)
{
    QJsonObject obj;
    obj["Width"] = size.width();
    obj["Height"] = size.height();
    return obj;
}

QJsonValue SizeToJson(const QSize &size)
{
    return SizeFToJson(QSizeF(size));
}

QSizeF JsonToSizeF(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    return {obj["Width"].toDouble(), obj["Height"].toDouble()};
}

QSize JsonToSize(const QJsonValue &value)
{
    return JsonToSizeF(value).toSize();
}

QJsonValue PolygonFToJson(const QPolygonF &polygon)
{
    QJsonArray array;
    for (const QPointF &point: polygon) {
        array.append(point.x());
        array.append(point.y());
    }
    return array;
}

QPolygonF JsonToPolygonF(const QJsonValue &value)
{
    QJsonArray array = value.toArray();
    QPolygonF polygon;
    for (int index = 0; index < array.count(); index += 2) {
        polygon.append( QPointF( array.at(index).toDouble(), array.at(index + 1).toDouble() ) );
    }
    return polygon;
}

QJsonValue RectFToJson(const QRectF &rect)
{
    QJsonArray array;
    array.append( PointFToJson(rect.topLeft()));
    array.append( PointFToJson(rect.bottomRight()));
    return array;
}

QRectF JsonToRectF(const QJsonValue &value)
{
    QJsonArray array = value.toArray();
    return QRectF( JsonToPointF( array.at(0) ), JsonToPointF( array.at(1) ) );
}

QJsonValue RectToJson(const QRect &rect)
{
    return RectFToJson( QRectF(rect) );
}

QRect JsonToRect(const QJsonValue &value)
{
    return JsonToRectF(value).toRect();
}

QVector3D JsonToVector3D(const QJsonValue &value)
{
    QJsonArray vec = value.toArray();
    return QVector3D(vec.at(0).toDouble(), vec.at(1).toDouble(), vec.at(2).toDouble());
}

QJsonValue Vector3DToJson(const QVector3D &vec)
{
    return QJsonArray() << vec.x() << vec.y() << vec.z();
}

QUuid JsonToUuid(const QJsonValue &json)
{
    return QUuid(json.toString());
}

QJsonValue UuidToJson(const QUuid &uuid)
{
    return uuid.toString();
}

QImage JsonToImage(const QJsonValue &value)
{
    QImage image;
    QByteArray byteData = QByteArray::fromBase64(value.toString().toLatin1());
    image.loadFromData(byteData, "PPM");
    return image;
}

QJsonValue ImageToJson(const QImage &image)
{
    QByteArray byte;
    QBuffer buffer(&byte);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PPM");
    return QJsonValue(QString::fromLatin1(byte.toBase64()));
}

QJsonValue PixmapToJson(const QPixmap &pixmap)
{
    QByteArray byte;
    QBuffer buffer(&byte);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PPM");
    return QJsonValue(QString::fromLatin1(qCompress(byte, 9).toBase64()));
}

QPixmap JsonToPixmap(const QJsonValue &value)
{
    QPixmap pixmap;
    QByteArray byteData = QByteArray::fromBase64(value.toString().toLatin1());
    pixmap.loadFromData(qUncompress(byteData), "PPM");
    return pixmap;
}

template <typename T>
QJsonValue DateTimeToJson(const T &dateTime)
{
    return dateTime.toString(Qt::ISODate);
}

template <typename T>
T JsonToDateTime(const QJsonValue &json)
{
    return T::fromString(json.toString(), Qt::ISODate);
}

QJsonValue KeySequenceToJson(const QKeySequence &sequence)
{
    return sequence.toString(QKeySequence::PortableText);
}

QKeySequence JsonToKeySequence(const QJsonValue &json)
{
    return QKeySequence::fromString(json.toString(), QKeySequence::PortableText);
}

QJsonValue VariantToJson(const QVariant &value)
{
    if (!value.isValid()) {
        return QJsonValue::Null;
    }
    if (value.canConvert(QMetaType::QJsonValue)) {
        return value.toJsonValue();
    }
    QJsonValue v = QJsonValue::fromVariant(value);
    if (v.type() != QJsonValue::Null) {
        return v;
    }
    switch (value.userType()) {
    case QMetaType::QStringList:    return convertToJson(value.toStringList());
    case QMetaType::QColor:         return ColorToJson(value.value<QColor>());
    case QMetaType::QPoint:         return PointToJson(value.toPoint());
    case QMetaType::QPointF:        return PointFToJson(value.toPointF());
    case QMetaType::QSize:          return SizeToJson(value.toSize());
    case QMetaType::QSizeF:         return SizeFToJson(value.toSizeF());
    case QMetaType::QRect:          return RectToJson(value.toRect());
    case QMetaType::QRectF:         return RectFToJson(value.toRectF());
    case QMetaType::QPolygonF:      return PolygonFToJson(value.value<QPolygonF>());
    case QMetaType::QUuid:          return UuidToJson(value.toUuid());
    case QMetaType::QDateTime:      return DateTimeToJson(value.toDateTime());
    case QMetaType::QDate:          return DateTimeToJson(value.toDate());
    case QMetaType::QTime:          return DateTimeToJson(value.toTime());
    case QMetaType::QVector3D:      return Vector3DToJson(value.value<QVector3D>());
    case QMetaType::QKeySequence:   return KeySequenceToJson(value.value<QKeySequence>());
    case QMetaType::QImage:         return ImageToJson(value.value<QImage>());
    case QMetaType::QPixmap:        return PixmapToJson(value.value<QPixmap>());
    default:                    break;
    }
    if (value.canConvert<QVariantList>()) {
        QSequentialIterable iterable = value.value<QSequentialIterable>();
        if (iterable.size() == 0) {
            return QJsonValue();
        }
        QJsonArray array;
        for (const QVariant &v: iterable) {
            array << VariantToJson(v);
        }
        return array;
    }
    if (value.canConvert<QVariantMap>()) {
        QAssociativeIterable iterable = value.value<QAssociativeIterable>();
        if (iterable.size() == 0) {
            return QJsonValue();
        }
        QJsonObject obj;
        for (auto it = iterable.begin(); it != iterable.end(); ++it) {
            const QVariant key = it.key();
            if (!key.canConvert<QString>()) {
                qCWarning(debug, "Can't convert variant to json as map: Type %s (%d) has unsopported key type %s (%d)", value.typeName(), value.userType(), key.typeName(), key.userType());
                return QJsonValue();
            }
            obj[key.toString()] = VariantToJson(it.value());
        }
        return obj;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    QMetaType type(value.userType());
    if (type.flags() & QMetaType::IsGadget) {
        const QMetaObject *meta = type.metaObject();
        if (!meta) {
            qCWarning(debug, "Can't convert variant to json: Null meta-object for gadget type %s (%d)", QMetaType::typeName(value.userType()), value.userType());
            return QJsonValue();
        }
        return GadgetToJson(*meta, value.constData());
    }
#endif
    qCWarning(debug, "Can't convert variant to json: Unsupported type %s (%d)", value.typeName(), value.userType());
    return QJsonValue();
}

QVariant JsonToVariant(const QJsonValue &value, int typeId)
{
    QVariant variant = value;
    if (variant.canConvert(typeId)) {
        variant.convert(typeId);
        return variant;
    }
    variant = value.toVariant();
    if (variant.userType() == typeId) {
        return variant;
    }
    switch (typeId) {
    case QMetaType::QJsonValue:     return value;
    case QMetaType::QJsonObject:    return value.toObject();
    case QMetaType::QJsonArray:     return value.toArray();
    case QMetaType::QStringList:    return convertFromJson<QStringList>(value);
    case QMetaType::QColor:         return JsonToColor(value);
    case QMetaType::QPoint:         return JsonToPoint(value);
    case QMetaType::QPointF:        return JsonToPointF(value);
    case QMetaType::QSize:          return JsonToSize(value);
    case QMetaType::QSizeF:         return JsonToSizeF(value);
    case QMetaType::QRect:          return JsonToRect(value);
    case QMetaType::QRectF:         return JsonToRectF(value);
    case QMetaType::QPolygonF:      return JsonToPolygonF(value);
    case QMetaType::QUuid:          return JsonToUuid(value);
    case QMetaType::QDateTime:      return JsonToDateTime<QDateTime>(value);
    case QMetaType::QDate:          return JsonToDateTime<QDate>(value);
    case QMetaType::QTime:          return JsonToDateTime<QTime>(value);
    case QMetaType::QVector3D:      return JsonToVector3D(value);
    case QMetaType::QKeySequence:   return JsonToKeySequence(value);
    case QMetaType::QImage:         return JsonToImage(value);
    case QMetaType::QPixmap:        return JsonToPixmap(value);
    default:                        break;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    QMetaType type(typeId);
    if (type.flags() & QMetaType::IsGadget) {
        const QMetaObject *meta = type.metaObject();
        if (!meta) {
            qCCritical(debug, "Can't convert json to variant: Null meta-object for type %s (%d)", QMetaType::typeName(typeId), typeId);
            return QVariant();
        }
        void *gadget = type.create();
        JsonToGadget(value, *meta, gadget);
        QVariant variant(typeId, gadget);
        type.destroy(gadget);
        return variant;
    }
#endif
    qCWarning(debug, "Can't convert json to variant: Unsupported type %s (%d)", QMetaType::typeName(typeId), typeId);
    return QVariant();
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
QJsonValue GadgetToJson(const QMetaObject &meta, const void *gadget)
{
    QJsonObject obj;
    for (int index = meta.propertyOffset(); index < meta.propertyCount(); ++index) {
        const QMetaProperty prop = meta.property(index);
        const QVariant value = prop.readOnGadget(gadget);
        if (!value.isValid()) {
            continue;
        }
        if (prop.isEnumType()) {
            const QMetaEnum e = prop.enumerator();
            obj[prop.name()] = e.valueToKey(value.toInt());
        } else {
            obj[prop.name()] = VariantToJson(value);
        }
    }
    return obj;
}

void JsonToGadget(const QJsonValue &value, const QMetaObject &meta, void *gadget)
{
    if (value.type() != QJsonValue::Object) {
        return;
    }
    QJsonObject obj = value.toObject();
    for (int index = meta.propertyOffset(); index < meta.propertyCount(); ++index) {
        const QMetaProperty prop = meta.property(index);
        const QString key = prop.name();
        if (obj.contains(key)) {
            const QJsonValue v = obj.value(key);
            if (prop.isEnumType()) {
                const QMetaEnum e = prop.enumerator();
                prop.writeOnGadget(gadget, e.keyToValue(qPrintable(v.toString())));
            } else {
                prop.writeOnGadget(gadget, JsonToVariant(v, prop.userType()));
            }
        } else {
#ifdef QT_DEBUG
            qCWarning(debug, "Cannot deserialize object, value of property %s undefined", prop.name());
#endif
        }
    }
}
#endif
