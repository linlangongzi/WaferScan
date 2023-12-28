#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_cmopen_clicked();
    void updateAA();

    void on_pushButton_send_clicked();

private:
    Ui::Widget *ui;
    QSerialPort *my_serialport;
    QByteArray requestData;
    QTimer *timer;
};

#endif // WIDGET_H
