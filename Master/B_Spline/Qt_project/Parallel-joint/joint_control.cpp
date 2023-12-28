#include "joint_control.h"
#include "ui_joint_control.h"
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>

joint_control::joint_control(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::joint_control)
{
    ui->setupUi(this);
    //将可用的串口显示在下拉列表中
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        QSerialPort serial_temp;
        serial_temp.setPort(info);
        if(serial_temp.open(QIODevice::ReadWrite))
        {
            ui->PortNameCom->addItem(info.portName());
            serial_temp.close();
        }
    }
    //将可用的波特率显示在下拉列表中
//    foreach (const qint32 &baud, QSerialPort::) {
        ui->BaudRateCom->addItem("1200");
        ui->BaudRateCom->addItem("2400");
        ui->BaudRateCom->addItem("4800");
        ui->BaudRateCom->addItem("9600");
        ui->BaudRateCom->addItem("19200");
        ui->BaudRateCom->addItem("38400");
        ui->BaudRateCom->addItem("57600");
        ui->BaudRateCom->addItem("115200");


    ui->SendDataButton->setDisabled(true);//打串口前屏蔽发送数据和关闭串口功能
    ui->ClosePortButton->setDisabled(true);
    connect(ui->Goal_A,SIGNAL(editingFinished()),this,SLOT(Joint_A_response()));
    connect(ui->Goal_C,SIGNAL(editingFinished()),this,SLOT(Joint_C_response()));
    connect(ui->Goal_Encoder_1,SIGNAL(editingFinished()),this,SLOT(Encoder_1_response()));
    connect(ui->Goal_Encoder_2,SIGNAL(editingFinished()),this,SLOT(Encoder_2_response()));
    connect(ui->clearButton,SIGNAL(clicked(bool)),this,SLOT(reset()));
}

joint_control::~joint_control()
{
    delete ui;
}

void joint_control::Joint_A_response()
{
    double temp1 = ui->Goal_A->text().toDouble() + ui->Goal_C->text().toDouble();
    double temp2 = ui->Goal_A->text().toDouble() - ui->Goal_C->text().toDouble();
    QString a = QString("%1").arg(temp1);//QString::number(temp1,10);
    QString b = QString("%1").arg(temp2);//QString::number(temp2,10);
    ui->Goal_Encoder_1->setText(a);
    ui->Goal_Encoder_2->setText(b);
}
void joint_control::Joint_C_response()
{
    double temp1 = ui->Goal_A->text().toDouble() + ui->Goal_C->text().toDouble();
    double temp2 = ui->Goal_A->text().toDouble() - ui->Goal_C->text().toDouble();
    QString a = QString("%1").arg(temp1);//QString::number(temp1,10);
    QString b = QString("%1").arg(temp2);//QString::number(temp2,10);
    ui->Goal_Encoder_1->setText(a);
    ui->Goal_Encoder_2->setText(b);
}
void joint_control::Encoder_1_response()
{
    double temp1 = 0.5 * ui->Goal_Encoder_1->text().toDouble() + 0.5* ui->Goal_Encoder_2->text().toDouble();
    double temp2 = 0.5 * ui->Goal_Encoder_1->text().toDouble() - 0.5* ui->Goal_Encoder_2->text().toDouble();
    QString a = QString("%1").arg(temp1);//QString::number(temp1,10);
    QString b = QString("%1").arg(temp2);//QString::number(temp2,10);
    ui->Goal_A->setText(a);
    ui->Goal_C->setText(b);
}
void joint_control::Encoder_2_response()
{
    double temp1 = 0.5 * ui->Goal_Encoder_1->text().toDouble() + 0.5* ui->Goal_Encoder_2->text().toDouble();
    double temp2 = 0.5 * ui->Goal_Encoder_1->text().toDouble() - 0.5* ui->Goal_Encoder_2->text().toDouble();
    QString a = QString("%1").arg(temp1);//QString::number(temp1,10);
    QString b = QString("%1").arg(temp2);//QString::number(temp2,10);
    ui->Goal_A->setText(a);
    ui->Goal_C->setText(b);
}
void joint_control::reset()
{
    ui->Goal_A->setText("0");
    ui->Goal_C->setText("0");
    ui->Goal_Encoder_1->setText("0");
    ui->Goal_Encoder_2->setText("0");
    ui->Joint_A->setText("0");
    ui->Joint_C->setText("0");
    ui->Encoder_1->setText("0");
    ui->Encoder_2->setText("0");
}
void joint_control::on_OpenPortButton_clicked()
{
    if(!ui->PortNameCom->currentText().isEmpty())
    {
        mySerialPort = new QSerialPort();
        qDebug()<<"serial port "<<ui->PortNameCom->currentText()<<"is open";
        mySerialPort->setPortName(ui->PortNameCom->currentText());
        mySerialPort->open(QIODevice::ReadWrite);
        //若未设置波特率则默认为9600
        int a = ui->BaudRateCom->currentText().toInt();
        mySerialPort->setBaudRate(a==0?9600:a);
        qDebug()<<"baudtate is "<<mySerialPort->baudRate();
        mySerialPort->setDataBits(QSerialPort::Data8);
        mySerialPort->setParity(QSerialPort::NoParity);
        mySerialPort->setStopBits(QSerialPort::OneStop);
        mySerialPort->setFlowControl(QSerialPort::NoFlowControl);
        timer = new QTimer(this);
        ui->SendDataButton->setDisabled(false);//打开串口后使能发送数据和关闭串口功能
        ui->ClosePortButton->setDisabled(false);
        ui->OpenPortButton->setDisabled(true);//打开串口后屏蔽打开串口功能
        connect(timer,SIGNAL(timeout()),this,SLOT(updateReceive()));
        timer->start(10);//10ms接收更新一次
    }
    else
        ui->SerialReveiveData->setText("can not open the serial port!");
}
void joint_control::updateReceive()
{
    requestData = mySerialPort->readAll();
    if(!requestData.isEmpty())
    {
        ui->SerialReveiveData->append(requestData);
    }
    requestData.clear();
}
void joint_control::on_ClosePortButton_clicked()
{
    mySerialPort->close();
    timer->stop();
    ui->SerialReveiveData->clear();
    ui->ClosePortButton->setDisabled(true);
    ui->SendDataButton->setDisabled(true);
    ui->OpenPortButton->setDisabled(false);
    qDebug()<<"close the serial port";
}
void joint_control::on_SendDataButton_clicked()
{
    QString a = ui->SerialSendData->toPlainText();
    qDebug()<<"the send data is "<< ui->SerialSendData->toPlainText();
    QByteArray TxData = a.toLatin1();
    mySerialPort->write(TxData);
}
