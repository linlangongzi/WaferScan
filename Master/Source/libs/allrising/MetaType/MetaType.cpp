#include "MetaType.h"
#include "../Util.h"
#include "DialogGadgetEditor.h"
#include "EnumComboBox.h"
#include "GadgetListEditor.h"
#include "JsonConverter.h"
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QLoggingCategory>
#include <QMetaType>
#include <QReadWriteLock>
#include <limits>
#include <utils/qtcolorbutton.h>

struct MetaTypeRegistry
{
    QMap<int, MetaType> map;
    QReadWriteLock lock;
};
Q_GLOBAL_STATIC(MetaTypeRegistry, registry)

static Q_LOGGING_CATEGORY(debug, "util.type");

MetaType::MetaType(int typeId) :
    m_typeId(typeId)
{
}

MetaType MetaType::fromType(int typeId)
{
    MetaTypeRegistry *reg = registry;
    QReadLocker locker(&reg->lock);
    Q_UNUSED(locker);
    return reg->map.value(typeId, MetaType(typeId));
}

int MetaType::typeId() const
{
    return m_typeId;
}

bool MetaType::hasEditor() const
{
    return isEnumType() || widget;
}

QWidget *MetaType::createEditor() const
{
    if (isEnumType()) {
        auto *c = new EnumComboBox;
        QMetaEnum enums = metaEnum();
        for (int index = 0; index < enums.keyCount(); ++index) {
            c->addItem(gadgetKeyText(enums.key(index)), enums.value(index));
        }
        return c;
    }
    if (widget) {
        return widget();
    }
    return nullptr;
}

QString MetaType::editorPropertyName() const
{
    if (isEnumType()) {
        return "currentValue";
    }
    return widgetUserProperty;
}

QString MetaType::gadgetKeyText(const QString &key) const
{
    if (keyTranslates.contains(key)) {
        return keyTranslates.value(key);
    }
    return key;
}

bool MetaType::isEnumType() const
{
    return QMetaType::typeFlags(m_typeId) & QMetaType::IsEnumeration;
}

QMetaEnum MetaType::metaEnum() const
{
    QMetaType meta(m_typeId);
    if (!isEnumType()) {
        return QMetaEnum();
    }
    const QMetaObject *metaObject = meta.metaObject();
    if (!metaObject) {
        qCWarning(debug) << "MetaObject for" << QMetaType::typeName(m_typeId) << "is null";
        return QMetaEnum();
    }
    QString name = QMetaType::typeName(m_typeId);
    int index = metaObject->indexOfEnumerator(qPrintable(name.section("::", -1)));
    if (index < 0) {
        qCWarning(debug) << "MetaType can't find enumerator for type" << name;
    }
    return metaObject->enumerator(index);
}

bool MetaType::isGadgetType() const
{
    return QMetaType::typeFlags(m_typeId) & QMetaType::IsGadget;
}

int MetaType::gadgetPropertyCount() const
{
    if (!isGadgetType()) {
        return 0;
    }
    return QMetaType::metaObjectForType(m_typeId)->propertyCount();
}

MetaType MetaType::gadgetProperty(int index) const
{
    if (!isGadgetType()) {
        return 0;
    }
    int type = QMetaType::metaObjectForType(m_typeId)->property(index).userType();
    return MetaType::fromType(type);
}

bool MetaType::registerMeta(const MetaType &meta)
{
    if (meta.m_typeId == QMetaType::UnknownType) {
        qCCritical(debug) << "Can't register meta for unknown typeId" << meta;
        return false;
    }
    MetaTypeRegistry *reg = registry;
    QWriteLocker locker(&reg->lock);
    reg->map[meta.m_typeId] = meta;
    locker.unlock();
    return true;
}

bool MetaType::registerEditor(int typeId, std::function<QWidget *()> func, const QString &property)
{
    if (typeId == QMetaType::UnknownType) {
        qCCritical(debug) << "Can't register editor for unknown typeId" << property;
        return false;
    }
    /* 加锁, 写入注册表 */
    MetaTypeRegistry *reg = registry;
    QWriteLocker locker(&reg->lock);
    MetaType &meta = reg->map[typeId];
    meta.widget = func;
    meta.widgetUserProperty = property;
    meta.m_typeId = typeId;
    locker.unlock();
    return true;
}

bool MetaType::registerTr(int typeId, const QString &key, const QString &translation)
{
    if (typeId == QMetaType::UnknownType) {
        qCCritical(debug) << "Can't register tr for unknown typeId" << key << translation;
        return false;
    }
    MetaTypeRegistry *reg = registry;
    QWriteLocker locker(&reg->lock);
    MetaType &meta = reg->map[typeId];
    meta.keyTranslates[key] = translation;
    meta.m_typeId = typeId;
    locker.unlock();
    return true;
}

void MetaType::setTr(const char *key, const QString &translation)
{
    keyTranslates[key] = translation;
}

void MetaType::setEditorProperty(const QString &gadgetProperty, const QString editorProperty, const QVariant &value)
{
    widgetProperty[gadgetProperty][editorProperty] = value;
}

QVariantHash MetaType::editorProperty(const QString &propertyName) const
{
    return widgetProperty.value(propertyName);
}

bool MetaType::isGadgetsEqual(const QMetaObject &meta, const void *v1, const void *v2)
{
    for (int index = meta.propertyOffset(); index < meta.propertyCount(); ++index) {
        const QMetaProperty prop = meta.property(index);
        if (prop.readOnGadget(v1) != prop.readOnGadget(v2)) {
            return false;
        }
    }
    return true;
}

bool MetaType::isGadgetsDifferent(const QMetaObject &meta, const void *v1, const void *v2)
{
    for (int index = meta.propertyOffset(); index < meta.propertyCount(); ++index) {
        const QMetaProperty prop = meta.property(index);
        if (prop.readOnGadget(v1) != prop.readOnGadget(v2)) {
            return true;
        }
    }
    return false;
}

QStringList MetaType::enumKeyList(const QMetaEnum &meta)
{
    QStringList keys;
    int count = meta.keyCount();
    keys.reserve(count);
    for (int index = 0; index < count; ++index) {
        keys << meta.key(index);
    }
    return keys;
}

QVariant MetaType::readGadgetProperty(const void *gadget, const QMetaObject *meta, const QString &propertyName)
{
    if (!meta) {
        return QVariant();
    }

    int index = meta->indexOfProperty(qPrintable(propertyName));
    if (index < 0) {
        qCCritical(debug) << "Can't read property of gadget" << meta->className() << ": property" << propertyName << "not exist";
        return QVariant();
    }
    QMetaProperty prop = meta->property(index);
    if (!prop.isReadable()) {
        qCCritical(debug) << "Can't read property of gadget" << meta->className() << ": property" << propertyName << "is not readable";
        return QVariant();
    }
    return prop.readOnGadget(gadget);
}

QVariant MetaType::readGadgetProperty(const QVariant &gadget, const QString &property)
{
    const QMetaType metaType(gadget.userType());
    if (!(metaType.flags() & QMetaType::IsGadget)) {
        qCCritical(debug) << "Can't read property of gadget" << gadget.typeName() << ": not gadget";
        return QVariant();
    }
    const QMetaObject *meta = metaType.metaObject();
    return readGadgetProperty(gadget.constData(), meta, property);
}

bool MetaType::writeGadgetProperty(QVariant &gadget, const QString &property, const QVariant &value)
{
    const QMetaType metaType(gadget.userType());
    if (!(metaType.flags() & QMetaType::IsGadget)) {
        qCCritical(debug) << "Can't write gadget property: not gadget" << gadget.typeName();
        return false;
    }
    const QMetaObject *meta = metaType.metaObject();
    return writeGadgetProperty(gadget.data(), meta, property, value);
}

bool MetaType::writeGadgetProperty(void *gadget, const QMetaObject *meta, const QString &propertyName, const QVariant &value)
{
    if (!meta) {
        return false;
    }
    int index = meta->indexOfProperty(qPrintable(propertyName));
    if (index < 0) {
        qCCritical(debug) << "Can't write property of gadget" << meta->className() << ": property" << propertyName << "not exist";
        return false;
    }
    QMetaProperty prop = meta->property(index);
    if (!prop.isWritable()) {
        qCCritical(debug) << "Can't write property of gadget" << meta->className() << ": property" << propertyName << "is not writable";
        return false;
    }
    return prop.writeOnGadget(gadget, value);
}

void MetaType::forEachProperty(const void *gadget, const QMetaObject *meta, std::function<void (QString, QVariant)> func)
{
    if (!func) {
        return;
    }
    for (int index = 0; index < meta->propertyCount(); ++index) {
        const auto prop = meta->property(index);
        func(prop.name(), readGadgetProperty(gadget, meta, prop.name()));
    }
}

QDebug &MetaType::debugDumpGadget(QDebug &stream, const QMetaObject &meta, const void *gadget)
{
    QDebugStateSaver state(stream);
    Q_UNUSED(state);
    stream.nospace() << meta.className() << '(';

    for (int index = meta.propertyOffset(); index < meta.propertyCount(); ++index) {
        const QMetaProperty prop = meta.property(index);
        const QVariant v = prop.readOnGadget(gadget);
        stream << prop.name() << '=';
        bool userStream = QMetaType::debugStream(stream, v.constData(), v.userType());
        if (!userStream) {
            QDebugStateSaver state(stream);
            Q_UNUSED(state);
            if (prop.isEnumType() || prop.isFlagType()) {
                QMetaEnum e = prop.enumerator();
                if (e.isValid()) {
                    if (prop.isFlagType()) {
                        stream.noquote() << e.valueToKeys(v.toInt());
                    } else {
                        stream << e.valueToKey(v.toInt());
                    }
                }
                stream << "(" << v.toInt() << ")";
            } else if (v.canConvert<QString>()) {
                stream.noquote() << v.toString();
            } else {
                stream << v;
            }
        }
        stream << ' ';
    }

    stream << ')';
    return stream;
}

QDebug &operator <<(QDebug &stream, const MetaType &type)
{
    QDebugStateSaver state(stream);
    Q_UNUSED(state);
    stream.nospace() << "MetaType(" << QMetaType::typeName(type.typeId()) << "(" << type.typeId() << "),"
                     "hasEditor=" << type.hasEditor() << ",isEnum=" << type.isEnumType() << ",translations=" << type.keyTranslates << ")";
    return stream;
}

template <typename T>
void registerBuildInJsonConverters()
{
    QMetaType::registerConverter<T, QJsonValue>(&convertToJson<T>);
    QMetaType::registerConverter<QJsonValue, T>(&convertFromJson<T>);
}
template <typename T>
void registerBuiltInJsonConvertersForType()
{
    registerBuildInJsonConverters<QList<T> >();
    registerBuildInJsonConverters<QVector<T> >();
    registerBuildInJsonConverters<QMap<QString, T> >();
    registerBuildInJsonConverters<QHash<QString, T> >();
    registerBuildInJsonConverters<QMap<int, T> >();
    registerBuildInJsonConverters<QHash<int, T> >();
}

Q_DECL_UNUSED static const bool builtinConverters = []() {
#define R(T) registerBuiltInJsonConvertersForType<T>();
    R(float)
    R(double)
    R(int)
    R(QString)
    R(QJsonValue)
    R(QJsonObject)
    R(QJsonArray)
    R(QSizeF)
    R(QSize)
    R(QColor)
    R(QPointF)
    R(QPoint)
    R(QPolygonF)
    R(QRectF)
    R(QRect)
    R(QUuid)
    R(QVector3D)
#undef R
    return true;
}();

Q_DECL_UNUSED static const bool builtinEditors = []() {
    auto reg = &MetaType::registerEditor;
    bool ret = true;
    ret &= reg(QMetaType::Bool, [](){return new QCheckBox;}, "checked");
    ret &= reg(QMetaType::Short, [](){
        auto *c = new QSpinBox;
        c->setRange(std::numeric_limits<short>::min(), std::numeric_limits<short>::max());
        return c;
    }, "value");
    ret &= reg(QMetaType::Int, [](){
        auto *c = new QSpinBox;
        c->setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        return c;
    }, "value");
    ret &= reg(QMetaType::Float, [](){auto *c = new QDoubleSpinBox; c->setRange(-qInf(), qInf()); return c;}, "value");
    ret &= reg(QMetaType::Double, [](){auto *c = new QDoubleSpinBox; c->setRange(-qInf(), qInf()); return c;}, "value");

    ret &= reg(QMetaType::QString, [](){return new QLineEdit;}, "text");
    ret &= reg(QMetaType::QDate, [](){return new QDateEdit;}, "date");
    ret &= reg(QMetaType::QTime, [](){return new QTimeEdit;}, "time");
    ret &= reg(QMetaType::QDateTime, [](){return new QDateTimeEdit;}, "dateTime");
    ret &= reg(QMetaType::QColor, [](){return new Utils::QtColorButton;}, "color");
    return ret;
}();
