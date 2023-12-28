#include "PanelInformation.h"
#include "ui_PanelInformation.h"

PanelInformation::PanelInformation(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PanelInformation),
    timerStartId(0),
    timerEndId(0)
{
    ui->setupUi(this);

    ui->lineEditInfo->setStyleSheet("color:green");
    ui->lineEditInfo->setFont(QFont( "Timers" , 17.5 ,  QFont::Bold));
}

PanelInformation::~PanelInformation()
{
    delete ui;
}

void PanelInformation::OnScanStart()
{
    setVisible(true);
    ui->lineEditInfo->setText(tr("列车进入,开始扫查"));
    timerStartId = startTimer(1000);
}

void PanelInformation::timerEvent(QTimerEvent *e)
{
    killTimer(e->timerId());
    if(e->timerId() == timerStartId)
    {
        timerStartId = -1;
        ui->lineEditInfo->setText(tr("扫查中。。。"));
    }
    else if(e->timerId() == timerEndId)
    {
        timerEndId = -1;
        setVisible(false);
    }
}

void PanelInformation::OnScanEnd()
{
    setVisible(true);
    ui->lineEditInfo->setText(tr("列车驶离,扫查结束"));
    timerEndId = startTimer(1500);
}
