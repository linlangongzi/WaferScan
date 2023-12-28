#ifndef CHANNELLISTWIDGET_H
#define CHANNELLISTWIDGET_H

#include <QWidget>

namespace Ui {
class ChannelListWidget;
}

class ChannelListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelListWidget(QWidget *parent = 0);
    ~ChannelListWidget();

    void setValue(const QList<int> &channels);
    QList<int> value() const;
    int currentChannelId() const;

signals:
    void valueChanged(const QList<int> &channels);

private slots:
    void on_add_clicked();
    void on_remove_clicked();
    void addChannel(const int channelId);
    QList<int> channelListItems() const;

private:
    Ui::ChannelListWidget *ui;
};

#endif // CHANNELLISTWIDGET_H
