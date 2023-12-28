#include "GadgetTreeItem.h"
#include "MetaType.h"
#include "../GeneralDelegate.h"
#include <QLoggingCategory>
#include <QMetaProperty>

static Q_LOGGING_CATEGORY(debug, "util.gadgettree");

enum Columns {
    ColKey,
    ColValue,
};

GadgetTreeItem::GadgetTreeItem(int typeId) :
    m_value(typeId)
{
}

GadgetTreeItem::GadgetTreeItem(const QVariant &gadget) :
    m_value(gadget)
{
}

QVariant GadgetTreeItem::data(int column, int role) const
{
    if (isParentProperty) {
        auto *parent = parentItem();
        if (column == ColKey && role == Qt::DisplayRole) {
            MetaType meta = MetaType::fromType(parent->value().userType());
            return meta.gadgetKeyText(parentKey());
        } else if (column == ColValue) {
            return parent->toDisplayValue(role, m_value.toString(), value());
        }
    } else {
        if (column == 0 && role == Qt::DisplayRole) {
            return value().toString();
        } else if (column == 1) {
            return status(role);
        }
    }
    return QVariant();
}

bool GadgetTreeItem::setData(int column, const QVariant &data, int role)
{
    auto *parent = parentItem();
    if (!parent || column != ColValue) {
        return false;
    }

    QVariant d = parent->fromDisplayValue(role, m_value.toString(), data);
    if (value().type() == QVariant::Bool) {
        if (role == Qt::CheckStateRole) {
            return parent->setProperty(m_value, d.toInt() == Qt::Checked);
        }
        return false;
    }
    if (role == Qt::EditRole) {
        return parent->setProperty(m_value, d);
    }
    return false;
}

Qt::ItemFlags GadgetTreeItem::flags(int column) const
{
    Qt::ItemFlags flags = Utils::TreeItem::flags(column);
    if (column == 0) {
        flags &= ~Qt::ItemIsEditable;
    }
    if (column == 1) {
        if (valueType() == ValueSimple) {
            flags |= Qt::ItemIsEditable;
        }
        if (value().type() == QVariant::Bool) {
            flags |= Qt::ItemIsUserCheckable;
        }
    }
    return flags;
}

bool GadgetTreeItem::canFetchMore() const
{
    return (valueType() != ValueSimple) && !childrenCreated;
}

void GadgetTreeItem::fetchMore()
{
    if (childrenCreated) {
        return;
    }
    childrenCreated = true;

    const QVariant v = value();
    ValueType type = valueType();
    if (type == ValueGadget) {
        QMetaType meta(v.userType());
        const QMetaObject *metaObject = meta.metaObject();
        if (!metaObject) {
            qCWarning(debug) << "No valid metaObject" << QMetaType::typeName(v.userType());
            return;
        }
//        MetaType metaType = MetaType::fromType(v.userType());
        for (int index = 0; index < metaObject->propertyCount(); ++index) {
            const QMetaProperty prop = metaObject->property(index);
            auto *item = new GadgetTreeItem(prop.userType());
            item->isParentProperty = true;
            item->m_value = prop.name();
//            item->m_editorProperty = metaType.editorProperty(prop.name());
            appendChild(item);
        }
    } else if (type == ValueList) {
        auto list = v.value<QSequentialIterable>();
        for (int index = 0; index < list.size(); ++index) {
            auto *item = new GadgetTreeItem(QMetaType::UnknownType);
            item->m_value = index;
            item->isParentProperty = true;
            appendChild(item);
        }
    } else if (type == ValueMap) {
        auto list = v.value<QAssociativeIterable>();
        for (auto it = list.begin(); it != list.end(); ++it) {
            auto *item = new GadgetTreeItem(QMetaType::UnknownType);
            item->m_value = it.key();
            item->isParentProperty = true;
            appendChild(item);
        }
    }
}

QVariant GadgetTreeItem::toDisplayValue(int role, const QString &key, const QVariant &value) const
{
    Q_UNUSED(key);
    if (role == Qt::EditRole) {
        return value;
    } else if (role == GeneralDelegate::AdditionalPropertyRole) {
        return editorProperty(key);
    }

    int typeId = value.userType();
    if (typeId == QMetaType::Bool) {
        /* bool类型的显示, 使用model嵌入的复选框 */
        if (role == Qt::CheckStateRole) {
            return value.toBool() ? Qt::Checked : Qt::Unchecked;
        }
    } else if (typeId == QMetaType::QColor || typeId == QMetaType::QIcon || typeId == QMetaType::QPixmap) {
        /* 颜色/图标/图片的显示, 使用model嵌入的图像 */
        if (role == Qt::DecorationRole) {
            return value;
        }
    } else if (typeId == QMetaType::Float || typeId == QMetaType::Double) {
        /* 浮点数字的显示, 与DoubleSpinbox的显示位数和后缀保持一致 */
        if (role == Qt::DisplayRole) {
            bool ok = false;
            auto editorProp = editorProperty(key);
            int precision = editorProp.value("decimals").toInt(&ok);
            QString text = QString::number(value.toDouble(), 'f', ok ? precision : 2);
            if (editorProp.contains("suffix")) {
                text += editorProp.value("suffix").toString();
            }
            return text;
        }
    } else if (typeId == QMetaType::Int) {
        /* 整数的显示, 与SpinBox的specialValueText保持一致 */
        if (role == Qt::DisplayRole) {
            auto editorProp = editorProperty(key);
            if (editorProp.contains("specialValueText") && editorProp.contains("minimum")) {
                if (value == editorProp.value("minimum")) {
                    return editorProp.value("specialValueText");
                }
            }
            return value;
        }
    } else if (MetaType::fromType(typeId).isEnumType()) {
        if (role == Qt::DisplayRole) {
            return MetaType::fromType(typeId).gadgetKeyText(value.toString());
        }
    } else {
        if (role == Qt::DisplayRole) {
            return value.toString();
        }
    }
    return QVariant();
}

QVariant GadgetTreeItem::fromDisplayValue(int role, const QString &key, const QVariant &value) const
{
    Q_UNUSED(role)
    Q_UNUSED(key)
    return value;
}

QVariantHash GadgetTreeItem::editorProperty(const QString &key) const
{
    if (valueType() != ValueGadget) {
        return QVariantHash();
    }
    return MetaType::fromType(value().userType()).editorProperty(key);
}

QVariant GadgetTreeItem::value() const
{
    if (isParentProperty) {
        return parentItem()->property(m_value);
    }
    return m_value;
}

bool GadgetTreeItem::setValue(const QVariant &newValue)
{
    if (isParentProperty) {
        return parentItem()->setProperty(parentKey(), newValue);
    }

//    if (m_value.userType() != newValue.userType()) {
//        return false;
//    }
    m_value = newValue;
    update();
    forAllChildren([] (Utils::TreeItem *child) {
        child->update();
    });
    return true;
}

QVariant GadgetTreeItem::property(const QVariant &key) const
{
    ValueType type = valueType();
    if (type == ValueGadget) {
        return MetaType::readGadgetProperty(value(), key.toString());
    } else if (type == ValueList) {
        return value().value<QSequentialIterable>().at(key.toInt());
    } else if (type == ValueMap) {
        return value().value<QAssociativeIterable>().value(key.toString());
    }
    return QVariant();
}

bool GadgetTreeItem::setProperty(const QVariant &key, const QVariant &propertyValue)
{
    QVariant g = value();
    ValueType type = valueType();
    if (type == ValueGadget) {
        bool ret = MetaType::writeGadgetProperty(g, key.toString(), propertyValue);
        if (!ret) {
            return false;
        }
        return setValue(g);
    } else if (type == ValueList) {
        QVariantList list = g.value<QVariantList>();
        bool ok = false;
        int index = key.toInt(&ok);
        if (!ok || uint(index) >= uint(list.size())) {
            qCWarning(debug) << "Failed writing list with invalid index" << key;
            return false;
        }
        list[index] = propertyValue;
        QVariant newValue = list;
        if (!newValue.convert(g.userType())) {
            qCWarning(debug) << "Failed converting QVariantList to" << g.typeName();
            return false;
        }
        return setValue(newValue);
    } else if (type == ValueMap) {
        QVariantMap map = g.value<QVariantMap>();
        map[key.toString()] = propertyValue;
        QVariant newValue = map;
        if (!newValue.convert(g.userType())) {
            qCWarning(debug) << "Failed converting QVariantMap to" << g.typeName();
            return false;
        }
        return setValue(newValue);
    }
    return false;
}

QVariant GadgetTreeItem::status(int role) const
{
    Q_UNUSED(role);
    ValueType type = valueType();
    if (type == ValueList && role == Qt::DisplayRole) {
        return tr("<%1 项>").arg(value().value<QSequentialIterable>().size());
    } else if (type == ValueMap && role == Qt::DisplayRole) {
        return tr("<%1 项>").arg(value().value<QAssociativeIterable>().size());
    }
    return QVariant();
}

GadgetTreeItem::ValueType GadgetTreeItem::valueType() const
{
    const QVariant v = value();
    int typeId = v.userType();
    if (QMetaType::typeFlags(typeId) & QMetaType::IsGadget) {
        return ValueGadget;
    }
    if (v.canConvert<QVariantList>()) {
        return ValueList;
    } else if (v.canConvert<QVariantMap>()) {
        return ValueMap;
    }
    return ValueSimple;
}

QString GadgetTreeItem::parentKey() const
{
    return m_value.toString();
}
