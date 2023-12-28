#pragma once

#include "../allrising_global.h"
#include <QColor>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QPoint>
#include <QPolygon>
#include <QRect>
#include <QUuid>
#include <QVariant>
#include <QVector3D>
#include <QMetaEnum>
#include <QDataStream>
#include <QImage>
#include <QPixmap>

/*! 将Json转换为字符串，转换失败时返回空字符串 */
ALLRISING_UTILS_EXPORT QByteArray JsonToByteArray(const QJsonValue &value, bool indent = true, QString *errorString = nullptr);
/*! 将字符串转换为Json，转换失败时返回Null */
ALLRISING_UTILS_EXPORT QJsonValue ByteArrayToJson(const QByteArray &data, bool tryCompressed = false, QString *errorString = nullptr);

ALLRISING_UTILS_EXPORT QJsonValue ColorToJson(const QColor &color);
ALLRISING_UTILS_EXPORT QColor JsonToColor(const QJsonValue &value);
ALLRISING_UTILS_EXPORT QJsonValue PointFToJson(const QPointF &point);
ALLRISING_UTILS_EXPORT QJsonValue PointToJson(const QPoint &point);
ALLRISING_UTILS_EXPORT QPointF JsonToPointF(const QJsonValue &value);
ALLRISING_UTILS_EXPORT QPoint JsonToPoint(const QJsonValue &value);
ALLRISING_UTILS_EXPORT QJsonValue SizeFToJson(const QSizeF &size);
ALLRISING_UTILS_EXPORT QJsonValue SizeToJson(const QSize &size);
ALLRISING_UTILS_EXPORT QSizeF JsonToSizeF(const QJsonValue &value);
ALLRISING_UTILS_EXPORT QSize JsonToSize(const QJsonValue &value);
ALLRISING_UTILS_EXPORT QJsonValue PolygonFToJson(const QPolygonF &polygon);
ALLRISING_UTILS_EXPORT QPolygonF JsonToPolygonF(const QJsonValue &value);
ALLRISING_UTILS_EXPORT QJsonValue RectFToJson(const QRectF &rect);
ALLRISING_UTILS_EXPORT QRectF JsonToRectF(const QJsonValue &value);
ALLRISING_UTILS_EXPORT QJsonValue RectToJson(const QRect &rect);
ALLRISING_UTILS_EXPORT QRect JsonToRect(const QJsonValue &value);
ALLRISING_UTILS_EXPORT QVector3D JsonToVector3D(const QJsonValue &value);
ALLRISING_UTILS_EXPORT QJsonValue Vector3DToJson(const QVector3D &vec);
ALLRISING_UTILS_EXPORT QUuid JsonToUuid(const QJsonValue &json);
ALLRISING_UTILS_EXPORT QJsonValue UuidToJson(const QUuid &uuid);
ALLRISING_UTILS_EXPORT QJsonValue ImageToJson(const QImage &image);
ALLRISING_UTILS_EXPORT QImage JsonToImage(const QJsonValue &value);

ALLRISING_UTILS_EXPORT QJsonValue PixmapToJson(const QPixmap &pixmap);
ALLRISING_UTILS_EXPORT QPixmap JsonToPixmap(const QJsonValue &value);

ALLRISING_UTILS_EXPORT QJsonValue VariantToJson(const QVariant &value);
ALLRISING_UTILS_EXPORT QVariant JsonToVariant(const QJsonValue &value, int typeId);
ALLRISING_UTILS_EXPORT QJsonValue GadgetToJson(const QMetaObject &meta, const void *gadget);
ALLRISING_UTILS_EXPORT void JsonToGadget(const QJsonValue &value, const QMetaObject &meta, void *gadget);

/* 前置声明 */
template <typename T>
QJsonValue convertToJson(const T &value);
template <typename T>
T convertFromJson(const QJsonValue &value);

namespace JsonUtilPrivate {
/*! 用于生成辅助模板类的宏.
 * 生成的模板类用于判断T是否含有符合函数签名Signature的成员方法Func. */
#define HAS_MEMBER_FUNC(Name, Signature, Func)                                  \
template <typename T>                                                           \
struct Name                                                                     \
{                                                                               \
    typedef char yes[1];                                                        \
    typedef char no[2];                                                         \
    template <typename U, U> struct type_check;                                 \
    template <typename _1> static yes &check(type_check<Signature, &_1::Func> *);   \
    template <typename> static no &check(...);                                  \
    enum {                                                                      \
        Value = sizeof(check<T>(0)) == sizeof(yes)                              \
    };                                                                          \
};

/*! 辅助模板类, 检测是否存在方法 QJsonValue T::toJson() const */
HAS_MEMBER_FUNC(HasToJson, QJsonValue (T::*)() const, toJson)
/*! 辅助模板类, 检测T是否存在静态方法 static T T::fromJson(const QJsonValue &) */
HAS_MEMBER_FUNC(HasFromJson, T (*)(const QJsonValue &), fromJson)

#undef HAS_MEMBER_FUNC

template <typename T>
struct IsGadget
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    enum {Value = QtPrivate::IsGadgetHelper<T>::IsGadgetOrDerivedFrom};
#elif QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    enum {Value = QtPrivate::IsGadgetHelper<T>::Value};
#else
    enum {Value = false};
#endif
};

template <typename T>
struct IsEnum
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    enum {Value = QtPrivate::IsQEnumHelper<T>::Value};
#else
    enum {Value = false};
#endif
};

/* T = 顺序容器 */
template <typename T>
typename std::enable_if<QtPrivate::IsSequentialContainer<T>::Value, QJsonValue>::type
convertToJsonHelper(const T &list)
{
    if (list.empty()) {
        return QJsonValue();
    }
    QJsonArray array;
    for (const auto &item: list) {
        array << convertToJson<typename T::value_type>(item);
    }
    return array;
}

template <typename T>
typename std::enable_if<QtPrivate::IsSequentialContainer<T>::Value, T>::type
convertFromJsonHelper(const QJsonValue &json)
{
    T list;
    for (const auto &v: json.toArray()) {
        list << convertFromJson<typename T::value_type>(v);
    }
    return list;
}

/* T = 关联容器 */
template <typename T>
typename std::enable_if<QtPrivate::IsAssociativeContainer<T>::Value, QJsonValue>::type
convertToJsonHelper(const T &container)
{
    if (container.size() == 0) {
        return QJsonValue();
    }
    QJsonObject obj;
    for (auto it = container.cbegin(); it != container.cend(); ++it) {
        obj.insert(QVariant(it.key()).toString(), convertToJson(it.value()));
    }
    return obj;
}

template <typename T>
typename std::enable_if<QtPrivate::IsAssociativeContainer<T>::Value, T>::type
convertFromJsonHelper(const QJsonValue &json)
{
    T container;
    QJsonObject obj = json.toObject();
    for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
        auto key = QVariant(it.key()).value<typename T::key_type>();
        auto value = convertFromJson<typename T::mapped_type>(it.value());
        container.insert(key, value);
    }
    return container;
}

/* T 含有 toJson() const 方法 */
template <typename T>
typename std::enable_if<HasToJson<T>::Value, QJsonValue>::type
convertToJsonHelper(const T &gadget)
{
    return gadget.toJson();
}

/* T 含有 static T::fromJson() 方法 */
template <typename T>
typename std::enable_if<HasFromJson<T>::Value, T>::type
convertFromJsonHelper(const QJsonValue &json)
{
    return T::fromJson(json);
}

/* T = Gadget, 并且不含 toJson() const 方法 */
template <typename T>
typename std::enable_if<IsGadget<T>::Value && !HasToJson<T>::Value, QJsonValue>::type
convertToJsonHelper(const T &gadget)
{
    return GadgetToJson(T::staticMetaObject, &gadget);
}

/* T = Gadget, 并且不含 static T::fromJson() 方法 */
template <typename T>
typename std::enable_if<IsGadget<T>::Value && !HasFromJson<T>::Value, T>::type
convertFromJsonHelper(const QJsonValue &json)
{
    T gadget;
    JsonToGadget(json, T::staticMetaObject, &gadget);
    return gadget;
}

/* T = enum (Q_ENUM) */
template <typename T>
typename std::enable_if<IsEnum<T>::Value, QJsonValue>::type
convertToJsonHelper(const T &value)
{
    const QMetaEnum meta = QMetaEnum::fromType<T>();
    return meta.valueToKey(value);
}

template <typename T>
typename std::enable_if<IsEnum<T>::Value, T>::type
convertFromJsonHelper(const QJsonValue &json)
{
    const QMetaEnum meta = QMetaEnum::fromType<T>();
    bool ok = false;
    int v = meta.keyToValue(qPrintable(json.toString()), &ok);
    return ok ? T(v) : T();
}

/* T = (Gadget *) */
template <typename T>
typename std::enable_if<std::is_pointer<T>::value, QJsonValue>::type
convertToJsonHelper(const T &gadget)
{
    using Type = typename std::remove_pointer<T>::type;
    if (!gadget) {
        return QJsonValue();
    }
    return convertToJson<Type>(*gadget);
}

template <typename T>
typename std::enable_if<std::is_pointer<T>::value, T>::type
convertFromJsonHelper(const QJsonValue &json)
{
    using Type = typename std::remove_pointer<T>::type;
    Type *gadget = new Type;
    *gadget = convertFromJson<Type>(json);
    return gadget;
}

/* 将 QVariantList 转换为 QList<T> */
template <typename T>
QList<T> variantListToList(const QVariantList &list)
{
    QList<T> l;
    l.reserve(list.size());
    for (const QVariant &v: list) {
        l << v.value<T>();
    }
    return l;
}

}

/*! 将一个值(T)序列化为 Json.
 * 部分Qt类型有专用序列化器
 * 当T为顺序容器(QList, QVector...)时, 将其转换为QJsonArray
 * 当T为关联容器(QMap, QHash...)时, 将其转换为QJsonObject
 * 当T含有 QJsonValue T::toJson() const 成员方法时, 使用该方法转换.
 * 当T为Gadget, 并且不含 T::toJson() 时, 使用通用序列化器 GadgetToJson() */
template <typename T>
QJsonValue convertToJson(const T &value)
{
    /* 此类型为 Qt 内置类型, 暂不支持将其转换为 json */
    Q_STATIC_ASSERT_X(!QMetaTypeId2<T>::IsBuiltIn, "convertToJson: unsupported built-in type");

    return JsonUtilPrivate::convertToJsonHelper<T>(value);
}

/*! 禁止调用 convertToJson<QVariant>.
 *  QVariant 转换为 json 时将丢失类型信息, 应当使用 VariantToJson */
template<> inline QJsonValue convertToJson<QVariant>(const QVariant &value) = delete;

template<> inline QJsonValue convertToJson<bool>(const bool &value) {return QJsonValue(value);}
template<> inline QJsonValue convertToJson<int>(const int &value) {return QJsonValue(value);}
template<> inline QJsonValue convertToJson<float>(const float &value) {return QJsonValue(double(value));}
template<> inline QJsonValue convertToJson<double>(const double &value) {return QJsonValue(value);}
template<> inline QJsonValue convertToJson<QString>(const QString &value) {return QJsonValue(value);}
template<> inline QJsonValue convertToJson<QStringList>(const QStringList &value) {return convertToJson<QList<QString> >(value);}

template<> inline QJsonValue convertToJson<QJsonValue>(const QJsonValue &value) {return value;}
template<> inline QJsonValue convertToJson<QJsonObject>(const QJsonObject &value) {return value;}
template<> inline QJsonValue convertToJson<QJsonArray>(const QJsonArray &value) {return value;}

template<> inline QJsonValue convertToJson<QSizeF>(const QSizeF &value) {return SizeFToJson(value);}
template<> inline QJsonValue convertToJson<QSize>(const QSize &value) {return SizeToJson(value);}
template<> inline QJsonValue convertToJson<QColor>(const QColor &value) {return ColorToJson(value);}
template<> inline QJsonValue convertToJson<QPointF>(const QPointF &value) {return PointFToJson(value);}
template<> inline QJsonValue convertToJson<QPoint>(const QPoint &value) {return PointToJson(value);}
template<> inline QJsonValue convertToJson<QPolygonF>(const QPolygonF &value) {return PolygonFToJson(value);}
template<> inline QJsonValue convertToJson<QRectF>(const QRectF &value) {return RectFToJson(value);}
template<> inline QJsonValue convertToJson<QRect>(const QRect &value) {return RectToJson(value);}
template<> inline QJsonValue convertToJson<QUuid>(const QUuid &value) {return UuidToJson(value);}
template<> inline QJsonValue convertToJson<QVector3D>(const QVector3D &value) {return Vector3DToJson(value);}
template<> inline QJsonValue convertToJson<QImage>(const QImage &value) {return ImageToJson(value);}
template<> inline QJsonValue convertToJson<QPixmap>(const QPixmap &value) {return PixmapToJson(value);}

/*! 将一个 Json 反序列化为值(T). 与 convertToJson 互逆
* 部分Qt类型有专用序列化器
* 当T为顺序容器(QList, QVector...)时, 按照 QJsonArray 进行读取
* 当T为关联容器(QMap, QHash...)时, 按照 QJsonObject 进行读取
* 当T含有 static T T::fromJson(const QJsonValue &) 方法时, 使用该方法转换.
* 当T为Gadget, 并且不含 T::fromJson() 时, 使用通用反序列化器 JsonToGadget() */
template <typename T>
T convertFromJson(const QJsonValue &value)
{
    /* 此类型为 Qt 内置类型, 暂不支持将 json 转换为此类型 */
    Q_STATIC_ASSERT_X(!QMetaTypeId2<T>::IsBuiltIn, "convertFromJson: unsupported built-in type");

    return JsonUtilPrivate::convertFromJsonHelper<T>(value);
}

/*! 禁止调用convertFromJson<QVariant>.
 *  因为 json 中不含有类型信息, 但将 json 转为 QVariant 需要指定类型. 应当使用 JsonToVariant */
template<> inline QVariant convertFromJson<QVariant>(const QJsonValue &value) = delete;

template<> inline bool convertFromJson<bool>(const QJsonValue &value) {return value.toBool();}
template<> inline int convertFromJson<int>(const QJsonValue &value) {return value.toInt();}
template<> inline float convertFromJson<float>(const QJsonValue &value) {return float(value.toDouble());}
template<> inline double convertFromJson<double>(const QJsonValue &value) {return value.toDouble();}
template<> inline QString convertFromJson<QString>(const QJsonValue &value) {return value.toString();}
template<> inline QStringList convertFromJson<QStringList>(const QJsonValue &value) {return convertFromJson<QList<QString> >(value);}

template<> inline QJsonValue convertFromJson<QJsonValue>(const QJsonValue &value) {return value;}
template<> inline QJsonObject convertFromJson<QJsonObject>(const QJsonValue &value) {return value.toObject();}
template<> inline QJsonArray convertFromJson<QJsonArray>(const QJsonValue &value) {return value.toArray();}

template<> inline QColor convertFromJson<QColor>(const QJsonValue &value) {return JsonToColor(value);}
template<> inline QPoint convertFromJson<QPoint>(const QJsonValue &value) {return JsonToPoint(value);}
template<> inline QPointF convertFromJson<QPointF>(const QJsonValue &value) {return JsonToPointF(value);}
template<> inline QPolygonF convertFromJson<QPolygonF>(const QJsonValue &value) {return JsonToPolygonF(value);}
template<> inline QRect convertFromJson<QRect>(const QJsonValue &value) {return JsonToRect(value);}
template<> inline QRectF convertFromJson<QRectF>(const QJsonValue &value) {return JsonToRectF(value);}
template<> inline QSize convertFromJson<QSize>(const QJsonValue &value) {return JsonToSize(value);}
template<> inline QSizeF convertFromJson<QSizeF>(const QJsonValue &value) {return JsonToSizeF(value);}
template<> inline QUuid convertFromJson<QUuid>(const QJsonValue &value) {return JsonToUuid(value);}
template<> inline QVector3D convertFromJson<QVector3D>(const QJsonValue &value) {return JsonToVector3D(value);}
template<> inline QImage convertFromJson<QImage>(const QJsonValue &value) {return JsonToImage(value);}
template<> inline QPixmap convertFromJson<QPixmap>(const QJsonValue &value) {return JsonToPixmap(value);}

template <typename T>
typename std::enable_if<JsonUtilPrivate::IsGadget<T>::Value, QDataStream &>::type
operator <<(QDataStream &out, const T &gadget)
{
    out << JsonToByteArray(convertToJson(gadget), false);
    return out;
}

template <typename T>
typename std::enable_if<JsonUtilPrivate::IsGadget<T>::Value, QDataStream &>::type
operator >>(QDataStream &in, T &gadget)
{
    QByteArray data;
    in >> data;
    gadget = convertFromJson<T>(ByteArrayToJson(data));
    return in;
}

template <typename T>
bool registerMetaTypeJsonConverter()
{
    QMetaType::registerConverter<T, QJsonValue>(&convertToJson<T>);
    QMetaType::registerConverter<QJsonValue, T>(&convertFromJson<T>);
    return true;
}
