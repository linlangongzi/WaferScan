#include "GeneralDelegate.h"
#include <QComboBox>
#include <QLoggingCategory>
#include <QDoubleSpinBox>
#include <QProgressBar>
#include <QApplication>
#include "MetaType/MetaType.h"

static const char USER_PROPERTY[] = "GadgetPropertyName";
static Q_LOGGING_CATEGORY(debug, "util.delegate");

GeneralDelegate::GeneralDelegate(QObject *obj) :
    QStyledItemDelegate(obj)
{
}

QWidget *GeneralDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qCDebug(debug) << "Creating editor at index" << index << index.data(Qt::EditRole);
    const int typeId = index.data(Qt::EditRole).userType();
    MetaType metaType = MetaType::fromType(typeId);
    if (metaType.hasEditor()) {
        QWidget *widget = metaType.createEditor();
        widget->setProperty(USER_PROPERTY, metaType.editorPropertyName());
        widget->setParent(parent);

        const QVariant additionalProp = index.data(AdditionalPropertyRole);
        if (additionalProp.type() == QVariant::Hash) {
            const QVariantHash hash = additionalProp.toHash();
            for (auto it = hash.begin(); it != hash.end(); ++it) {
                bool ret = widget->setProperty(qPrintable(it.key()), it.value());
                if (!ret) {
                    qCWarning(debug) << "Unhandled widget property" << it.key() << "on widget" << widget;
                }
            }
        }
        return widget;
    }
    DelegateType type = DelegateType(index.data(DelegateRole).toInt());
    switch (type) {
    case None:
        return nullptr;
    case ComboBox: {
        auto *editor = new QComboBox(parent);
        const QStringList enums = index.data(EnumRole).toStringList();
        editor->addItems(enums);
        return editor;
    }
    case SpinBox: {
        auto *editor = new QSpinBox(parent);
        int max = index.data(MaxRole).toInt();
        int min = index.data(MinRole).toInt();
        editor->setRange(min, max);
        return editor;
    }
    case DoubleSpinBox: {
        auto *editor = new QDoubleSpinBox(parent);
        qreal min = index.data(MinRole).toReal();
        qreal max = index.data(MaxRole).toReal();
        editor->setRange(min, max);
        return editor;
    }
    default:
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void GeneralDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const QVariant propertyName = editor->property(USER_PROPERTY);
    if (propertyName.type() == QVariant::String) {
        editor->setProperty(qPrintable(propertyName.toString()), index.data(Qt::EditRole));
        return;
    }
    return QStyledItemDelegate::setEditorData(editor, index);
}

void GeneralDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    const QVariant propertyName = editor->property(USER_PROPERTY);
    if (propertyName.type() == QVariant::String) {
        const QVariant data = editor->property(qPrintable(propertyName.toString()));
        model->setData(index, data, Qt::EditRole);
        return;
    }
    const DelegateType type = DelegateType(index.data(DelegateRole).toInt());
    switch (type) {
    case SpinBox: {
        auto *e = qobject_cast<QSpinBox *>(editor);
        if (e == nullptr) {
            return;
        }
        const qreal value = e->value();
        const QString suffix = index.data(SuffixRole).toString();
        const QString prefix = index.data(PrefixRole).toString();
        model->setData(index, tr("%3%1 %2").arg(value).arg(suffix, prefix), Qt::DisplayRole);
        model->setData(index, value, Qt::EditRole);
        return;
    }
    case DoubleSpinBox: {
        auto *e = qobject_cast<QDoubleSpinBox *>(editor);
        if (e == nullptr) {
            return;
        }
        const qreal value = e->value();
        const QString suffix = index.data(SuffixRole).toString();
        const QString prefix = index.data(PrefixRole).toString();
        model->setData(index, tr("%3%1 %2").arg(value).arg(suffix, prefix), Qt::DisplayRole);
        model->setData(index, value, Qt::EditRole);
        return;
    }
    default:
        return QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void GeneralDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const DelegateType type = DelegateType(index.data(DelegateRole).toInt());
    if (type == ProgressBar) {
        int value = index.data(Qt::EditRole).toInt();
        //进度条的风格选项
        auto *progressBarOption = new QStyleOptionProgressBar();
        progressBarOption->rect = option.rect;
        progressBarOption->direction = QApplication::layoutDirection();
        progressBarOption->fontMetrics = QApplication::fontMetrics();
        progressBarOption->state = QStyle::State_Enabled;
        progressBarOption->minimum = 0;
        progressBarOption->maximum = 100;
        progressBarOption->progress = value;
        progressBarOption->textAlignment = Qt::AlignCenter;
        progressBarOption->text = index.data(Qt::DisplayRole).toString();
        progressBarOption->textVisible = true;
        //绘制进度条
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, progressBarOption, painter);
    } else {
        //否则调用默认委托
        QStyledItemDelegate::paint(painter, option, index);
    }
}
