#include "EditDelegate.h"
#include <QDebug>

EditDelegate::EditDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{}

QWidget *EditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug() << "EditDelegate::createEditor" << index << index.parent();
    if ( index.parent().isValid() && (index.column() == 0) )
    {
        auto *editor = new QLineEdit(parent);
        return editor;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void EditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        QString strName = index.model()->data(index, Qt::DisplayRole).toString();
        auto *lineEdit = qobject_cast<QLineEdit *>(editor);
        lineEdit->setText(strName);
        qDebug() << "EditDelegate::setEditorData" << strName << index << index.model();
        return;
    }
    QStyledItemDelegate::setEditorData(editor, index);
}

void EditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        auto *lineEdit = qobject_cast<QLineEdit *>(editor);
        QString strName = lineEdit->text();
        model->setData( index, QVariant(strName) );
        qDebug() << "EditDelegate::setModelData" << strName << index;
        return;
    }
    QStyledItemDelegate::setModelData(editor, model, index);
}

void EditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
