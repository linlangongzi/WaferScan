#pragma once

#include <QStyledItemDelegate>
#include "allrising_global.h"

class ALLRISING_UTILS_EXPORT GeneralDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    enum GeneralRoles {
        AdditionalPropertyRole = Qt::UserRole + 9,
        DelegateRole = Qt::UserRole + 10,
        EnumRole = Qt::UserRole + 11,
        MinRole = Qt::UserRole + 12,
        MaxRole = Qt::UserRole + 13,
        SuffixRole = Qt::UserRole + 14,
        PrefixRole = Qt::UserRole + 15,
    };
    Q_ENUM(GeneralRoles)

    enum DelegateType {
        Default,
        None,
        ComboBox,
        SpinBox,
        DoubleSpinBox,
        ProgressBar,
    };
    Q_ENUM(DelegateType)

    GeneralDelegate(QObject *obj = nullptr);

public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
