#include "PanelTrainInfo.h"
#include "ui_PanelTrainInfo.h"
#include <Wheel/ControlPanel.h>

PanelTrainInfo::PanelTrainInfo(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PanelTrainInfo)
{
    ui->setupUi(this);
    connect(controlPanel(), &ControlPanel::trainInfoChanged, this, &PanelTrainInfo::refresh);
}

PanelTrainInfo::~PanelTrainInfo()
{
    delete ui;
}

void PanelTrainInfo::refresh()
{
    const auto info = controlPanel()->getTrainInfo();
    ui->trainId->setText(info.trainId);
    ui->trainType->setText(info.trainType);
}

void PanelTrainInfo::actionModify(bool checked)
{
    ui->trainId->setEnabled(checked);

    if (!checked) {
        const auto control = controlPanel();
        auto info = control->getTrainInfo();
        info.trainId = ui->trainId->text();
        info.trainType = control->trainType(info.trainId);
        ui->trainType->setText(info.trainType);
        control->setTrainInfo(info);
    }
}
