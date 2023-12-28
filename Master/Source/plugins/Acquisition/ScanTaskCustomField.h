#pragma once

#include <utils/treemodel.h>

//! 编辑字段Tab页所需Model
class ScanTaskCustomField : public Utils::TreeItem
{
    Q_GADGET

    Q_PROPERTY(QString displayName MEMBER displayName)
    Q_PROPERTY(QString key MEMBER key)
    Q_PROPERTY(QStringList valueList MEMBER valueList)
    Q_PROPERTY(QString lastValue MEMBER lastValue)
    Q_PROPERTY(bool isRequired MEMBER isRequired)

public:
    enum Columns {
        Required,
        Key,
        Name,
        Value
    };
    Q_ENUM(Columns)

    QVariant data(int column, int role) const override;
    bool setData(int column, const QVariant &data, int role) override;
    Qt::ItemFlags flags(int column) const override;

    void loadJson(const QJsonValue &json);
    QJsonValue toJson() const;

    QString displayName;
    QString key;
    QStringList valueList;
    QString lastValue;
    bool isRequired = false;
};

using CustomFieldRoot = Utils::TypedTreeItem<ScanTaskCustomField>;

class ScanTaskCustomFieldModel : public Utils::TreeModel<CustomFieldRoot, ScanTaskCustomField>
{
    Q_OBJECT

public:
    ScanTaskCustomFieldModel(QObject *parent = nullptr);

    void loadJson(const QJsonValue &json);
    QJsonValue toJson() const;
};
