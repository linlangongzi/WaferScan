#pragma once

#include "WaveformGlobal.h"
#include <allrising/ConnectionHelper.h>
#include <QJsonValue>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <Ultrasonic/ChannelModel.h>
#include <Ultrasonic/IInstrumentDriver.h>

class PanelUTSetup;
class UTChannel;

namespace Ui {
class PanelAScan;
}

class WAVEFORM_EXPORT PanelAScan : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(UTSetup setup READ setup WRITE setSetup NOTIFY setupChanged)

public:
    using UTChannelPointer = QPointer<UTChannel>;

public:
    explicit PanelAScan(QWidget *parent = nullptr);
    ~PanelAScan() override;

    QJsonObject saveSettings() const;
    void loadSettings(const QJsonObject &layout);
    QPixmap RenderPixmap();
    UTDataFrame GetCurrentData() const {return lastFrame;}
    void ProjectCustomizeUI(QString projectName);
    UTSetup setup() const;
    void setSetupLocked(bool locked);

public slots:
    void setChannelId(int channelId);
    void SetToolBarVisible(bool enabled);
    void SetFrozenData(const UTDataFrame &frame);
    void setSetup(const UTSetup &setup);

signals:
    void setupChanged(UTSetup setup);

private:
    void UiInit();                                                              // UI初始化
    void InitAction();
    bool eventFilter(QObject *obj, QEvent *event);
    void setInstrumentInfo(const InstrumentInfo &info);

private slots:
    void SetMonitorData(UTDataFrame data);                                // A扫描收到数据并告知PlotterData更新数据并画图
    void OnMonitorDataArrived(UTDataFrame frame);
    void OnDataArrived(UTDataSegment segment);
    void openMaterialDialog();
    void openMaterialDlg();
    void onInstrumentConnectChanged(const InstrumentStatus &status);
    void OnOpenDAC();
    void ExportWaveform();
    void on_connect_clicked();
    void on_actionScreenshot_triggered();
    void on_actionCopyScreenshots_triggered();
    void on_actionSetup_triggered();
    void on_actionExportCsv_triggered();
    void on_actionFreeze_toggled(bool checked);
    void on_actionGateAlarm_triggered();
    void on_channelList_currentIndexChanged(int index);
    void on_gainShow_toggled(bool checked);
    void on_channels_clicked(const QModelIndex &index);
    void on_gainPlus_clicked();
    void on_gainMinus_clicked();
    void on_gainSlider_valueChanged(int value);
    void on_gain_valueChanged(double arg1);
    void on_gainRefSlider_valueChanged(int value);
    void on_gainRef_valueChanged(double arg1);
    void on_actionSelectProbe_triggered();

private:
    Ui::PanelAScan *ui;
    PanelUTSetup *panelUTSetup = nullptr;
    QDialog *setupWindow = nullptr;

    UTChannelPointer curChannel;                                                // 当前绑定通道
    int m_channelId = -1;                                                       // 当前监视的通道号
    int gateIndex = -1;                                                         // 当前打开报警的闸门
    UTSetup m_setup;
    UTDataFrame lastFrame;
    ConnectionHelper<PanelAScan, UTSetup> handler;
    ChannelModel *channelModel = nullptr;
    QSortFilterProxyModel *proxyModel = nullptr;
    InstrumentInfo m_info;
};
