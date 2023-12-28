#ifndef DIALOGGLOBALCONFIG_H
#define DIALOGGLOBALCONFIG_H

#include <QDialog>
#include <utils/treemodel.h>
#include <QSortFilterProxyModel>
#include <Wheel/ControlPanel.h>

class QListWidgetItem;

class ChannelConfigItem : public Utils::TypedTreeItem<ChannelConfigItem>
{
    typedef FeatureFilterConfig::FeatureType FeatureType;

public:
    enum Columns {
        ColChannel,
        ColRemoveFront,
        ColRemoveBack,
        ColRemoveAfterSwitch,
        ColAverage,
        ColFrameNumber,
        ColFeatureType,
        ColMergeType,
        ColInstrument,
        ColAecCard,
        ColHardChannel,
        ColSensor,
        ColLocation,
        ColPosition,
        ColJudgments,
        ColFeatureFilter
    };

    QVariant data(int column, int role) const override;
    bool setData(int column, const QVariant &data, int role) override;
    Qt::ItemFlags flags(int column) const override;

    ChannelConfig config;
};

namespace Ui {
class DialogGlobalConfig;
}

class DialogGlobalConfig : public QDialog
{
    Q_OBJECT

    typedef ChannelConfigItem::Columns Columns;

public:
    explicit DialogGlobalConfig(QWidget *parent = 0);
    ~DialogGlobalConfig();
    GlobalConfigOptions configOptions() const;
    void setConfigOptions(GlobalConfigOptions options);

private slots:
    void on_add_clicked();
    void on_remove_clicked();
    void on_copy_clicked();
    void on_addLayout_clicked();
    void on_removeLayout_clicked();
    void on_wheelSetLayouts_itemDoubleClicked(QListWidgetItem *item);
    void on_addChannelGroup_clicked();
    void on_removeChannelGroup_clicked();
    void on_channelGroups_itemDoubleClicked(QListWidgetItem *item);
    void on_channels_clicked(const QModelIndex &index);
    void on_channels_doubleClicked(const QModelIndex &index);
    void on_clear_clicked();
    void on_load_clicked();
    void on_browseTemplate_clicked();
    void on_browseDocument_clicked();
    void on_autoReport_clicked(bool checked);

private:
    Ui::DialogGlobalConfig *ui;
    Utils::TreeModel<ChannelConfigItem, ChannelConfigItem> channelModel;
    QSortFilterProxyModel *proxyModel;
};

#endif // DIALOGGLOBALCONFIG_H
