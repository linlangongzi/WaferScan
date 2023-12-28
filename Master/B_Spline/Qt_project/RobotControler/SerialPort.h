#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QDialog>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class SerialPort;
}

class SerialPort : public QDialog
{
    Q_OBJECT

public:
    explicit SerialPort(QWidget *parent = 0);
    ~SerialPort();

signals:
    void serialPortIsOpen(bool IsOpen);

public slots:
    void sendData();                                                            /* 测试程序 */
    void sendData(const char *data);


    void updateReceive();
    void updateSerialPortList();


private slots:
    void on_pushButtonOpenSerialPot_clicked();
    void on_pushButtonCloseSerialPort_clicked();

private:
    Ui::SerialPort *ui;
    QSerialPort *my_serialport;
    QTimer *timerForPort;

};

#endif // SERIALPORT_H
