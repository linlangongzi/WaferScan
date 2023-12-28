#pragma once

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
class ChannelTreeWidget;
}

class ChannelTreeWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QList<qreal> probeIntervals READ probeIntervals WRITE setProbeIntervals NOTIFY probeIntervalsChanged)
    Q_PROPERTY(QList<int> channels READ channels WRITE setChannels NOTIFY channelsChanged)
    Q_PROPERTY(bool intervalColumnVisible READ intervalColumnVisible WRITE setIntervalColumnVisible NOTIFY intervalColumnVisibleChanged)

public:
    explicit ChannelTreeWidget(QWidget *parent = nullptr);
    ~ChannelTreeWidget() override;
    QTreeWidgetItem *currentItem();

    QList<int> channels() const;
    QList<qreal> probeIntervals() const;
    bool intervalColumnVisible() const;

    void setChannels(const QList<int> &channels);
    void setProbeIntervals(const QList<qreal> &probeIntervals);
    void setIntervalColumnVisible(bool visible);

signals:
    void channelsChanged(const QList<int> &channels);
    void probeIntervalsChanged(const QList<qreal> &probeIntervals);
    void intervalColumnVisibleChanged(bool intervalColumnVisible);

private slots:
    void refreshIntervals();
    void on_add_clicked();
    void on_remove_clicked();
    void addChannel(int channelId);
    void onItemChanged(QTreeWidgetItem *item, int column);

private:
    Ui::ChannelTreeWidget *ui;
    QList<int> m_channels;
    QList<qreal> m_probeIntervals;
    bool m_intervalColumnVisible;
};
