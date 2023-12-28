#include "PanelChannel.h"
#include "ui_PanelChannel.h"
#include <utils/treemodel.h>
#include <arsCore/Platform.h>
#include <Wheel/ControlPanel.h>
#include <WheelDataset/WheelDataset.h>

PanelChannel::PanelChannel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelChannel)
{
    ui->setupUi(this);

    channelModel.setRootItem(new ChannelItem);
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(&channelModel);
    proxyModel->setFilterKeyColumn(0);
    ui->channels->setModel(proxyModel);
    ui->channels->sortByColumn(0, Qt::AscendingOrder);

    connect(ui->channels->selectionModel(), &QItemSelectionModel::currentChanged, this, &PanelChannel::onChannelSelected);
    connect(controlPanel(), &ControlPanel::globalConfigChanged, this, &PanelChannel::refreshChannels);
    refreshChannels();

    ui->surface->click();
}

PanelChannel::~PanelChannel()
{
    delete ui;
}

void PanelChannel::refreshChannels()
{
    channelModel.rootItem()->removeChildren();
    QMap<QString, Utils::StaticTreeItem *> groupMap;
    const auto channelConfigs = controlPanel()->getChannelConfigs();

    for (auto it: channelConfigs) {
        QString groupName = it.channelGroup;
        if (!groupMap.contains(groupName)) {
            auto itemGroup = new Utils::StaticTreeItem(groupName + " ");
            channelModel.rootItem()->appendChild(itemGroup);
            groupMap.insert(groupName, itemGroup);
        }

        auto channelItem = new ChannelItem;
        channelItem->setChannel(it.channel);
        groupMap[groupName]->appendChild(channelItem);
    }
}

void PanelChannel::setCurrentChannel(int channel)
{
    switch (ControlPanel::getInstance()->waveType()) {
    case WaveType::WaveSurface: ui->surface->setChecked(true);  break;
    case WaveType::WaveFlaw:    ui->flaw->setChecked(true);     break;
    default:                                                    break;
    }

    auto channelItem = channelModel.findItemAtLevel<2>([&](ChannelItem *item){
        return (item->channel() == channel);
    });

    if (channelItem) {
        ui->channels->setCurrentIndex(proxyModel->mapFromSource(channelItem->index()));
    }
}

void PanelChannel::on_flaw_toggled(bool checked)
{
    if (checked) {
        ControlPanel::getInstance()->setWaveType(WaveType::WaveFlaw);
    }
}

void PanelChannel::on_surface_toggled(bool checked)
{
    if (checked) {
        ControlPanel::getInstance()->setWaveType(WaveType::WaveSurface);
    }
}

void PanelChannel::on_filtreEdit_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        proxyModel->setFilterRegExp(QString());
    } else {
        proxyModel->setFilterRegExp("\\s$|^" + arg1);
    }
}

void PanelChannel::onChannelSelected(const QModelIndex &index)
{
    QVariant id = index.data();
    if (id.type() != QVariant::Int) {
        return;
    }

    ControlPanel::getInstance()->setCurrentChannel(id.toInt());
}
