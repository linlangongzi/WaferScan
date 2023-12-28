#pragma once

#include "Ars1000/PipeDataDef.h"
#include "DataManagerBase.h"
#include "PipeGlobalConfig.h"
#include <QCheckBox>
#include <QDialog>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QListWidget>
#include <QMap>
#include <QVector>
#include <utils/treemodel.h>

namespace Ui {
class TestModeBase;
}

class PanelAScan;
class ChannelConfigItem;

class TestModeBase : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(PipeGlobalConfig globalConfig READ globalConfig WRITE setGlobalConfig)

public:
    enum Mode
    {
        InvalidMode = 0,
        FinalPipe = 1,
        FinalBar = 2,
        SamplePipe = 3,
        SampleBar = 4
    };

    Q_ENUM(Mode)
    typedef QVector<QCheckBox*> CheckVector;

public:
    explicit TestModeBase(Mode mode = Mode::FinalPipe, QWidget *parent = 0);
    ~TestModeBase();
    PipeGlobalConfig globalConfig() const;
    void UpdateCheckBoxs();

public slots:
    void Ok();
    void Cancel();
    void SetMode(Mode mode);
    void ChangeDataManager(Mode mode);
    DataManagerBase *DataManager() const;
    void setGlobalConfig(PipeGlobalConfig globalConfig);

private slots:
    void act_AScanBox1(bool isChecked);
    void act_AScanBox2(bool isChecked);
    void act_AScanBox3(bool isChecked);
    void act_AScanBox4(bool isChecked);
    void act_AScanBox5(bool isChecked);
    void on_addMode_clicked();
    void on_removeMode_clicked();
    void on_modes_itemDoubleClicked(QListWidgetItem *item);
    void addMode(const PipeModeConfig &mode);

private:
    void Connect();
    void Disconnect();
    void CheckBoxAction(int boxIndex, bool checked);
    void RefreshAScanCheckStatus();
    void RefreshBScanCheckStatus();
    void RefreshCScanCheckStatus();
    void RefreshAlarmStatus();
    void RefreshBoxCheckStatus();
    void RefreshAScanUI();
    void RefreshBScanUI();
    void RefreshCScanUI();
    void RefreshAlarmUI();
    void TabifyWidgets();
    QList<PanelAScan *> PanelAScanList();
    QList<QDockWidget *> PanelCScanList();
    QList<QCheckBox *> AScanCheckBoxs();
    QList<QCheckBox *> CScanCheckBoxs();

protected:
    Ui::TestModeBase *ui;
    bool isFlaw = true;
    DataManagerBase *dataManager = nullptr;
    Utils::TreeModel<ChannelConfigItem> channelModel;
};
