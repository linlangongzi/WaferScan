#include "PanelTCGEdit.h"
#include "ui_PanelTCGEdit.h"

const double PanelTCGEdit::MIN_TIME_VALUE = 0.0;
const double PanelTCGEdit::MAX_TIME_VALUE = 999999.0;
const double PanelTCGEdit::MAX_GAIN_VALUE = 80.0;
const double PanelTCGEdit::MIN_GAIN_VALUE = 0.0;

PanelTCGEdit::PanelTCGEdit(UTSetup *setup, int index,  bool bModify, QWidget *parent) :
    QDialog(parent),
    modelIndex(index),
    ui(new Ui::PanelTCGEdit)
{
    ui->setupUi(this);
    setWindowTitle(tr("TCGEditDialog"));
    dTCGTimeValidator = new QDoubleValidator(MIN_TIME_VALUE, MAX_TIME_VALUE, 10, this);  // 10为进制的格式
    ui->timeLineEdit->setValidator(dTCGTimeValidator);
    dTCGGainValidator = new QDoubleValidator(MIN_GAIN_VALUE, MAX_GAIN_VALUE, 10, this);
    ui->gainLineEdit->setValidator(dTCGGainValidator);
    if (bModify)
    {
        QJsonArray TCGArray;
        if (setup)
        {
            TCGArray = setup->GetTCGJsonArray();
        }
        ui->timeLineEdit->setText(QString::number(TCGArray[modelIndex].toObject()["Time"].toDouble(), 'f', 2));
        ui->gainLineEdit->setText(QString::number(TCGArray[modelIndex].toObject()["Gain"].toDouble(), 'f', 2));
    }
}

PanelTCGEdit::~PanelTCGEdit()
{
    if (dTCGTimeValidator)
    {
        delete dTCGTimeValidator;
        dTCGTimeValidator = NULL;
    }
    if (dTCGGainValidator)
    {
        delete dTCGGainValidator;
        dTCGGainValidator = NULL;
    }
    delete ui;
}

double PanelTCGEdit::GetTCGTimeValue()
{
    return timeValue;
}

double PanelTCGEdit::GetTCGGainValue()
{
    return gainValue;
}

void PanelTCGEdit::on_saveBtn_clicked()
{
    if (ui->timeLineEdit->text() == "")
    {
        QMessageBox::warning(this, tr("Warning"), tr("The value can't be null!"));
        return;
    }
    if (ui->gainLineEdit->text() == "")
    {
        QMessageBox::warning(this, tr("Warning"), tr("The value can't be null!"));
        return;
    }
    timeValue = ui->timeLineEdit->text().toDouble();
    gainValue = ui->gainLineEdit->text().toDouble();
    accept();
}

void PanelTCGEdit::on_cancelBtn_clicked()
{
    close();
}
