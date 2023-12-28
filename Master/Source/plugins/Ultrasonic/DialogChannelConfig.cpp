#include "DialogChannelConfig.h"
#include "InstrumentModel.h"
#include "ui_DialogChannelConfig.h"

DialogChannelConfig::DialogChannelConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChannelConfig)
{
    ui->setupUi(this);
    ui->instrument->setModel(new InstrumentModel(this));
}

DialogChannelConfig::~DialogChannelConfig()
{
    delete ui;
}

ChannelConfig DialogChannelConfig::config() const
{
    ChannelConfig config;
    config.channelId = ui->channelId->value();
    config.name = ui->name->text();
    config.identifier.instrument = ui->instrument->currentData(Qt::EditRole).toString();
    config.identifier.channel = ui->channel->value();
    config.identifier.cycle = ui->cycle->value();
    return config;
}

void DialogChannelConfig::setConfig(ChannelConfig config)
{
    ui->channelId->setValue(config.channelId);
    ui->name->setText(config.name);
    int index = ui->instrument->findData(config.identifier.instrument, Qt::EditRole);
    ui->instrument->setCurrentIndex(index);
    ui->channel->setValue(config.identifier.channel);
    ui->cycle->setValue(config.identifier.cycle);
}
