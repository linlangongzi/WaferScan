﻿#include "SerialPort.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "ui_SerialPort.h"
#include <QDebug.h>
#include <QTimer>

SerialPort::SerialPort(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialPort)
{
    ui->setupUi(this);

    ui->pushButtonCloseSerialPort->setEnabled(0);
    ui->pushButtonSend->setEnabled(0);

    my_serialport= new QSerialPort();

    timerForPort = new QTimer(this);    //未打开串口时，刷新端口
    timerForPort->start(200);           //200ms中断一次


    //信号与槽连接
    connect(timerForPort, SIGNAL(timeout()), this, SLOT(updateSerialPortList()));
    connect(ui->pushButtonSend, SIGNAL(clicked(bool)), this, SLOT(sendData()));
    connect(ui->pushButtonClear, SIGNAL(clicked(bool)), ui->textBrowserReceive, SLOT(clear()));
    connect(my_serialport, SIGNAL(readyRead()), this, SLOT(updateReceive()));   //串口收到接收中断即去接收信息

}

SerialPort::~SerialPort()
{
    delete ui;
}

/*!
 * \brief SerialPort::sendData      配合发送文字编辑窗口
 * \param str
 */
void SerialPort::sendData()
{
    if (my_serialport) {
        QString temp0 = ui->textEditSendData->toPlainText();
        QStringList temp1 = temp0.split(" ", QString::SkipEmptyParts);  //将发送栏中数据变换成QStringList
        QByteArray data(temp1.size(), '0');
        //将QStringList转换成uchar []
//        uchar serialData[temp1.size()];
        for (int i = 0; i < temp1.size(); i++){
//            bool ok = false;
            data[i] = char(temp1[i].toInt(NULL, 16));    //窗口中的数据为16进制，因此转换时要设置16进制
        }
        qDebug() << data;
        if (my_serialport->isWritable()){
            my_serialport->write(data);
            //qDebug()<< "send data is: " << QString::number(serialData[2]);
        }

    }

}

void SerialPort::sendData(const char *data)
{
    if (data != NULL && my_serialport && my_serialport->isWritable()) {
        my_serialport->write(data);
    }
}
/*!
 * \brief SerialPort::updateReceive     接收串口中断
 */
void SerialPort::updateReceive()
{
    QByteArray requestData = my_serialport->readAll();
    if (!requestData.isEmpty()){
        ui->textBrowserReceive->append(QString(requestData));
    }
}
/*!
 * \brief SerialPort::updateSerialPortList      更新串口列表：可能会导致在一开始选择串口号时出现问题，无法选中，因为会清空
 */
void SerialPort::updateSerialPortList()
{
    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    // 清除多余的串口名称
    for (int index = 0; index < ui->comboBoxPortName->count(); ++index) {
        bool isValid = false;
        foreach(const QSerialPortInfo &info, serialList) {
            if (ui->comboBoxPortName->itemText(index) == info.portName()) {
                isValid = true;
                break;
            }
        }
        if (isValid == false) {
            ui->comboBoxPortName->removeItem(index);
        }
    }
    //添加未登记的串口名称
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())            // 列出所有可用的串口引脚
    {
//        qDebug() << "Name        : " << info.portName();
//        qDebug() << "Description : " << info.description();
//        qDebug() << "Manufacturer: " << info.manufacturer();

        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            if(ui->comboBoxPortName->findText(info.portName()) == -1) {
                ui->comboBoxPortName->addItem(info.portName());
            }
            serial.close();
        }
    }
}


void SerialPort::on_pushButtonOpenSerialPot_clicked()
{
    //设置串口协议
    qDebug()<<ui->comboBoxPortName->currentText();
    my_serialport->setPortName(ui->comboBoxPortName->currentText());
    my_serialport->open(QIODevice::ReadWrite);
    my_serialport->setBaudRate(ui->comboBoxBautRate->currentText().toInt());
    my_serialport->setDataBits(QSerialPort::Data8);
    ui->comboBoxDataBits->setCurrentText(QString::number(QSerialPort::Data8));
    my_serialport->setParity(QSerialPort::NoParity);
    ui->comboBoxParity->setCurrentText(QString::number(QSerialPort::NoParity));
    my_serialport->setStopBits(QSerialPort::OneStop);
    ui->comboBoxStopBits->setCurrentText(QString::number(QSerialPort::OneStop));
    my_serialport->setFlowControl(QSerialPort::NoFlowControl);

    if (my_serialport->isOpen()){
        qDebug()<< "串口打开成功！";
        emit serialPortIsOpen(true);
        ui->pushButtonCloseSerialPort->setEnabled(1);          //打开串口后，才可使用关闭串口
        ui->pushButtonSend->setEnabled(1);
        ui->pushButtonOpenSerialPot->setEnabled(0);            //打开串口后，不可再次打开
        timerForPort->stop();
    }else {
        qDebug() << "串口打开失败";
        ui->textBrowserReceive->clear();
        QString failOpen("fail to open the serial port!");
        ui->textBrowserReceive->setText(failOpen);
    }
}

void SerialPort::on_pushButtonCloseSerialPort_clicked()
{
    if (my_serialport->isOpen()){
        my_serialport->close();
        ui->pushButtonOpenSerialPot->setEnabled(1);
        ui->pushButtonCloseSerialPort->setEnabled(0);
        ui->pushButtonSend->setEnabled(0);
        qDebug()<< "串口关闭";
        emit serialPortIsOpen(false);
        timerForPort->start(200);
    }
}
