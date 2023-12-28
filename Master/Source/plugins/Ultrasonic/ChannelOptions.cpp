#include "ChannelModel.h"
#include "ChannelOptions.h"
#include "DialogChannelConfig.h"
#include "ui_ChannelOptions.h"
#include "UltrasonicConstants.h"
#include "UltrasonicManager.h"
#include <QMessageBox>
#include <utils/algorithm.h>
#include <QLoggingCategory>

using namespace Ultrasonic;
static Q_LOGGING_CATEGORY(debug, "ut.channel.option");

ChannelOptions::ChannelOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelOptions)
{
    ui->setupUi(this);

    model = new ChannelModel(this);
    ui->channels->setModel(model);
}

ChannelOptions::~ChannelOptions()
{
    delete ui;
}

void ChannelOptions::on_add_clicked()
{
    DialogChannelConfig dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const auto config = dialog.config();
    const auto id = config.identifier;
    if (UltrasonicManager::isValidChannel(id.instrument, id.channel, id.cycle)) {
        QMessageBox::warning(this, QString(), tr("无法添加通道: %1 中的物理通道 %2 已被占用")
                             .arg(id.instrument).arg(id.channel));
        return;
    }

    UTChannel *channel = UltrasonicManager::instance()->createChannel(config);
    if (!channel) {
        QMessageBox::warning(this, QString(), tr("无法添加通道: 通道号 %1 已被占用").arg(config.channelId));
    }
}

void ChannelOptions::on_edit_clicked()
{
    UTChannel *channel = model->channel(ui->channels->currentIndex());
    if (!channel) {
        return;
    }

    DialogChannelConfig dialog(this);
    const auto oldConfig = channel->config();
    dialog.setConfig(oldConfig);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const auto newConfig = dialog.config();
    if (oldConfig == newConfig) {
        return;
    }

    if (newConfig.channelId != oldConfig.channelId && UltrasonicManager::isValidId(newConfig.channelId)) {  // channelID 已存在
        QMessageBox::warning(this, QString(), tr("无法编辑通道: 新的通道号 %1 已被占用").arg(newConfig.channelId));
        return;
    } else if (!UltrasonicManager::isValidId(newConfig.channelId)) {
        qCDebug(debug, "Remove channel by id: %d", oldConfig.channelId);
        UltrasonicManager::instance()->removeChannel(oldConfig.channelId);
        qCDebug(debug, "Create channel by id: %d", newConfig.channelId);
        UltrasonicManager::instance()->createChannel(newConfig);
        return;
    } else if ((newConfig.identifier != oldConfig.identifier) && UltrasonicManager::channel(newConfig.identifier)) {
        QMessageBox::warning(this, QString(), tr("无法编辑通道: 仪器(%1) 物理通道(%2) 软通道(%3) 已存在")
                                                .arg(newConfig.identifier.instrument)
                                                .arg(newConfig.identifier.channel)
                                                .arg(newConfig.identifier.cycle));
        return;
    }

    channel->setConfig(newConfig);
}

void ChannelOptions::on_remove_clicked()
{
    QModelIndexList indexes = ui->channels->selectionModel()->selectedRows(0);
    QList<UTChannel *> channels = Utils::transform(indexes, [this](const QModelIndex &index) {
        return model->channel(index);
    });
    channels = Utils::filteredUnique(channels);
    if (QMessageBox::warning(this, QString(), tr("要删除选中的 %1 个通道吗?").arg(channels.count()), QMessageBox::Ok | QMessageBox::Cancel) != QMessageBox::Ok) {
        return;
    }
    for (UTChannel *channel: channels) {
        if (channel) {
            UltrasonicManager::instance()->removeChannel(channel->channelId());
        }
    }
}

void ChannelOptions::on_addMultiple_clicked()
{
    if (QMessageBox::warning(this, QString(), tr("这个操作将删除并重新创建所有通道，已有的通道配置会丢失。\n\n确定要继续吗?"), QMessageBox::Ok | QMessageBox::Cancel) != QMessageBox::Ok) {
        return;
    }
    UltrasonicManager::instance()->createLegacyChannels();
}

void ChannelOptions::on_channels_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    on_edit_clicked();
}

ChannelOptionsPage::ChannelOptionsPage(QObject *parent) :
    Core::IOptionsPage(parent)
{
    setId(SETTINGS_ID_CHANNEL);
    setCategory(SETTINGS_CATEGORY_ULTRASONIC);
    setCategoryIcon(QIcon(SETTINGS_ICON_ULTRASONIC));
    setDisplayCategory(tr("仪器"));
    setDisplayName(tr("通道"));
}

QWidget *ChannelOptionsPage::widget()
{
    if (!m_widget) {
        m_widget = new ChannelOptions;
    }
    return m_widget;
}

void ChannelOptionsPage::finish()
{
    delete m_widget;
}
