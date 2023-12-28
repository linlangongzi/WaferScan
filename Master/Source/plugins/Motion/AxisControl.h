#pragma once

#include <QFrame>
#include <QTime>
#include <QPointer>
#include <QElapsedTimer>
#include <Motion/Axis.h>

class HotkeyListener;

namespace Ui {
class AxisControl;
}

class AxisControl : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(Axis *axis READ axis WRITE setAxis RESET unsetAxis)

public:
    enum JogDirection {
        Forward = 0,
        Reverse = 1,
    };
    Q_ENUM(JogDirection)

public:
    explicit AxisControl(QWidget *parent = 0);
    ~AxisControl();

    double ptpDestination();
    void stop();
    void setDisabled(bool enabled);
    bool isDisabled() const;
    void setToPointValue(double value);
    Axis *axis() const;

public slots:
    void setEnforceStep(bool enabled);
    void setHotKeyEnable(bool arg);
    void jogStep(int direction);
    void cancelMove();
    void continuousForward();
    void continuousReverse();
    void openSettings();
    void setAxis(Axis *axis);
    void unsetAxis();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void timerEvent(QTimerEvent *event) override;

public slots:
    void setAxisConfig(const AxisConfig &config);
    void setAxisSetup(const AxisSetup &setup);
    void setAxisStatus(const AxisStatus &status);
    void on_moveUp_pressed();
    void on_moveDown_pressed();
    void on_moveUp_released();
    void on_moveDown_released();
    void updateDisableStatus();
    void on_loadPos_clicked();
    void on_step_editingFinished();
    void on_speed_editingFinished();
    void on_axis_toggled(bool checked);

private:
    void updateButtonStatus(JogDirection direction, bool pressed);
    void setExpectedDirection(int direction);

private:
    Ui::AxisControl *ui;
    bool m_forceStep = false;
    HotkeyListener *m_hotkeyForward = nullptr;
    HotkeyListener *m_hotkeyReverse = nullptr;
    int m_keyStatus[2] = {0, 0};                                                /* 按键的状态*/
    QElapsedTimer m_keyDownTime;                                                /* 方向操作的按下时间, invalid 为未被按下 */
    int m_expectedDirection = 0;
    int m_currentDirection = 0;
    AxisStatus m_status;
    Axis *m_axis = nullptr;
};
