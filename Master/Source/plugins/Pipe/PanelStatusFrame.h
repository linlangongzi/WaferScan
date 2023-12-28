#pragma once

#include <QFrame>
#include <QEvent>
#include <QLabel>
#include <QTimerEvent>
#include "TestModeBase.h"
#include <Ultrasonic/IInstrumentDriver.h>
#include "Ars1000/PipeDataDef.h"

#ifdef OPC
#   include <Opc/DataDefine.h>
#endif

enum TestResultType
{
    NoJudge,
    Accept,
    Reject,
    Decouple
};

namespace Ui {
class PanelStatusFrame;
}

class PanelStatusFrame : public QFrame
{
    Q_OBJECT

public:
    typedef TestModeBase::Mode Mode;
    typedef QVector<QLabel*> LabelVector;

public:
    explicit PanelStatusFrame(QWidget *parent = 0);
    ~PanelStatusFrame();

signals:
    void TestModeChanged(Mode mode);
    void KeepTestingChanged(bool isOpen);
    void ProbeBoxStatusChanged(quint16 status);
    void TestStartFlagArrived(quint16 flag);

public slots:
#ifdef Q_CC_MSVC
    void UpdatePLCStatus(GlobalItemValue &itemNew);
#endif
    void UpdatePLXConnectionStatus(InstrumentStatus status);  // 下位机状态
    void OnResultChanged(const TestResultType &result);

protected:
    void timerEvent(QTimerEvent *event);

private:
    void Init();
    void LockedSetups(bool isLocked);
    void UpdateLocked(int setupIndex);

private:
    Ui::PanelStatusFrame *ui;
#ifdef Q_CC_MSVC
    GlobalItemValue m_itemValue;
#endif
    quint32 m_TIMER_REFRESH_OPC_DATA;
    LabelVector m_boxsLabelVector;

};
