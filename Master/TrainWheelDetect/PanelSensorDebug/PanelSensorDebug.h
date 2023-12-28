#ifndef PANELSENSORDEBUG_H
#define PANELSENSORDEBUG_H

#include <QList>
#include <QTime>
#include <QEvent>
#include <QTimer>
#include <QWidget>
#include <QTimerEvent>
#include <QRadioButton>
#include "time.h"
#include <Ultrasonic/UTData.h>
#include "UTInstrumentDriver/PXI100/DataDef.h"

#define SENSOR_FIRST 0
#define FIRST_END 31
#define SECOUND_END 63
#define THIRD_END 95
#define SENSOR_END 127

namespace Ui {
class PanelSensorDebug;
}

class PanelSensorDebug : public QWidget
{
    Q_OBJECT

public:
    typedef QList<QRadioButton *> RadioButtonList;

    enum RunType
    {
        STOP,
        FRONT_RUN,
        FRONT_PAUSE,
        BACK_RUN,
        BACK_PAUSE
    };

public:
    explicit PanelSensorDebug(QWidget *parent = 0);
    ~PanelSensorDebug();

signals:
    void SimulationEnableChanged(bool bEnable);

public slots:
    void ActionChanged(int index, bool active);
    void ChannelChanged(qint32 channelID, qint16 surOrFlaw);
    void SelfTestStateChanged(bool selfTest);
    void ConnectionChanged();
    void OnSelfTestConfirm(int sensorNo);
    void onChannelChanged(int channel);

private slots:
    void ChangeEnable(bool checked);
    void ChangeModle();
    void GoFront();
    void GoBack();
    void Stop();
    void SetRunType(RunType type);
    void SetTimeSpace(qreal timeSpace);
    void TimeEvent();
    void OnScanStart();
    void on_spinBoxSensorStart_valueChanged(int value);
    void on_spinBoxSensorEnd_valueChanged(int value);

private:
    void InitUI();
    void Connect();
    void Clear();
    void SetSensorSwitch(bool isOpen);
    void AddButtonToList(QWidget *widget);

private:
    Ui::PanelSensorDebug *ui;
    RadioButtonList m_radioButtonList;
    RadioButtonList m_radioButtonHide;
    UTSharedDataSimulation p_action;    // 16位 * 9 = 129位有效
    RunType m_type;
    qreal m_timeSpace;
    time_t m_proTime;
    int m_index;
    QTimer *m_timer;
    int m_sensorStart;
    int m_sensorEnd;
};

#endif // PANELSENSORDEBUG_H
