#include "TestModeBase.h"
#include "ui_TestModeBase.h"
#include "PipeModeEdit.h"
#include <Core/ICore.h>
#include <Core/Platform.h>
#include <Pipe/PipeManager.h>
#include <QMessageBox>
#include <QTableWidget>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <Waveform/PanelAScan.h>

class ChannelConfigItem : public Utils::TypedTreeItem<ChannelConfigItem>
{
public:
    enum Columns {
        ColChannel,
        ColHardChannel,
        ColSoftChannel,
        ColGroup,
        ColMuxCard,
        ColMuxInput,
        ColMuxOutput,
        ColProbeName,
        ColAngleCorrect,
        ColBaseOffset,
        ColOffset1,
        ColOffset2,
        ColOffset3,
        ColOffset4,
    };

    QVariant data(int column, int role) const override
    {
        if (column == ColChannel) {
            if (role == Qt::DisplayRole) {
                return UltrasonicManager::channelName(channelId);;
            } else if (role == Qt::CheckStateRole) {
                return config.enable ? Qt::Checked : Qt::Unchecked;
            }
        } else if (column == ColHardChannel && (role == Qt::DisplayRole || role == Qt::EditRole)) {
            return config.hardChannel;
        } else if (column == ColSoftChannel && (role == Qt::DisplayRole || role == Qt::EditRole)) {
            return config.softChannel;
        } else if (column == ColGroup && (role == Qt::DisplayRole || role == Qt::EditRole)) {
            return config.probeBoxIndex;
        } else if (column == ColMuxCard && (role == Qt::DisplayRole || role == Qt::EditRole)) {
            return config.muxCardId;
        } else if (column == ColMuxInput && (role == Qt::DisplayRole || role == Qt::EditRole)) {
            return config.muxInput;
        } else if (column == ColMuxOutput && (role == Qt::DisplayRole || role == Qt::EditRole)) {
            return config.muxOutput;
        } else if (column == ColProbeName && (role == Qt::DisplayRole || role == Qt::EditRole)) {
            return config.probeName;
        } else if (column == ColAngleCorrect) {
            if (role == Qt::DisplayRole) {
                return QString("%1 度").arg(config.angleCorrect);
            } else if (role == Qt::EditRole) {
                return config.angleCorrect;
            }
        }else if (column == ColBaseOffset) {
            if (role == Qt::DisplayRole) {
                return QString("%1 mm").arg(config.baseOffset);
            } else if (role == Qt::EditRole) {
                return config.baseOffset;
            }
        } else if (column > ColBaseOffset) {
            qreal value = config.gateOffset.value(column - ColOffset1);
            if (role == Qt::DisplayRole) {
                return QString("%1 mm").arg(value);
            } else if (role == Qt::EditRole) {
                return value;
            }
        }
        return QVariant();
    }
    bool setData(int column, const QVariant &data, int role) override
    {
        if (column == ColChannel && role == Qt::CheckStateRole) {
            config.enable = (data.toInt() == Qt::Checked);
        } else if (column == ColGroup && role == Qt::EditRole) {
            config.probeBoxIndex = data.toInt();
        } else if (column == ColAngleCorrect && role == Qt::EditRole) {
            config.angleCorrect = data.toReal();
        } else if (column == ColBaseOffset && role == Qt::EditRole) {
            config.baseOffset = data.toReal();
        } else if (column > ColBaseOffset && role == Qt::EditRole) {
            int index = column - ColOffset1;
            config.gateOffset.resize(qMax(config.gateOffset.size(), index + 1));
            config.gateOffset[index] = data.toReal();
        } else {
            return false;
        }
        return true;
    }

    Qt::ItemFlags flags(int column) const override
    {
        Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        if (column == ColChannel) {                                             /* 第一列为通道号，不可编辑，可勾选 */
            return flags | Qt::ItemIsUserCheckable;
        } else if (column >= ColAngleCorrect) {                                   /* 后面的列为偏移，可编辑 */
            return flags | Qt::ItemIsEditable;
        }
        return flags;
    }

    PipeChannelConfig config;
    int channelId = -1;
};

TestModeBase::TestModeBase(Mode mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestModeBase)
{
    ui->setupUi(this);

    Connect();
    SetMode(mode);

    channelModel.setHeader(QStringList() << tr("通道")
                                         << tr("硬通道")
                                         << tr("软通道")
                                         << tr("探头盒")
                                         << tr("前置卡号")
                                         << tr("前置卡输入")
                                         << tr("前置卡输出")
                                         << tr("探头名")
                                         << tr("角度校正")
                                         << tr("基础偏移")
                                         << tr("闸门1偏移")
                                         << tr("闸门2偏移")
                                         << tr("闸门3偏移")
                                         << tr("闸门4偏移"));
    ui->channels->setModel(&channelModel);
}

TestModeBase::~TestModeBase()
{
    Disconnect();

    if (dataManager) {
        dataManager->Disconnect();
        delete dataManager;
        dataManager = nullptr;
    }

    delete ui;
}

PipeGlobalConfig TestModeBase::globalConfig() const
{
    PipeGlobalConfig config;

    channelModel.rootItem()->forFirstLevelChildren([&](ChannelConfigItem *item) {
        config.channels << item->config;
    });
    config.autoChannelOffset = ui->autoChannelOffset->isChecked();
    config.autoAngleCorrect = ui->autoAngleCorrect->isChecked();

    config.intervalX = ui->intervalX->value();
    config.intervalR = ui->intervalR->value();
    config.intervalPlot = ui->intervalPlot->value();

    config.autoDeadZone = ui->autoDeadZone->isChecked();
    config.deadZoneLeft = ui->deadZoneLeft->value();
    config.deadZoneRight = ui->deadZoneRight->value();

    for (int index = 0; index < ui->modes->count(); ++index) {
        config.modes << ui->modes->item(index)->data(Qt::UserRole).value<PipeModeConfig>();
    }

    return config;
}

void TestModeBase::UpdateCheckBoxs()
{
    RefreshAScanCheckStatus();
    RefreshBScanCheckStatus();
    RefreshCScanCheckStatus();
    RefreshAlarmStatus();
    RefreshBoxCheckStatus();
}

void TestModeBase::Ok()
{
    dataManager->setConfig(globalConfig());

    if (ui->tabifyWidgets->isChecked()) {
        TabifyWidgets();
    }

    RefreshAScanUI();
    RefreshBScanUI();
    RefreshCScanUI();
    RefreshAlarmUI();

    close();
}

void TestModeBase::Cancel()
{
    close();
}

/*!
 * \brief TestModeBase::SetMode
 * \param mode
 */
void TestModeBase::SetMode(Mode mode)
{
    ChangeDataManager(mode);
}

void TestModeBase::ChangeDataManager(Mode mode)
{
    if(dataManager) {
        dataManager->Disconnect();
        delete dataManager;
        dataManager = nullptr;
    }

    switch (mode) {
    case Mode::FinalPipe:   ui->mode_FinelPipe->setChecked(true);   break;  // 1
    case Mode::FinalBar:    ui->mode_FinelBar->setChecked(true);    break;  // 2
    case Mode::SamplePipe:  ui->mode_SamplePipe->setChecked(true);  break;  // 3
    case Mode::SampleBar:   ui->mode_SampleBar->setChecked(true);   break;  // 4
    default:                ui->mode_InValid->setChecked(true);     break;  // 0
    }

    dataManager = new DataManagerBase;
    dataManager->setConfig(globalConfig());
    if (dataManager) {
        dataManager->Connect();
    }
}

DataManagerBase *TestModeBase::DataManager() const
{
    return dataManager;
}

void TestModeBase::setGlobalConfig(PipeGlobalConfig globalConfig)
{
    channelModel.clear();
    int count = globalConfig.channels.count();
    for (int i = 0; i < count; ++i) {
        auto *item = new ChannelConfigItem;
        item->channelId = i;
        item->config = globalConfig.channels.value(i);
        channelModel.rootItem()->appendChild(item);
    }
    for (int index = 0; index < channelModel.columnCount(QModelIndex()); ++index) {
        ui->channels->resizeColumnToContents(index);
    }
    ui->autoChannelOffset->setChecked(globalConfig.autoChannelOffset);
    ui->autoAngleCorrect->setChecked(globalConfig.autoAngleCorrect);

    ui->intervalX->setValue(globalConfig.intervalX);
    ui->intervalR->setValue(globalConfig.intervalR);
    ui->intervalPlot->setValue(globalConfig.intervalPlot);

    ui->deadZoneLeft->setValue(globalConfig.deadZoneLeft);
    ui->deadZoneRight->setValue(globalConfig.deadZoneRight);
    ui->autoDeadZone->setChecked(globalConfig.autoDeadZone);

    ui->modes->clear();
    for (auto &mode: globalConfig.modes) {
        addMode(mode);
    }

    dataManager->setConfig(globalConfig);
}

void TestModeBase::act_AScanBox1(bool isChecked)
{
    CheckBoxAction(1, isChecked);
}

void TestModeBase::act_AScanBox2(bool isChecked)
{
    CheckBoxAction(2, isChecked);
}

void TestModeBase::act_AScanBox3(bool isChecked)
{
    CheckBoxAction(3, isChecked);
}

void TestModeBase::act_AScanBox4(bool isChecked)
{
    CheckBoxAction(4, isChecked);
}

void TestModeBase::act_AScanBox5(bool isChecked)
{
    CheckBoxAction(5, isChecked);
}

/*!
 * \brief TestModeBase::Connect
 * 信号槽绑定
 */
void TestModeBase::Connect()
{
    connect(ui->pushButtonOk, &QAbstractButton::clicked, this, &TestModeBase::Ok);
    connect(ui->pushButtonCancel, &QAbstractButton::clicked, this, &TestModeBase::Cancel);
    connect(ui->probeBox_1, &QAbstractButton::clicked, this, &TestModeBase::act_AScanBox1);
    connect(ui->probeBox_2, &QAbstractButton::clicked, this, &TestModeBase::act_AScanBox2);
    connect(ui->probeBox_3, &QAbstractButton::clicked, this, &TestModeBase::act_AScanBox3);
    connect(ui->probeBox_4, &QAbstractButton::clicked, this, &TestModeBase::act_AScanBox4);
    connect(ui->probeBox_5, &QAbstractButton::clicked, this, &TestModeBase::act_AScanBox5);
}

/*!
 * \brief TestModeBase::Disconnect
 * 信号槽解除绑定
 */
void TestModeBase::Disconnect()
{
    disconnect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(Ok()));
    disconnect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(Cancel()));
    disconnect(ui->probeBox_1, &QAbstractButton::clicked, this, &TestModeBase::act_AScanBox1);
    disconnect(ui->probeBox_2, &QAbstractButton::clicked, this, &TestModeBase::act_AScanBox2);
    disconnect(ui->probeBox_3, &QAbstractButton::clicked, this, &TestModeBase::act_AScanBox3);
    disconnect(ui->probeBox_4, &QAbstractButton::clicked, this, &TestModeBase::act_AScanBox4);
    disconnect(ui->probeBox_5, &QAbstractButton::clicked, this, &TestModeBase::act_AScanBox5);
}

/*!
 * \brief TestModeBase::CheckBoxAction
 * \param boxIndex:要从1开始
 * \param checked：选择状态
 */
void TestModeBase::CheckBoxAction(int boxIndex, bool checked)
{
    auto *probeBoxCheck = findChild<QCheckBox *>(tr("probeBox_%1").arg(boxIndex));
    if (!probeBoxCheck) {
        return;
    }

    QString findName = tr("AScan_Box%1_").arg(boxIndex);
    for (QCheckBox *channel : findChildren<QCheckBox *>(QRegularExpression(tr("%1\\d+").arg(findName)))) {
        channel->setChecked(checked);
    }
}

void TestModeBase::RefreshAScanCheckStatus()
{
    QList<PanelAScan *> ascanList = PanelAScanList();
    QList<QCheckBox *> checkList = AScanCheckBoxs();
    int count = ascanList.count();
    if (count != checkList.count()) {
        return;
    }

    for (int i = 0; i < count; ++i) {
        if (ascanList[i]->isHidden()) {
            checkList[i]->setChecked(false);
        } else {
            checkList[i]->setChecked(true);
        }
    }
}

void TestModeBase::RefreshBScanCheckStatus()
{
    auto *bscanMain = Core::ICore::mainWindow()->findChild<QDockWidget *>("PipePanelBScanMain");
    if (bscanMain) {
        if (bscanMain->isHidden()) {
            ui->BScanView->setChecked(false);
        } else {
            ui->BScanView->setChecked(true);
        }
    }
}

void TestModeBase::RefreshCScanCheckStatus()
{
    QList<QDockWidget *> cscanList = PanelCScanList();
    QList<QCheckBox *> cscanCheckList = CScanCheckBoxs();

    int count = cscanList.count();
    if (cscanCheckList.count() != count) {
        return;
    }

    for (int i = 0; i < count; ++i) {
        if (cscanList[i]->isHidden()) {
            cscanCheckList[i]->setChecked(false);
        } else {
            cscanCheckList[i]->setChecked(true);
        }
    }
}

void TestModeBase::RefreshAlarmStatus()
{
    auto *alarm = Core::ICore::mainWindow()->findChild<PanelAlarm *>("PanelAlarm");
    if (alarm) {
        if (alarm->isHidden()) {
            ui->AlarmView->setChecked(false);
        } else {
            ui->AlarmView->setChecked(true);
        }
    }
}

void TestModeBase::RefreshBoxCheckStatus()
{
    for (QCheckBox *probeBox : findChildren<QCheckBox *>(QRegularExpression("probeBox_\\d+"))) {
        if (probeBox) {
            probeBox->setChecked(false);
            int probeBoxIndex = probeBox->objectName().midRef(9).toInt();
            for (QCheckBox *channel : findChildren<QCheckBox *>(QRegularExpression(tr("AScan_Box%1_\\d+").arg(probeBoxIndex)))) {
                if (channel && channel->isChecked()) {
                    probeBox->setChecked(true);
                    break;
                }
            }
        }
    }
}

void TestModeBase::RefreshAScanUI()
{
    QList<PanelAScan *> ascanList = PanelAScanList();
    QList<QCheckBox *> checkList = AScanCheckBoxs();
    int count = ascanList.count();
    if (count != checkList.count()) {
        return;
    }

    for (int i = 0; i < count; ++i) {
        if (checkList[i]->isChecked()) {
            ascanList[i]->show();
        } else {
            ascanList[i]->hide();
        }
    }
}

void TestModeBase::RefreshBScanUI()
{
    auto *bscanMain = Core::ICore::mainWindow()->findChild<QDockWidget *>("PipePanelBScanMain");
    if (bscanMain) {
        if (ui->BScanView->isChecked()) {
            bscanMain->show();
        } else {
            bscanMain->hide();
        }
    }
}

void TestModeBase::RefreshCScanUI()
{
    QList<QDockWidget *> cscanList = PanelCScanList();
    QList<QCheckBox *> cscanCheckList = CScanCheckBoxs();

    int count = cscanList.count();
    if (cscanCheckList.count() != count) {
        return;
    }

    for (int i = 0; i < count; ++i) {
        if (cscanCheckList[i]->isChecked()) {
            cscanList[i]->show();
        } else {
            cscanList[i]->hide();
        }
    }
}

void TestModeBase::RefreshAlarmUI()
{
    auto *alarm = Core::ICore::mainWindow()->findChild<PanelAlarm *>("PanelAlarm");
    if (alarm) {
        if (ui->AlarmView->isChecked()) {
            alarm->show();
        } else {
            alarm->hide();
        }
    }
}

void TestModeBase::TabifyWidgets()
{
    // AScan
    PanelAScan *firstAScan = nullptr;
    QList<PanelAScan *> ascanList = PanelAScanList();
    for (int index = 0; index < ascanList.count(); ++index) {
        if (firstAScan == nullptr && ascanList[index]->isVisible()) {
            firstAScan = ascanList[index];
        }

        if (index > 0) {
            Core::ICore::mainWindow()->tabifyDockWidget(qobject_cast<QDockWidget *>(ascanList[index - 1]), qobject_cast<QDockWidget *>(ascanList[index]));
        }
    }

    if (firstAScan) {
        firstAScan->raise();
    }

    // CScan
    QDockWidget *firstCScan = nullptr;
    QList<QDockWidget *> cscanList = PanelCScanList();
    for (int index = 0; index < cscanList.count(); ++index) {
        if (firstCScan == nullptr && cscanList[index]->isVisible()) {
            firstCScan = cscanList[index];
        }

        if (index > 0) {
            Core::ICore::mainWindow()->tabifyDockWidget(cscanList[index - 1], cscanList[index]);
        }
    }

    if (firstCScan) {
        firstCScan->raise();
    }
}

QList<PanelAScan *> TestModeBase::PanelAScanList()
{
    QList<PanelAScan *> ascanList = Core::ICore::mainWindow()->findChildren<PanelAScan *>(QRegularExpression("PanelAScan_\\d+"));
    int count = ascanList.count();

    for (int i = 0; i < count; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (ascanList[j]->objectName().midRef(11).toInt() < ascanList[i]->objectName().midRef(11).toInt()) {
                ascanList.move(j, i);
            }
        }
    }

    return ascanList;
}

QList<QCheckBox *> TestModeBase::AScanCheckBoxs()
{
    QList<QCheckBox *> checkList = findChildren<QCheckBox *>(QRegularExpression(R"(AScan_\w+\d+_\d+)"));
    int count = checkList.count();
    for (int i = 0; i < count; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (checkList[j]->objectName().midRef(11).toInt() < checkList[i]->objectName().midRef(11).toInt()) {
                checkList.move(j, i);
            }
        }
    }

    return checkList;
}

QList<QCheckBox *> TestModeBase::CScanCheckBoxs()
{
    QList<QCheckBox *> cscanCheckList = findChildren<QCheckBox *>(QRegularExpression("CScanView_\\d+"));
    int count = cscanCheckList.count();

    for (int i = 0; i < count; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (cscanCheckList[j]->objectName().midRef(10).toInt() < cscanCheckList[i]->objectName().midRef(10).toInt()) {
                cscanCheckList.move(j, i);
            }
        }
    }

    return cscanCheckList;
}

QList<QDockWidget *> TestModeBase::PanelCScanList()
{
    QList<QDockWidget *> cscanList = Core::ICore::mainWindow()->findChildren<QDockWidget *>(QRegularExpression("DockCScan_\\d+"));
    int count = cscanList.count();

    for (int i = 0; i < count; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (cscanList[j]->objectName().midRef(10).toInt() < cscanList[i]->objectName().midRef(10).toInt()) {
                cscanList.move(j, i);
            }
        }
    }

    return cscanList;
}

void TestModeBase::on_addMode_clicked()
{
    PipeModeEdit dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        addMode(dialog.config());
    }
}

void TestModeBase::on_removeMode_clicked()
{
    delete ui->modes->currentItem();
}

void TestModeBase::on_modes_itemDoubleClicked(QListWidgetItem *item)
{
    PipeModeEdit dialog(this);
    auto mode = item->data(Qt::UserRole).value<PipeModeConfig>();
    dialog.setConfig(mode);
    if (dialog.exec() == QDialog::Accepted) {
        mode = dialog.config();
        item->setText(mode.name());
        item->setData(Qt::UserRole, QVariant::fromValue(mode));
    }
}

void TestModeBase::addMode(const PipeModeConfig &mode)
{
    auto *item = new QListWidgetItem(ui->modes);
    item->setText(mode.name());
    item->setData(Qt::UserRole, QVariant::fromValue(mode));
}
