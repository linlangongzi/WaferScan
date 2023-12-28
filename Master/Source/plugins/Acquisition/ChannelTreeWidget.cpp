#include "ChannelTreeWidget.h"
#include "ui_ChannelTreeWidget.h"
#include <QInputDialog>
#include <QMessageBox>
#include <Ultrasonic/UltrasonicManager.h>
#include <utils/algorithm.h>
#include <QLoggingCategory>
#include <QStyledItemDelegate>

static Q_LOGGING_CATEGORY(debug, "acquire.channel.treeWidget")

enum Columns {
    ColChannel,
    ColInterval,
};

class InvalidDelegate : public QStyledItemDelegate
{
public:
    InvalidDelegate(QObject *parent) : QStyledItemDelegate(parent) {}
    virtual QWidget *createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const {return nullptr;}
};

ChannelTreeWidget::ChannelTreeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelTreeWidget)
{
    ui->setupUi(this);

    ui->treeWidget->setItemDelegateForColumn(ColChannel, new InvalidDelegate(ui->treeWidget));
    connect(ui->treeWidget, &QTreeWidget::itemChanged, this, &ChannelTreeWidget::onItemChanged);
}

ChannelTreeWidget::~ChannelTreeWidget()
{
    delete ui;
}

QList<int> ChannelTreeWidget::channels() const
{
    return m_channels;
}

void ChannelTreeWidget::setChannels(const QList<int> &channels)
{
    if (m_channels == channels) {
        return;
    }
    m_channels = channels;

    // Refresh channels.
    QSignalBlocker blocker(ui->treeWidget);
    int current = ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem());
    ui->treeWidget->clear();
    for (int channel: channels) {
        addChannel(channel);
    }
    ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(current));
    blocker.unblock();

    refreshIntervals();

    emit channelsChanged(m_channels);
}

void ChannelTreeWidget::setIntervalColumnVisible(bool visible)
{
    if (m_intervalColumnVisible == visible) {
        return;
    }
    m_intervalColumnVisible = visible;
    ui->treeWidget->setColumnHidden(ColInterval, m_intervalColumnVisible);
    emit intervalColumnVisibleChanged(m_intervalColumnVisible);
}

void ChannelTreeWidget::refreshIntervals()
{
    Q_ASSERT(m_channels.count() == ui->treeWidget->topLevelItemCount());
    QSignalBlocker blocker(ui->treeWidget);
    for (int index = 0; index < m_channels.count(); ++index) {
        auto *item = ui->treeWidget->topLevelItem(index);
        item->setText(ColInterval, tr("%1 mm").arg(m_probeIntervals.value(index)));
        item->setData(ColInterval, Qt::EditRole, m_probeIntervals.value(index));
    }
    blocker.unblock();
}

bool ChannelTreeWidget::intervalColumnVisible() const
{
    return m_intervalColumnVisible;
}

QTreeWidgetItem *ChannelTreeWidget::currentItem()
{
    return ui->treeWidget->currentItem();
}

QList<qreal> ChannelTreeWidget::probeIntervals() const
{
    return m_probeIntervals;
}

void ChannelTreeWidget::setProbeIntervals(const QList<qreal> &probeIntervals)
{
    if (m_probeIntervals == probeIntervals) {
        return;
    }
    m_probeIntervals = probeIntervals;

    refreshIntervals();

    emit probeIntervalsChanged(m_probeIntervals);
}

void ChannelTreeWidget::on_add_clicked()
{
    QList<UTChannel *> channelsFiltered = Utils::filtered(UltrasonicManager::channels(), [&](UTChannel *channel) {
        qCDebug(debug) << "m_channels =" << m_channels << "channelId =" << channel->channelId();
        return !m_channels.contains(channel->channelId());
    });
    if (channelsFiltered.isEmpty()) {
        QMessageBox::information(this, QString(), tr("没有可以添加的多余通道"));
        return;
    }
    QStringList channelNameList = Utils::transform(channelsFiltered, &UTChannel::name);
    QList<int> channelIdList = Utils::transform(channelsFiltered, &UTChannel::channelId);
    bool ok = false;
    QString name = QInputDialog::getItem(this, QString(), tr("通道选择"), channelNameList, 0, false, &ok);
    if (!ok) {
        return;
    }
    int channelId = channelIdList.value(channelNameList.indexOf(name));

    auto channels = m_channels;
    channels.insert(channelId, channelId);
    setChannels(channels);

    auto intervals = m_probeIntervals;
    intervals.insert(channelId, 0);
    setProbeIntervals(intervals);
}

void ChannelTreeWidget::on_remove_clicked()
{
    int row = ui->treeWidget->currentIndex().row();
    if (uint(row) >= uint(m_channels.count())) {
        return;
    }
    auto channelList = channels();
    qCDebug(debug) << "Remove channel at row" << row;
    channelList.removeAt(row);
    setChannels(channelList);

    if (uint(row) >= uint(m_probeIntervals.count())) {
        return;
    }
    auto intervalList = probeIntervals();
    intervalList.removeAt(row);
    setProbeIntervals(intervalList);
}

void ChannelTreeWidget::addChannel(int channelId)
{
    auto *item = new QTreeWidgetItem;
    if (ui->treeWidget->topLevelItemCount() > 0) {
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
    item->setText(ColChannel, UltrasonicManager::channelName(channelId));
    item->setData(ColChannel, Qt::UserRole, channelId);
    ui->treeWidget->addTopLevelItem(item);
}

void ChannelTreeWidget::onItemChanged(QTreeWidgetItem *item, int column)
{
    int index = ui->treeWidget->indexOfTopLevelItem(item);
    if (column == ColInterval && index >= 0) {
        auto intervals = m_probeIntervals;
        while (index >= intervals.count()) {
            intervals << 0;
        }
        intervals[index] = item->data(column, Qt::EditRole).toDouble();
        setProbeIntervals(intervals);
    }
}
