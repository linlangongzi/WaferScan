#include "PanelUTSetupTimebase.h"
#include "ui_PanelUTSetupTimebase.h"
#include "Platform/Platform.h"
//#include "ConfigObject/Material.h"
#include "ConfigObject/UTSetup.h"
#include "PanelMaterial/DialogMaterial.h"
#include <QDebug>

PanelUTSetupTimebase::PanelUTSetupTimebase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelUTSetupTimebase)
{
    ui->setupUi(this);
    InitUI();
}

PanelUTSetupTimebase::~PanelUTSetupTimebase()
{
    delete ui;
}

void PanelUTSetupTimebase::ConnectTo(QObject *obj)
{
    Disconnect(obj);
    setup = dynamic_cast<UTSetup *>(obj);
    if (setup.isNull())
    {
        return;
    }
    connect(setup, SIGNAL(ValueChangedSetup(QString, QJsonValue)), this, SLOT(OnParameterChanged(QString, QJsonValue)));
    connect(setup, SIGNAL(ParameterLimit(QString, QJsonValue)), this, SLOT(OnParameterChanged(QString, QJsonValue)));
    connect(this, SIGNAL(ParameterValueChanged(QString,QJsonValue)), setup, SLOT(SetParameter(QString,QJsonValue)));
    OnParameterChanged("Delay", setup->Get(setup->GetWaveType().append("Delay")));
    OnParameterChanged("Range", setup->Get("Range"));
    OnParameterChanged("DelayUnit", setup->Get("DelayUnit"));
    OnParameterChanged("RangeUnit", setup->Get("RangeUnit"));
    OnParameterChanged("MaterialID", setup->Get("MaterialID"));
    ui->materialSpeed->ConnectTo(setup);
}

void PanelUTSetupTimebase::Disconnect(QObject *obj)
{
    Q_UNUSED(obj);
    if (setup)
    {
        disconnect(setup, 0, this, 0);
        disconnect(this, 0, setup, 0);
        ui->materialSpeed->Disconnect(setup);
    }
    setup.clear();
}

void PanelUTSetupTimebase::OnParameterChanged(const QString &key, const QJsonValue &value)
{
    if (key == "Delay")
    {
        ui->delayTime->setValue(value.toDouble());
    }
    else if (key == "Range")
    {
        ui->rangeTime->setValue(value.toDouble());
    }
    else if (key == "DelayUnit")
    {
        QString delayPreference = value.toString("TIME");
        ui->delayPreferTime->setChecked(delayPreference == "TIME");
        ui->delayPreferDistance->setChecked(delayPreference != "TIME");
    }
    else if (key == "RangeUnit")
    {
        QString rangePreference = value.toString("TIME");
        ui->rangePreferTime->setChecked(rangePreference == "TIME");
        ui->rangePreferDistance->setChecked(rangePreference != "TIME");
    }
    else if (key == "Velocity")
    {
        ui->materialSpeed->setValue(value.toInt());
    }
    else if (key == "MaterialID")
    {
        UpdateMaterial();
    }
}

void PanelUTSetupTimebase::UpdateMaterial()
{
    if (setup)
    {
        ui->materialName->setText(GetMaterialName());
        ui->materialSpeed->setValue(GetMaterialSpeed());
        if (ui->delayPreferDistance->isChecked())
        {
            ui->delayTime->setValue(ToTime(ui->delayDistance->value()));
            on_delayTime_editingFinished();
        }
        else
        {
            ui->delayDistance->setValue(ToDistance(ui->delayTime->value()));
            on_delayDistance_editingFinished();
        }
        if (ui->rangePreferDistance->isChecked())
        {
            ui->rangeTime->setValue(ToTime(ui->rangeDistance->value()));
            on_rangeTime_editingFinished();
        }
        else
        {
            ui->rangeDistance->setValue(ToDistance(ui->rangeTime->value()));
            on_rangeDistance_editingFinished();
        }
    }
}

void PanelUTSetupTimebase::InitUI()
{
    connect(ui->delayDistance, SIGNAL(valueChanged(double)), this, SLOT(EmitNewDelayDistance(double)));
    connect(ui->delayTime, SIGNAL(valueChanged(double)), this, SLOT(EmitNewDelayTime(double)));
    connect(ui->rangeDistance, SIGNAL(valueChanged(double)), this, SLOT(EmitNewRangeDistance(double)));
    connect(ui->rangeTime, SIGNAL(valueChanged(double)), this, SLOT(EmitNewRangeTime(double)));
    ui->materialSpeed->SetParameterKey("Velocity");
}

double PanelUTSetupTimebase::GetMaterialSpeed()
{
    return setup ? setup->GetMaterialSpeed() : 1000;
}

QString PanelUTSetupTimebase::GetMaterialName()
{
    return setup ? setup->GetMaterialName() : QString();
}

double PanelUTSetupTimebase::ToDistance(double time)
{
    return time * GetMaterialSpeed() * 0.5e-3;
}

double PanelUTSetupTimebase::ToTime(double distance)
{
    return distance * 2e3 / GetMaterialSpeed();
}

void PanelUTSetupTimebase::on_delayTime_valueChanged(double arg1)
{
    ui->delayDistance->setValue(ToDistance(arg1));
}

void PanelUTSetupTimebase::on_delayDistance_valueChanged(double arg1)
{
    ui->delayTime->setValue(ToTime(arg1));
}

void PanelUTSetupTimebase::on_rangeTime_valueChanged(double arg1)
{
    ui->rangeDistance->setValue(ToDistance(arg1));
}

void PanelUTSetupTimebase::on_rangeDistance_valueChanged(double arg1)
{
    ui->rangeTime->setValue(ToTime(arg1));
}

void PanelUTSetupTimebase::on_delayTime_editingFinished()
{
    emit RequestSetParameter("Delay", ui->delayTime->value());
}

void PanelUTSetupTimebase::on_delayDistance_editingFinished()
{
    emit RequestSetParameter("Delay", ToTime(ui->delayDistance->value()));
}

void PanelUTSetupTimebase::on_rangeTime_editingFinished()
{
    emit RequestSetParameter("Range", ui->rangeTime->value());
}

void PanelUTSetupTimebase::on_rangeDistance_editingFinished()
{
    emit RequestSetParameter("Range", ToTime(ui->rangeDistance->value()));
}

void PanelUTSetupTimebase::on_changeMaterial_clicked()
{
    DialogMaterial dialog;
    if (dialog.exec() == QDialog::Accepted)
    {
        Material *material = dialog.GetMaterial();
        if (material)
        {
            emit RequestSetParameter("MaterialID", Config::FromObjectID(material->GetID()));
        }
    }
}

void PanelUTSetupTimebase::EmitNewDelayTime(double delay)
{
    emit ParameterValueChanged("Delay", delay);
}

void PanelUTSetupTimebase::EmitNewDelayDistance(double delay)
{
    emit ParameterValueChanged("Delay", ToTime(delay));
}

void PanelUTSetupTimebase::EmitNewRangeTime(double range)
{
    emit ParameterValueChanged("Range", range);
}

void PanelUTSetupTimebase::EmitNewRangeDistance(double range)
{
    emit ParameterValueChanged("Range", ToTime(range));
}
