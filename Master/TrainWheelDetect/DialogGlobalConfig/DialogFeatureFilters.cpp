#include "DialogFeatureFilter.h"
#include "DialogFeatureFilters.h"
#include "ui_DialogFeatureFilters.h"

DialogFeatureFilters::DialogFeatureFilters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFeatureFilters)
{
    ui->setupUi(this);
}

DialogFeatureFilters::~DialogFeatureFilters()
{
    delete ui;
}

void DialogFeatureFilters::setConfig(QList<FeatureFilterConfig> filters)
{
    ui->filters->clear();
    for (auto filter: filters) {
        QListWidgetItem *item = new QListWidgetItem;
        QString text = tr("%1(%2,%3)").arg(EnumToKey(filter.featureType)).arg(filter.min).arg(filter.max);
        item->setText(text);
        item->setData(Qt::UserRole, QVariant::fromValue(filter));
        ui->filters->addItem(item);
    }
}

QList<FeatureFilterConfig> DialogFeatureFilters::config() const
{
    QList<FeatureFilterConfig> filters;
    int filterCount = ui->filters->count();
    for (int index = 0; index < filterCount; ++index) {
        filters << ui->filters->item(index)->data(Qt::UserRole).value<FeatureFilterConfig>();
    }

    return filters;
}

void DialogFeatureFilters::on_add_clicked()
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(tr("new config"));
    item->setData(Qt::UserRole, QVariant::fromValue(FeatureFilterConfig()));
    ui->filters->addItem(item);
}

void DialogFeatureFilters::on_remove_clicked()
{
    delete ui->filters->currentItem();
}

void DialogFeatureFilters::on_filters_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item) {
        return;
    }

    DialogFeatureFilter dlg;
    dlg.setConfig(item->data(Qt::UserRole).value<FeatureFilterConfig>());
    if (dlg.exec() == QDialog::Accepted) {
        FeatureFilterConfig config = dlg.config();
        item->setData(Qt::UserRole, QVariant::fromValue(config));
        item->setText(tr("%1(%2,%3)").arg(EnumToKey(config.featureType)).arg(config.min).arg(config.max));
    }
}
