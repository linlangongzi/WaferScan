#pragma once

#include <QStyledItemDelegate>
#include <QLineEdit>

class EditDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit EditDelegate(QObject *parent = nullptr);

signals:

public slots:

public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
