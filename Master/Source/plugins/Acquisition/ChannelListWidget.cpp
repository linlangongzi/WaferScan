#include "ChannelListWidget.h"
#include "ui_ChannelListWidget.h"
#include <QInputDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <Ultrasonic/UltrasonicManager.h>
#include <utils/algorithm.h>

ChannelListWidget::ChannelListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelListWidget)
{
    ui->setupUi(this);
}

ChannelListWidget::~ChannelListWidget()
{
    delete ui;
}

void ChannelListWidget::setValue(const QList<int> &channels)
{
    ui->listWidget->clear();
    for (int channelId: channels) {
        addChannel(channelId);
    }
    emit valueChanged(channelListItems());
}

QList<int> ChannelListWidget::value() const
{
    return channelListItems();
}

int ChannelListWidget::currentChannelId() const
{
    return ui->listWidget->currentItem()->data(Qt::UserRole).toInt();
}

void ChannelListWidget::on_add_clicked()
{
    QList<UTChannel *> channelsFiltered = Utils::filtered(UltrasonicManager::channels(), [&](UTChannel *channel) {
        return !channelListItems().contains(channel->channelId());
    });
    if (channelsFiltered.isEmpty()) {
        QMessageBox::information(this, QString(), tr("没有可以添加的多余通道"));
        return;
    }
    QStringList listItems = Utils::transform(channelsFiltered, &UTChannel::name);
    bool ok = false;
    QString name = QInputDialog::getItem(this, QString(), tr("通道选择"), listItems, 0, false, &ok);
    if (!ok) {
        return;
    }
    addChannel(channelsFiltered[listItems.indexOf(name)]->channelId());
    emit valueChanged(channelListItems());
}

void ChannelListWidget::on_remove_clicked()
{
    delete ui->listWidget->takeItem(ui->listWidget->currentRow());
    emit valueChanged(channelListItems());
}

void ChannelListWidget::addChannel(const int channelId)
{
    UTChannel *channel = UltrasonicManager::channel(channelId);
    if (channel == nullptr) {
        return;
    }
    auto *item = new QListWidgetItem;
    item->setText(channel->name());
    item->setData(Qt::UserRole, channelId);
    ui->listWidget->addItem(item);
}

QList<int> ChannelListWidget::channelListItems() const
{
    QList<int> channels;
    for (int row = 0; row < ui->listWidget->count(); ++row) {
        auto *item = ui->listWidget->item(row);
        channels << item->data(Qt::UserRole).toInt();
    }
    return channels;
}
