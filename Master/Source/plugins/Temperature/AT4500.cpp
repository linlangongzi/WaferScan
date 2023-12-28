#include "AT4500.h"
#include <utils/Util.h>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(debug, "temp.at4500", QtInfoMsg)

AT4500::AT4500(QObject *parent) :
    QObject(parent),
    serial(new QSerialPort(this))
{
    serial->setPortName("COM1");
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);

    connect(serial, &QIODevice::readyRead, this, &AT4500::onDataReady);
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(onError(QSerialPort::SerialPortError)));

    startTimer(100);
}

QString AT4500::portName() const
{
    return serial->portName();
}

void AT4500::setPortName(QString portName)
{
    if (serial->isOpen()) {
        serial->close();
    }
    serial->setPortName(portName);
    serial->open(QIODevice::ReadWrite);
}

void AT4500::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if (!serial->isOpen()) {
        serial->open(QIODevice::ReadWrite);
        setState(Uninitialized);
    }
    if (state == WaitingFetch && serial->isOpen() && lastResponse.elapsed() > 500) {
        setState(WaitingValue);
        fetch();
    }
    if (state == Uninitialized && serial->isOpen()) {
        serial->write("\n");
        serial->write("idn?\n");
    }
    if (state != Uninitialized && lastResponse.elapsed() > 1000) {
        setState(Uninitialized);
        qCDebug(debug) << "Port timeout";
    }
}

void AT4500::fetch()
{
    if (serial->isOpen()) {
        serial->write("fetch?\n");
        lastFetch.start();
    }
}

void AT4500::onDataReady()
{
    buffer.append(serial->readAll());
    lastResponse.start();

    if (!buffer.contains('\n')) {
        return;
    }

    QStringList lines = buffer.split('\n', QString::KeepEmptyParts);
    buffer = lines.isEmpty() ? QString() : lines.takeLast();

    for (const QString &line: lines) {
        if (state == Uninitialized) {
            qCDebug(debug) << line.split(',');
            setState(WaitingFetch);
            serial->sendBreak();
        } else {
            if (line.isEmpty()) {
                continue;
            }
            QList<qreal> data = SplitNumber<qreal>(line, QString::SkipEmptyParts);
            qCDebug(debug) << "Data:" << data.size();
            emit dataArrived(data);
            setState(WaitingFetch);
        }
    }
}

void AT4500::onError(QSerialPort::SerialPortError error)
{
    if (!serial->isOpen() && error != QSerialPort::NoError) {
        qCInfo(debug) << "Port error" << error;
        setState(Uninitialized);
    }
}

void AT4500::setState(AT4500::State newState)
{
    if (state != newState) {
        qCDebug(debug) << "State" << state << "->" << newState;
        state = newState;
        if (state == Uninitialized) {
            emit dataArrived(QList<qreal>());
        }
    }
}
