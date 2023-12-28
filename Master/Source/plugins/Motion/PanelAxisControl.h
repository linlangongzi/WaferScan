#pragma once

#include "MotionPose.h"
#include <QFrame>
#include <QToolButton>
#include <Ultrasonic/UTChannel.h>

class QLabel;
class AxisControl;

namespace Ui {
class PanelAxisControl;
}

class MOTION_EXPORT PanelAxisControl : public QFrame
{
    Q_OBJECT

public:
    explicit PanelAxisControl(QWidget *parent = 0);
    ~PanelAxisControl();

    static PanelAxisControl *instance();
    static void setPose(const MotionPose &pose, const MotionAxisList &axisList);
    static QList<AxisControl *> axisControls();
    void startMove();

public slots:
    void refreshTcpPose();
    void refreshTcpStatus();
    void axesStatusReconnect();

private slots:
    void onParameterChanged(const QString &key, double newValue);
    void refreshAxisList();
    void refreshEndPointList();
    void refreshConnectionStatus();
    void on_SafeGo_clicked();
    void on_homeAll_clicked();
    void on_actionSafetyBarrier_triggered();
    void on_actionMotionOptions_triggered();
    void on_globalPositions_clicked();
    void on_importAxis_clicked();

    void on_actionManualMode_triggered();
    void on_actionRemoteMode_triggered();
    void on_actionCircularMode_triggered();
    void on_actionTcpMode_triggered();

    void on_bindingChannel_currentIndexChanged(int index);
    void setMonitorData(UTDataFrame frame);
    void on_endPoints_currentIndexChanged(int index);

    void on_forwardX_clicked();
    void on_reverseX_clicked();
    void on_forwardY_clicked();
    void on_reverseY_clicked();
    void on_forwardZ_clicked();
    void on_reverseZ_clicked();
    void on_forwardB_clicked();
    void on_reverseB_clicked();
    void on_forwardA_clicked();
    void on_reverseA_clicked();
    void on_load_clicked();
    void on_stopAll_pressed();

private:
    void updateCurrentPage();
    void stopReceiveMonitorData();
    void tcpJogStep(Qt::Axis axis, int direction);
    void tcpJogDegree(Qt::Axis axis, int direction);
    QList<QLabel *> tcpCoords() const;

private:
    Ui::PanelAxisControl *ui;
    QToolButton *statusButton = nullptr;
    UTChannel *m_channelPointer = nullptr;
    int wdog = 0;
    static QList<AxisControl *> axisControlList;
    static PanelAxisControl *m_instance;
};
