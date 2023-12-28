#pragma once

#include <QWidget>
#include <Ultrasonic/UTInstrument.h>

namespace Ui {
class InstrumentConnection;
}

class InstrumentConnection : public QWidget
{
    Q_OBJECT

public:
    explicit InstrumentConnection(UTInstrument *instrument, QWidget *parent = 0);
    ~InstrumentConnection();

private slots:
    void OnConnectionChanged(InstrumentStatus status);
    void on_connect_clicked();
    void on_disconnect_clicked();
    void on_reset_clicked();

private:
    Ui::InstrumentConnection *ui;
    UTInstrument *instrument;
};
