#pragma once

#include "WaveformGlobal.h"

#include <allrising/ConnectionHelper.h>
#include <functional>
#include <QPointer>
#include <QWidget>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UTData.h>
#include <Ultrasonic/UTSetup.h>
#include <utils/treemodel.h>

namespace Ui {
class PanelUTSetup;
}

class DataInfoWidget;
class GateItem;
class DacItem;
class InstrumentInfo;
class GateRootItem : public Utils::TypedTreeItem<GateItem> {};

/*!
 * @brief The PanelUTSetup class
 * 超声工艺参数设置的UI界面
 */
class WAVEFORM_EXPORT PanelUTSetup : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(UTSetup setup READ setup WRITE setSetup NOTIFY setupChanged)
    Q_PROPERTY(bool readonly READ isReadonly WRITE setReadonly NOTIFY readonlyChanged)

public:
    explicit PanelUTSetup(QWidget *parent = 0);
    ~PanelUTSetup();

    void hideAlterSourceCheckbox(bool enable);
    void setTabPage(const QString &name);

    UTSetup setup() const;
    bool isReadonly() const;

public slots:
    void setChannel(UTChannel *newChannel);
    void setDataFrame(const UTDataFrame &frame);
    void setInstrumentInfo(const InstrumentInfo &info);
    void setSetup(const UTSetup &setup);
    void setReadonly(bool readonly);
    void setPrfVisible(bool isVisible);

signals:
    void setupChanged(UTSetup setup);
    void readonlyChanged(bool readonly);
    void requestPopup();

private slots:
    void onLicenseUpdated();
    void refreshControls();
    void onLocked(bool locked);
    void onAlterSourceChanged(bool checked);
    void on_importSetup_clicked();
    void on_exportSetup_clicked();
    void on_addGate_clicked();
    void on_removeGate_clicked();
    void on_addDac_clicked();
    void on_removeDac_clicked();
    void on_gates_customContextMenuRequested(const QPoint &pos);
    void on_lock_toggled(bool checked);
    void on_dgsImport_clicked();
    void on_dgsExport_clicked();

private:
    void setTransmitVoltage(const int voltage);
    int voltage() const;
    void initUi();                                                              // 初始化UI界面

private:
    Ui::PanelUTSetup *ui;
    QPointer<UTChannel> channel;
    QList<QDoubleSpinBox *> axisOffset;
    DataInfoWidget *dataInfo;
    Utils::TreeModel<GateRootItem, GateItem> *gateModel;
    Utils::TreeModel<DacItem, DacItem> *dacModel;
    UTSetup m_setup;
    ConnectionHelper<PanelUTSetup, UTSetup> handler;
    QToolButton *popup = Q_NULLPTR;
    bool m_readonly;
};
