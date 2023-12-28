#include "GadgetListEditor.h"
#include "ui_GadgetListEditor.h"
#include "../Util.h"

GadgetListEditor::GadgetListEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GadgetListEditor)
{
    ui->setupUi(this);
}

GadgetListEditor::~GadgetListEditor()
{
    delete ui;
}

QVariantList GadgetListEditor::values() const
{
    QVariantList values;
    for (int index = 0; index < ui->list->count(); ++index) {
        values << ui->list->item(index)->data(Qt::UserRole);
    }
    return values;
}

QString GadgetListEditor::displayProperty() const
{
    return m_displayProperty;
}

void GadgetListEditor::setValues(const QVariantList &values)
{
    while (values.count() > ui->list->count()) {
        ui->list->addItem(QString());
    }
    while (values.count() < ui->list->count()) {
        delete ui->list->takeItem(ui->list->count() - 1);
    }
    for (int index = 0; index < values.count(); ++index) {
        QListWidgetItem *item = ui->list->item(index);
        item->setData(Qt::UserRole, values[index]);
        refreshTitle(item);
    }
    emit valuesChanged(values);
}

void GadgetListEditor::setDisplayProperty(QString displayProperty)
{
    if (m_displayProperty == displayProperty) {
        return;
    }
    m_displayProperty = displayProperty;
    for (int index = 0; index < ui->list->count(); ++index) {
        QListWidgetItem *item = ui->list->item(index);
        refreshTitle(item);
    }
}

void GadgetListEditor::refreshTitle(QListWidgetItem *item)
{
    QVariant data = item->data(Qt::UserRole);
    QString prop = m_displayProperty;
    if (m_displayProperty.isEmpty()) {
        prop = QMetaType::metaObjectForType(data.userType())->userProperty().name();
    }
    QString title = MetaType::readGadgetProperty(data, prop).toString();
    item->setText(title);
}

void GadgetListEditor::on_add_clicked()
{

}

void GadgetListEditor::on_remove_clicked()
{
    delete ui->list->currentItem();
}
