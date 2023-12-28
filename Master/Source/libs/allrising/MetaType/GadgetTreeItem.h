#pragma once

#include "../allrising_global.h"
#include <QCoreApplication>
#include <utils/treemodel.h>

class ALLRISING_UTILS_EXPORT GadgetTreeItem : public Utils::TypedTreeItem<GadgetTreeItem, GadgetTreeItem>
{
    Q_DECLARE_TR_FUNCTIONS(GadgetTreeItem)

public:
    GadgetTreeItem(int typeId = QMetaType::UnknownType);
    GadgetTreeItem(const QVariant &value);

    QVariant data(int column, int role) const override;
    bool setData(int column, const QVariant &data, int role) override;
    Qt::ItemFlags flags(int column) const override;

    bool canFetchMore() const override;
    void fetchMore() override;

    virtual QVariant toDisplayValue(int role, const QString &key, const QVariant &value) const;
    virtual QVariant fromDisplayValue(int role, const QString &key, const QVariant &value) const;
    virtual QVariantHash editorProperty(const QString &key) const;

    virtual QVariant value() const;
    virtual bool setValue(const QVariant &newValue);

    template <typename T>
    T value() const
    {
        return value().value<T>();
    }

    template <typename T>
    bool setValue(const T &newValue)
    {
        return setValue(QVariant::fromValue(newValue));
    }

    QVariant property(const QVariant &key) const;
    bool setProperty(const QVariant &key, const QVariant &propertyValue);

    virtual QVariant status(int role) const;

protected:
    enum ValueType {
        ValueSimple,
        ValueGadget,
        ValueList,
        ValueMap,
    };

    ValueType valueType() const;
    QString parentKey() const;
    inline GadgetTreeItem *parentItem() const
    {
        return static_cast<GadgetTreeItem *>(parent());
    }

    QVariant m_value;
    bool isParentProperty = false;
    bool childrenCreated = false;
    QVariantHash m_editorProperty;
};
