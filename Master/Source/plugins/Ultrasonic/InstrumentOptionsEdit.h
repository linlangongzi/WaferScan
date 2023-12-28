#pragma once

#include <QWidget>
#include "UTInstrument.h"
#include <allrising/ConnectionHelper.h>

namespace Ui {
class InstrumentOptionsEdit;
}

class InstrumentOptionsEdit : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(UTInstrument *instrument READ instrument WRITE setInstrument)
    Q_PROPERTY(InstrumentConfig config READ config WRITE setConfig)
    Q_PROPERTY(InstrumentSetup setup READ setup WRITE setSetup)

public:
    explicit InstrumentOptionsEdit(QWidget *parent = 0);
    ~InstrumentOptionsEdit();

    UTInstrument *instrument() const;
    InstrumentConfig config() const;
    InstrumentSetup setup() const;

public slots:
    void setInstrument(UTInstrument *instrument);
    void setConfig(const InstrumentConfig &config);
    void setSetup(const InstrumentSetup &setup);
    void refreshBenchmark();

private:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private slots:
    void onConnectionChanged(const InstrumentStatus &status);
    void onInstrumentInfoChanged(const InstrumentInfo &info);
    void on_connect_clicked();
    void on_disconnect_clicked();

private:
    Ui::InstrumentOptionsEdit *ui;
    UTInstrument *m_instrument = nullptr;
    InstrumentSetup m_setup;
    ConnectionHelper<InstrumentOptionsEdit, InstrumentConfig> configHandler;
    ConnectionHelper<InstrumentOptionsEdit, InstrumentSetup> setupHandler;
    InstrumentConfig m_config;
};
