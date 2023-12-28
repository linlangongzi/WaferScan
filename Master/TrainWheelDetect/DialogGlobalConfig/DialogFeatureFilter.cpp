#include "DialogFeatureFilter.h"
#include "ui_DialogFeatureFilter.h"

DialogFeatureFilter::DialogFeatureFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFeatureFilter)
{
    ui->setupUi(this);

    for (auto value: EnumValueList<FeatureType>()) {
        ui->feature->addItem(EnumToKey(value), value);
    }
    ui->feature->setCurrentIndex(ui->feature->findData(FeatureType::GateATime));
}

DialogFeatureFilter::~DialogFeatureFilter()
{
    delete ui;
}

void DialogFeatureFilter::setConfig(FeatureFilterConfig config)
{
    if (config.featureType < ui->feature->count()) {
        ui->feature->setCurrentIndex(config.featureType);
    } else {
        ui->feature->addItem(EnumToKey(config.featureType));
        ui->feature->setCurrentIndex(ui->feature->findData(config.featureType));
    }

    ui->min->setValue(config.min);
    ui->max->setValue(config.max);
}

FeatureFilterConfig DialogFeatureFilter::config() const
{
    FeatureFilterConfig filter;
    filter.featureType = ui->feature->currentData().value<FeatureType>();
    filter.min = ui->min->value();
    filter.max = ui->max->value();

    return filter;
}

void DialogFeatureFilter::on_feature_currentIndexChanged(int index)
{
    QString unit = " %";
    if (index % 2 == 0) {
        unit = " us";
    }

    ui->min->setSuffix(unit);
    ui->max->setSuffix(unit);
}
