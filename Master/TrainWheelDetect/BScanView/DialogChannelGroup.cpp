#include "DialogChannelGroup.h"
#include "ui_DialogChannelGroup.h"

DialogChannelGroup::DialogChannelGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChannelGroup)
{
    ui->setupUi(this);
}

DialogChannelGroup::~DialogChannelGroup()
{
    delete ui;
}

void DialogChannelGroup::setConfig(const ChannelGroup &config)
{
    ui->name->setText(config.name);
    ui->start->setValue(config.start);
    ui->end->setValue(config.end);
    ui->interval->setValue(config.interval);
    ui->view->setValue(config.view);
}

ChannelGroup DialogChannelGroup::config()
{
    ChannelGroup groupConfig;
    groupConfig.name = ui->name->text();
    groupConfig.start = ui->start->value();
    groupConfig.end = ui->end->value();
    groupConfig.interval = ui->interval->value();
    groupConfig.view = ui->view->value();

    return groupConfig;
}
