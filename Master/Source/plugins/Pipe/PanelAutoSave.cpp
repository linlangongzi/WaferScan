#include "PanelAutoSave.h"
#include "ui_PanelAutoSave.h"

PanelAutoSave::PanelAutoSave(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelAutoSave),
    autoTime(20000)
{
    ui->setupUi(this);
    Connect();
}

PanelAutoSave::~PanelAutoSave()
{
    delete ui;
}

quint32 PanelAutoSave::GetTime() const
{
    return autoTime;
}

void PanelAutoSave::Show()
{
    ui->lineEditTime->setText(QString("%1").arg(autoTime));
    show();
}

void PanelAutoSave::Yes()
{
    quint32 timer = ui->lineEditTime->text().toInt();
    if ( (timer >= 200) && (timer != autoTime) ) {
        autoTime = timer;
        emit TimerChanged(autoTime);
    }
    close();
}

void PanelAutoSave::Cancel()
{
    close();
}

void PanelAutoSave::Connect()
{
    connect(ui->pushButtonYes, SIGNAL(clicked()), this, SLOT(Yes()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(Cancel()));
}
