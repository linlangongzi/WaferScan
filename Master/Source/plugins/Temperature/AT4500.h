#pragma once

#include <QObject>
#include <QSerialPort>
#include <QTime>

class AT4500 : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString portName READ portName WRITE setPortName)

public:
    enum State {
        Uninitialized,
        WaitingFetch,
        WaitingValue,
    };
    Q_ENUM(State)

public:
    AT4500(QObject *parent = Q_NULLPTR);
    QString portName() const;

public slots:
    void setPortName(QString portName);

signals:
    void dataArrived(QList<qreal> data);

private:
    void timerEvent(QTimerEvent *event);

private slots:
    void fetch();
    void onDataReady();
    void onError(QSerialPort::SerialPortError error);
    void setState(State newState);

private:
    QSerialPort *serial;
    QString buffer;
    State state = Uninitialized;
    QTime lastResponse;
    QTime lastFetch;
};
