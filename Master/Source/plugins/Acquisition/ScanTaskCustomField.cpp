#include "ScanTaskCustomField.h"
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <allrising/MetaType/JsonConverter.h>

QVariant ScanTaskCustomField::data(int column, int role) const
{
    switch (column) {
    case Required:
        if (role == Qt::CheckStateRole) {
            return isRequired ? Qt::Checked : Qt::Unchecked;
        }
        break;
    case Key:
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return key;
        }
        break;
    case Name:
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return displayName;
        }
        break;
    case Value:
        if (role == Qt::DisplayRole) {
            return valueList.join(", ");
        }
        break;
    default:
        break;
    }
    return QVariant();
}

bool ScanTaskCustomField::setData(int column, const QVariant &data, int role)
{
    switch (column) {
    case Required:
        if (role == Qt::CheckStateRole) {
            isRequired = (data.toInt() == Qt::Checked);
            return true;
        }
        return false;
    case Key:
        if (role == Qt::EditRole) {
            key = data.toString();
            return true;
        }
        return false;
    case Name:
        if (role == Qt::EditRole) {
            displayName = data.toString();
            return true;
        }
        return false;
    case Value:
        return false;
    default:
        break;
    }
    return true;
}

Qt::ItemFlags ScanTaskCustomField::flags(int column) const
{
    auto flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (column == Required) {
        flags |= Qt::ItemIsUserCheckable;
    }
    if (column == Key || column == Name) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

//! 从Model中按照MetaObject属性表的规则读取JsonValue
void ScanTaskCustomField::loadJson(const QJsonValue &json)
{
    const QJsonObject obj = json.toObject();
    //! 兼容旧版本Json文件格式
    if (obj.contains("Name")) {
        displayName = obj["Name"].toString();
        key = obj["Key"].toString();
        lastValue = obj["Value"].toString();
        valueList = convertFromJson<QList<QString>>(obj["Enum"]);
        // 旧版本格式不包括 isRequired 属性
        isRequired = false;
    } else {
        JsonToGadget(json, ScanTaskCustomField::staticMetaObject, this);
    }
    update();
}

QJsonValue ScanTaskCustomField::toJson() const
{
    return GadgetToJson(ScanTaskCustomField::staticMetaObject, this);
}

ScanTaskCustomFieldModel::ScanTaskCustomFieldModel(QObject *parent) :
    Utils::TreeModel<CustomFieldRoot, ScanTaskCustomField>(parent)
{}

void ScanTaskCustomFieldModel::loadJson(const QJsonValue &json)
{
    clear();
    //! 遍历JsonValue中的值，并交给Item去读取
    for (const auto value: json.toArray()) {
        auto *item = new ScanTaskCustomField;
        item->loadJson(value);
        rootItem()->appendChild(item);
    }
}

//! 遍历model中的Jsonvalue并将值返回
QJsonValue ScanTaskCustomFieldModel::toJson() const
{
    QJsonArray json;
    forItemsAtLevel<1>([&] (ScanTaskCustomField *item) {json << item->toJson();});
    return json;
}
