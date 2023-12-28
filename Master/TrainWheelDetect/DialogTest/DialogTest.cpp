#include "DialogTest.h"
#include "ui_DialogTest.h"
#include <QTimer>
#include <arsCore/Platform.h>
#include "WheelDataset/WheelDataset.h"
#include <Ultrasonic/UTInstrument.h>
#include <QFileDialog>
#include <Core/DocumentManager.h>

DialogTest::DialogTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTest)
{
    ui->setupUi(this);
    m_pTimer = new TimerTest(this);
    m_pTimer->stop();
}

DialogTest::~DialogTest()
{
    delete ui;
}

void DialogTest::on_pushButton_clicked()
{
    if(m_pTimer->isActive())
    {
        m_pTimer->stop();
        ui->pushButton->setText("开始测试");
        ui->spinBoxInterval->setEnabled(true);
    }
    else
    {
        int nInterval = ui->spinBoxInterval->value() * 1000;
        m_pTimer->Start(nInterval);
        ui->pushButton->setText("结束测试");
        ui->spinBoxInterval->setEnabled(false);
    }
}

TimerTest* DialogTest::GetTimer()
{
    return m_pTimer;
}

void DialogTest::on_repeatFrequence_valueChanged(int arg1)
{
    for (auto instrument: platform()->Instruments()->GetRegistry<UTInstrument>()) {
        instrument->SetRepeatFrequence(arg1);
    }
}

void DialogTest::on_pushButtonSelfTest_clicked()
{
    emit SelfTestConfirm(ui->spinBoxSensorNo->value());
}

void DialogTest::on_btnReadResult_clicked()
{
    QString openPath = QFileDialog::getOpenFileName(this,
                                                    tr("open Result data"),
                                                    tr(""),
                                                    "open File(*.txt)");
    DocumentManager::setCurrentDocument(QString());
}
