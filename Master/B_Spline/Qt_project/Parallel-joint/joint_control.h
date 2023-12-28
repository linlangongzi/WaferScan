#ifndef JOINT_CONTROL_H
#define JOINT_CONTROL_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

namespace Ui {
class joint_control;
}

class joint_control : public QMainWindow
{
    Q_OBJECT

public:
    explicit joint_control(QWidget *parent = 0);
    ~joint_control();
private slots:
    void Joint_A_response();
    void Joint_C_response();
    void Encoder_1_response();
    void Encoder_2_response();
    void reset();
    void on_OpenPortButton_clicked();
    void on_SendDataButton_clicked();
    void on_ClosePortButton_clicked();
    void updateReceive();


private:
    Ui::joint_control *ui;
    QSerialPort *mySerialPort;
    QByteArray requestData;
    QTimer *timer;

};

#endif // JOINT_CONTROL_H
