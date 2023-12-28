#pragma once

#include "../allrising_global.h"
#include "JsonConverter.h"
#include <QDebug>
#include <QList>
#include <QMetaEnum>
#include <functional>

class QDebug;

/*!
 * MetaType 是类型管理的接口, 基于 Qt 的 QMetaType 和 QMetaObject, 提供值类型的创建,
 * 转换, 及编辑等服务.
 *
 * 对于Gadget类型, 可以进行比较, debug输出, json转换, 创建编辑控件, 创建询问对话框等操作.
 */
class ALLRISING_UTILS_EXPORT MetaType
{
public:
    MetaType(int typeId = QMetaType::UnknownType);
    static MetaType fromType(int typeId);

    template <typename T>
    static MetaType fromType()
    {
        return fromType(qMetaTypeId<T>());
    }

    int typeId() const;
    bool hasEditor() const;
    QWidget *createEditor() const;
    QString editorPropertyName() const;

    bool isEnumType() const;
    QMetaEnum metaEnum() const;

    bool isGadgetType() const;
    int gadgetPropertyCount() const;
    MetaType gadgetProperty(int index) const;
    QString gadgetKeyText(const QString &key) const;
    QVariantHash editorProperty(const QString &propertyName) const;
//    QWidget *gadgetCreateEditor(int index) const;
//    QString gadgetEditorUserProperty(int index) const;

    static QVariant askValue(int typeId, bool *ok = nullptr);

    /* 注册接口 */
    static bool registerMeta(const MetaType &meta);
    static bool registerEditor(int typeId, std::function<QWidget *()> func, const QString &property);
    static bool registerTr(int typeId, const QString &key, const QString &translation);
    void setTr(const char *key, const QString &translation);
    static bool registerEditorProperty(int typeId, const QString &property, const QString &editorProperty, const QVariant &value);
    void setEditorProperty(const QString &gadgetProperty, const QString editorProperty, const QVariant &value);

    /* Gadget 管理 */
    /*! 检测两个结构体是否相同。两个结构体中的所有属性都必须使用Q_PROPERTY声明，否则会返回错误的结果 */
    static bool isGadgetsEqual(const QMetaObject &meta, const void *v1, const void *v2);
    /*! 检测两个结构体是否不同。两个结构体中的所有属性都必须使用Q_PROPERTY声明，否则会返回错误的结果 */
    static bool isGadgetsDifferent(const QMetaObject &meta, const void *v1, const void *v2);

    template<typename T>
    static bool isGadgetsEqual(const T &v1, const T &v2)
    {
        return isGadgetsEqual(T::staticMetaObject, &v1, &v2);
    }

    template<typename T>
    static bool isGadgetsDifferent(const T &v1, const T &v2)
    {
        return isGadgetsDifferent(T::staticMetaObject, &v1, &v2);
    }

    static QDebug &debugDumpGadget(QDebug &stream, const QMetaObject &meta, const void *gadget);

    template<typename T>
    static QDebug &debugDumpGadget(QDebug &stream, const T &value)
    {
        return debugDumpGadget(stream, T::staticMetaObject, &value);
    }

    static QStringList enumKeyList(const QMetaEnum &meta);

#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    template <typename T>
    static bool setTr(T value, const QString &translation)
    {
        return registerTr(qMetaTypeId<T>(), QMetaEnum::fromType<T>().valueToKey(value), translation);
    }

    /*! 获取一个枚举的所有名称的列表，枚举必须使用Q_ENUM声明 */
    template<typename T>
    static QStringList enumKeyList()
    {
        return enumKeyList(QMetaEnum::fromType<T>());
    }
#endif

    template <typename T>
    static QVariant readGadgetProperty(const T &gadget, const QString &property)
    {
        return readGadgetProperty(&gadget, &T::staticMetaObject, property);
    }
    static QVariant readGadgetProperty(const void *gadget, const QMetaObject *meta, const QString &propertyName);
    static QVariant readGadgetProperty(const QVariant &gadget, const QString &property);

    template <typename T>
    static bool writeGadgetProperty(T &gadget, const QString &property, const QVariant &value)
    {
        return writeGadgetProperty(&gadget, &T::staticMetaObject, property, value);
    }
    static bool writeGadgetProperty(QVariant &gadget, const QString &property, const QVariant &value);
    static bool writeGadgetProperty(void *gadget, const QMetaObject *meta, const QString &propertyName, const QVariant &value);

    template <typename T>
    static void forEachProperty(const T &gadget, std::function<void(QString, QVariant)> func)
    {
        forEachProperty(&gadget, &T::staticMetaObject, func);
    }
    static void forEachProperty(const void *gadget, const QMetaObject *meta, std::function<void(QString, QVariant)> func);

private:
    int m_typeId = QMetaType::UnknownType;                                        /*!< 类型id, 对应qMetaTypeId */

    std::function<QWidget *()> widget;                                          /*!< 工厂方法, 构造一个编辑控件 */
    QString widgetUserProperty;                                                 /*!< 编辑控件的属性名 */
    QHash<QString, QVariantHash> widgetProperty;

    std::function<QVariant (bool *)> dialog;
    QMap<QString, QString> keyTranslates;                                       /*!< Gadget属性名的翻译表 */

    friend ALLRISING_UTILS_EXPORT QDebug &operator <<(QDebug &stream, const MetaType &type);
};

ALLRISING_UTILS_EXPORT QDebug &operator <<(QDebug &stream, const MetaType &type);

template<typename T>
typename std::enable_if<JsonUtilPrivate::IsGadget<T>::Value, QDebug &>::type
operator<<(QDebug &debug, const T &value)
{
    return MetaType::debugDumpGadget(debug, value);
}

/*! Helper for registerJsonConverter() */
template <typename T>
void registerJsonConverterForType()
{
    qRegisterMetaType<T>();
    qRegisterMetaTypeStreamOperators<T>();
    QMetaType::registerDebugStreamOperator<T>();
    QMetaType::registerEqualsComparator<T>();
    registerMetaTypeJsonConverter<T>();
}

template <typename T>
bool registerJsonConverter()
{
    registerJsonConverterForType<T>();
    registerJsonConverterForType<QList<T> >();
    QMetaType::registerConverter<QVariantList, QList<T> >(&JsonUtilPrivate::variantListToList<T>);

    return true;
}

#define REGISTER_JSON_CONVERTER(Type) \
    Q_DECL_UNUSED static const bool jsonConverterRegistered##Type = registerJsonConverter<Type>();

#define DECLARE_COMPARE_OPERATORS(T)                                            \
    inline bool operator ==(const T &other) const                               \
    {                                                                           \
        return MetaType::isGadgetsEqual(staticMetaObject, this, &other);        \
    }                                                                           \
    inline bool operator !=(const T &other) const                               \
    {                                                                           \
        return MetaType::isGadgetsDifferent(staticMetaObject, this, &other);    \
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
#include <QMetaEnum>
#include <QTextStream>
/*! 将一个枚举的名称转换为枚举值，枚举必须使用Q_ENUM声明 */
template<typename T>
T KeyToEnum(const QString &key, T defaultValue)
{
    const QMetaEnum meta = QMetaEnum::fromType<T>();
    bool ok = false;
    int v = meta.keyToValue(qPrintable(key), &ok);
    return ok ? T(v) : defaultValue;
}

/*! 将一个枚举值转换为枚举名称，枚举必须使用Q_ENUM声明 */
template<typename T>
QString EnumToKey(T value)
{
    const QMetaEnum meta = QMetaEnum::fromType<T>();
    return meta.valueToKey(value);
}

/*! 讲一个枚举值转换为多个枚举名称，枚举必须用Q_ENUM声明 */
template<typename T>
QByteArrayList EnumToKeys(T value)
{
    const QMetaEnum meta = QMetaEnum::fromType<T>();
    return meta.valueToKeys(value).split('|');
}

/*! 获取一个枚举的所有值列表，枚举必须使用Q_ENUM声明 */
template<typename T>
QList<T> EnumValueList()
{
    QList<T> values;
    const QMetaEnum meta = QMetaEnum::fromType<T>();
    int count = meta.keyCount();
    values.reserve(count);
    for (int index = 0; index < count; ++index) {
        values << T(meta.value(index));
    }
    return values;
}
#endif
