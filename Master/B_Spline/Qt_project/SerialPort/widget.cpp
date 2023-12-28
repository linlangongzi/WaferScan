#include "widget.h"
#include "ui_widget.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        // Example use QSerialPort
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox->addItem(info.portName());
            serial.close();
        }
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_cmopen_clicked()
{
    my_serialport= new QSerialPort();
    qDebug()<<ui->comboBox->currentText();
    my_serialport->setPortName(ui->comboBox->currentText());//设置
    my_serialport->open(QIODevice::ReadWrite);
    qDebug()<<ui->lineEdit->text().toInt();
    my_serialport->setBaudRate(ui->lineEdit->text().toInt());
    my_serialport->setDataBits(QSerialPort::Data8);
    my_serialport->setParity(QSerialPort::NoParity);
    my_serialport->setStopBits(QSerialPort::OneStop);
    my_serialport->setFlowControl(QSerialPort::NoFlowControl);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateAA()));
    timer->start(10);//相当于一个循环
}

void Widget::updateAA()
{
    requestData = my_serialport->readAll();

    if(!requestData.isEmpty())//requestData!= NULL)
    {
        ui->textEdit->append(requestData);
    }
    requestData.clear();
}

void Widget::on_pushButton_send_clicked()
{
    QByteArray TxData = "aaaa";
    my_serialport->write(TxData);
}
