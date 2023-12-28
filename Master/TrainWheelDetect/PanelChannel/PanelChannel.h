#ifndef PANELCHANNEL_H
#define PANELCHANNEL_H

#include <QWidget>
#include <utils/treemodel.h>
#include <QSortFilterProxyModel>
#include <PanelChannel/ChannelItem.h>

namespace Ui {
class PanelChannel;
}

class PanelChannel : public QWidget
{
    Q_OBJECT

public:
    explicit PanelChannel(QWidget *parent = 0);
    ~PanelChannel();

public slots:
    void refreshChannels();
    void setCurrentChannel(int channel);

private slots:
    void on_flaw_toggled(bool checked);
    void on_surface_toggled(bool checked);
    void on_filtreEdit_textChanged(const QString &arg1);
    void onChannelSelected(const QModelIndex &index);

private:
    Ui::PanelChannel *ui;
    Utils::TreeModel<Utils::TreeItem, Utils::StaticTreeItem, ChannelItem> channelModel;
    QSortFilterProxyModel *proxyModel;
};

#endif // PANELCHANNEL_H
