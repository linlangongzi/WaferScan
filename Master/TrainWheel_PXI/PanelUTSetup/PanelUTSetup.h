#ifndef PANELUTSETUP_H
#define PANELUTSETUP_H

#include <QWidget>
#include <QStandardItemModel>
#include "ConfigObject/ConfigManager.h"
#include "ConfigObject/UTSetup.h"
#include <QMessageBox>
#include "ConfigControl/ConfigRadioButton.h"
#include "ConfigControl/ConfigSpinBox.h"
#include "ConfigControl/ConfigSlider.h"
#include "ConfigControl/ConfigDoubleSpinBox.h"
#include "PanelUTGate/PanelUTGate.h"
#include "PanelUTSetup/PanelUTSetupTimebase.h"
#include "PanelChannel/ChannelTreeView.h"
#include "PanelTCG/PanelTCG.h"

namespace Ui {
class PanelUTSetup;
}

/**
 * @brief The PanelUTSetup class
 * 超声工艺参数设置的UI界面
 *
 */
class PanelUTSetup : public QWidget
{
    Q_OBJECT

    typedef QList<QStandardItem *> StdItemList;
    typedef QPointer<ConfigRadioButton> ConfigRadioButtonPointer;
    typedef QList<ConfigRadioButtonPointer> RadioButtonList;
    typedef ChannelTreeView::WaveType WaveType;

public:
    explicit PanelUTSetup(QWidget *parent = 0);
    ~PanelUTSetup();

signals:
    void ChannelChanged(int nChannelNo);
    void SetupChanged(UTSetup *setup);

public slots:
    void BindUTSetup(UTSetup *setup);
    void UnbindUTSetup();
    void SetChannelNo(int nChannelNo, WaveType type);

private slots:
    void GainDialToSpinBox(int value);
    void GainSpinBoxToDial(double value);
    void GainRefDialToSpinBox(int value);
    void GainRefSpinBoxToDial(double value);

private:
    void InitUI();     // 初始化UI界面

    void GetWidgetChildList(QWidget *widget, RadioButtonList &rBtnList);

    void InitControlValue();  // 初始化UI界面上控件的值

    void Disconnect(QWidget *widget, UTSetup *setup);

    void DisconnectWidgetChildren(QObject *obj, UTSetup *setup);

    RadioButtonList probeBandRadioButtons;
    RadioButtonList rectifierModeRadioButtons;
private:
    Ui::PanelUTSetup *ui;
    QPointer<UTSetup> m_pUTSetup;

    PanelUTGate *panelUTGate;
//    PanelUTSetupTimebase *panelMaterialAttribute;
    PanelTCG  *panelTCG;
    static const int MIN_PULSE_WIDTH;

    static const int MAX_PULSE_WIDTH;

    static const int MIN_PRF;		// 重复频率最小值

    static const int MAX_PRF;		// 重复频率最大值 ADFREQUENCY成400，最大值为16000 ；；ADFREQUENCY成200，最大值为8000 ； ADFREQUENCY成100，最大值为4000

    static const double MAX_RANGE;

    static const double MIN_RANGE;

    static const double MAX_DELAY;

    static const double MIN_DELAY;

    static const double MAX_HEIGH;

    static const double MIN_HEIGH;

    static const double MAX_GAIN;

    static const int tableViewWidth;
    void CopyChannelData();
    void ConnectTo();
};

#endif // PANELUTSETUP_H
