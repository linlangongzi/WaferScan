#include "MotionOptions.h"
#include "ui_MotionOptions.h"
#include "Axis.h"
#include "DialogAxisConfig.h"
#include "DialogEndPointOptions.h"
#include "EndPoint.h"
#include "MotionConstants.h"
#include "MotionManager.h"
#include "TrioDriver.h"
#include <Core/ICore.h>
#include <Core/Platform.h>
#include <Core/actionmanager/actionmanager.h>
#include <QInputDialog>
#include <QWidgetList>
#include <utils/algorithm.h>
#include <utils/treemodel.h>

using namespace Motion;

class IOConfigItem : public Utils::TypedTreeItem<IOConfigItem>
{
public:
    enum Columns {
        ColId,
        ColName,
        ColInvert
    };

    QVariant data(int column, int role) const override
    {
        if (column == ColId) {
            if (role == Qt::DisplayRole || role == Qt::EditRole) {
                return index().row();
            } else if (role == Qt::DecorationRole) {
                if (MotionManager::driver()->status().state != Normal) {
                    return QColor(Qt::red);
                }
                int i = index().row();
                QBitArray ba = MotionManager::driver()->ioStatus();
                bool status = i >= ba.size() ? false : ba.at(i);
                return status ? QColor(Qt::green) : QColor(Qt::gray);
            }
        } else if (column == ColName) {
            if (role == Qt::DisplayRole || role == Qt::EditRole) {
                return config.name;
            }
        } else if (column == ColInvert) {
            if (role == Qt::CheckStateRole) {
                return config.inverted ? Qt::Checked : Qt::Unchecked;
            }
        }

        return QVariant();
    }

    bool setData(int column, const QVariant &data, int role) override
    {
        if (column == ColInvert && role == Qt::CheckStateRole) {
            config.inverted = (data.toInt() == Qt::Checked);
            return true;
        } else if (column == ColName && role == Qt::EditRole) {
            config.name = data.toString();
            return true;
        }

        return false;
    }

    Qt::ItemFlags flags(int column) const override
    {
        Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        if (column == ColName) {
            flags |= Qt::ItemIsEditable;
        }
        if (column == ColInvert) {                                             /* 第一列为通道号，不可编辑，可勾选 */
            return flags | Qt::ItemIsUserCheckable;
        }

        return flags;
    }

    ControllerIoConfig config;
};

MotionOptions::MotionOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotionOptions)
{
    ui->setupUi(this);

    using Core::ActionManager;
    ui->connect->setDefaultAction(ActionManager::command(Motion::CONNECT)->action());
    ui->disconnect->setDefaultAction(ActionManager::command(Motion::DISCONNECT)->action());

    auto *driver = MotionManager::driver();
    auto config = driver->config();

    initUi(driver->status());
    ui->ip->setText(config.host);
    ui->fastProgram->setChecked(config.fastProgram);
    for (const auto &ioConfig: config.ioConfig) {
        auto *item = new IOConfigItem;
        item->config = ioConfig;
        ioModel.rootItem()->appendChild(item);
    }
    ioModel.setHeader(QStringList{tr("IO"), tr("名称"), tr("反转")});
    ui->ioList->setModel(&ioModel);
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(G_MOTION_SETTINGS);
    ui->simulateDynamic->setChecked(settings->value(K_CONTROLLER_SIMULATE_DYNAMIC).toBool());
    settings->endGroup();

    ui->disableDatumStatus->setChecked(platform()->Get("MotionDisableDatumStatus").toBool());

    axisModel = new AxisModel(this);
    ui->axisList->setModel(axisModel);
    ui->axisList->setColumnHidden(AxisModel::ColUuid, true);

    connect(driver, &TrioDriver::ioStatusChanged, this, &MotionOptions::updateIoStatus);
    connect(driver, &TrioDriver::controllerInfoChanged, this, &MotionOptions::updateControllerInfo);
    connect(driver, &TrioDriver::statusChanged, this, &MotionOptions::updateControllerInfo);
    connect(MotionManager::instance(), &MotionManager::endPointsChanged, this, &MotionOptions::refreshEndPoint);
    refreshEndPoint();
    updateControllerInfo();
}

MotionOptions::~MotionOptions()
{
    delete ui;
}

void MotionOptions::apply()
{
    ControllerConfig config;
    config.host = ui->ip->text();
    config.fastProgram = ui->fastProgram->isChecked();
    ioModel.forItemsAtLevel<1>([&](IOConfigItem *item) {
        config.ioConfig << item->config;
    });

    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(G_MOTION_SETTINGS);
    settings->setValue(K_CONTROLLER_SIMULATE_DYNAMIC, ui->simulateDynamic->isChecked());
    MotionManager::instance()->simulateDynamicConnection(ui->simulateDynamic->isChecked());
    settings->endGroup();

    platform()->Set("MotionDisableDatumStatus", ui->disableDatumStatus->isChecked());

    MotionManager::driver()->setConfig(config);
    MotionManager::instance()->saveConfig();
}

void MotionOptions::refreshEndPoint()
{
    ui->endPointList->clear();
    ui->endPointList->addItems(Utils::transform(MotionManager::endPoints(), &EndPoint::description));
}

void MotionOptions::updateIoStatus()
{
    ioModel.forItemsAtLevel<1>([](IOConfigItem *item) {
        item->update();
    });
}

void MotionOptions::updateControllerInfo()
{
    auto info = MotionManager::driver()->controllerInfo();
    MetaType::forEachProperty(info, [this] (QString name, QVariant value) {
        auto *label = findChild<QLabel *>(name);
        if (label) {
            label->setText(value.toString());
        }
    });
    auto status = MotionManager::driver()->status();
    ui->connectionStatus->setText(status.text());
    ui->featureEnabled->setText(info.featureEnabledText());
    ui->cpuLoad->setText(QString("%1%").arg(info.cpuLoad, 0, 'f', 2));
    updateIoStatus();
}

void MotionOptions::on_axisList_doubleClicked(const QModelIndex &index)
{
    DialogAxisConfig dialog(this);
    dialog.loadAxis(MotionManager::axis(index.row()));
    dialog.exec();
}

void MotionOptions::on_axisAdd_clicked()
{
    const QString name = QInputDialog::getText(this, QString(), tr("名称:"));
    if (name.isEmpty()) {        return;
    }
    auto *axis = new Axis;
    AxisConfig config;
    config.name = name;
    axis->setConfig(config);
    MotionManager::instance()->addAxis(axis);
}

void MotionOptions::on_axisRemove_clicked()
{
    MotionManager::instance()->removeAxis(MotionManager::axis(ui->axisList->currentIndex().row()));
}

void MotionOptions::on_endPointList_doubleClicked(const QModelIndex &index)
{
    DialogEndPointOptions dialog(this);
    dialog.loadEndPoint(MotionManager::endPoint(index.row()));
    dialog.exec();
    refreshEndPoint();
}

void MotionOptions::on_endPointAdd_clicked()
{
    const QString name = QInputDialog::getText(this, QString(), tr("名称:"));
    if (name.isEmpty()) {
        return;
    }
    auto *endPoint = new EndPoint;
    endPoint->setName(name);
    MotionManager::instance()->addEndPoint(endPoint);
}

void MotionOptions::on_endPointRemove_clicked()
{
    MotionManager::instance()->removeEndPoint(MotionManager::endPoint(ui->endPointList->currentRow()));
}

void MotionOptions::on_ioAdd_clicked()
{
    ioModel.rootItem()->appendChild(new IOConfigItem);
}

void MotionOptions::on_ioRemove_clicked()
{
    auto item = ioModel.rootItem()->lastChild();
    if (item) {
        ioModel.destroyItem(item);
    }
}

MotionOptionsPage::MotionOptionsPage(QObject *parent) :
    Core::IOptionsPage(parent)
{
    setId(SETTINGS_ID_MOTION);
    setCategory(SETTINGS_CATEGORY_MOTION);
    setDisplayName(tr("轴配置"));
    setDisplayCategory(tr("运动控制"));
    setCategoryIcon(QIcon(SETTINGS_ICON_MOTION));
}

QWidget *MotionOptionsPage::widget()
{
    if (!m_widget) {
        m_widget = new MotionOptions;
    }

    return m_widget;
}

void MotionOptionsPage::apply()
{
    m_widget->apply();
}

void MotionOptionsPage::finish()
{
    delete m_widget;
}

void MotionOptions::on_simulateDynamic_toggled(bool checked)
{
    MotionManager::instance()->simulateDynamicConnection(checked);
}

void MotionOptions::initUi(const ControllerStatus &status)
{
    bool isDisconnect = (status.state == Motion::ControllerState::Disconnected);
    QWidgetList editableWidgets({
        ui->axisAdd,
        ui->axisRemove,
        ui->endPointAdd,
        ui->endPointRemove,
    });
    for (QWidget *widget: editableWidgets) {
        widget->setEnabled(isDisconnect);
    }
}
