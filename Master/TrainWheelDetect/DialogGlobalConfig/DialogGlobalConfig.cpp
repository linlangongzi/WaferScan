#include "DialogGlobalConfig.h"
#include "ui_DialogGlobalConfig.h"
#include "DialogFeatureFilters.h"
#include "DialogTrainConfig.h"
#include "arsCore/Platform.h"
#include <BScanView/DialogChannelGroup.h>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QVariant>
#include <allrising/Util.h>

QVariant ChannelConfigItem::data(int column, int role) const
{
    if (column == ColChannel && role == Qt::DisplayRole) {
        return config.channel;
    } else if (column == ColRemoveFront && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.removeFront * 100.0;
    } else if (column == ColRemoveBack && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.removeBack * 100.0;
    } else if (column == ColRemoveAfterSwitch && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.removeAfterSwitch;
    } else if (column == ColAverage && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.averageSize;
    } else if (column == ColFrameNumber && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.frameNumber;
    } else if (column == ColFeatureType && (role == Qt::DisplayRole)) {
        switch (config.featureType) {
        case FeatureType::GateITime:    return QObject::tr("I  闸门 时间");
        case FeatureType::GateIAmp:     return QObject::tr("I  闸门 幅度");
        case FeatureType::GateATime:    return QObject::tr("A  闸门 时间");
        case FeatureType::GateAAmp:     return QObject::tr("A  闸门 幅度");
        case FeatureType::GateBTime:    return QObject::tr("B  闸门 时间");
        case FeatureType::GateBAmp:     return QObject::tr("B  闸门 幅度");
        case FeatureType::GateI2Time:   return QObject::tr("I2 闸门 时间");
        case FeatureType::GateI2Amp:    return QObject::tr("I2 闸门 幅度");
        default:                        return QObject::tr("无效类型");
        }
    } else if (column == ColFeatureType && role == Qt::EditRole) {
        return config.featureType;
    } else if (column == ColMergeType && role == Qt::DisplayRole) {
        switch (config.merge) {
        case ChannelConfig::MergeType::Min: return QObject::tr("最小值");
        case ChannelConfig::MergeType::Max: return QObject::tr("最大值");
        default:                                  return QObject::tr("");
        }
    } else if (column == ColMergeType && role == Qt::EditRole) {
        return config.merge;
    } else if (column == ColInstrument && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.instrumentId;
    } else if (column == ColAecCard && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.aecCardId;
    } else if (column == ColHardChannel && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.aecChannel;
    } else if (column == ColSensor && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.sensorId;
    } else if (column == ColLocation && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.location;
    } else if (column == ColPosition && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.position;
    } else if (column == ColJudgments && role == Qt::DisplayRole) {
        QString text;
        for (auto filter: config.judgments) {
            text.append(QObject::tr("%1(%2,%3) ").arg(filter.featureType).arg(filter.min).arg(filter.max));
        }
        return text;
    } else if (column == ColFeatureFilter && role == Qt::DisplayRole) {
        QString text;
        for (auto filter: config.featureFilters) {
            text.append(QObject::tr("%1(%2,%3) ").arg(filter.featureType).arg(filter.min).arg(filter.max));
        }
        return text;
    }

    return QVariant();
}

bool ChannelConfigItem::setData(int column, const QVariant &data, int role)
{
    if (column == ColChannel && role == Qt::EditRole) {
        config.channel = data.toInt();
    } else if (column == ColRemoveFront && role == Qt::EditRole) {
        config.removeFront = qBound(0.0, data.toDouble(), 100.0) * 0.01;
    } else if (column == ColRemoveBack && role == Qt::EditRole) {
        config.removeBack = qBound(0.0, data.toDouble(), 100.0) * 0.01;
    } else if (column == ColRemoveAfterSwitch && role == Qt::EditRole) {
        config.removeAfterSwitch = qMax(0, data.toInt());
    } else if (column == ColAverage && role == Qt::EditRole) {
        config.averageSize = qMax(1, data.toInt());
    } else if (column == ColFrameNumber && role == Qt::EditRole) {
        config.frameNumber = qMax(0, data.toInt());
    } else if (column == ColFeatureType && role == Qt::EditRole) {
        config.featureType = FeatureType(data.toInt());
    } else if (column == ColMergeType && role == Qt::EditRole) {
        config.merge = ChannelConfig::MergeType(qMin(1, qMax(0, data.toInt())));
    } else if (column == ColInstrument && role == Qt::EditRole) {
        config.instrumentId = qMax(1, data.toInt());
    } else if (column == ColAecCard && role == Qt::EditRole) {
        config.aecCardId = qMax(0, data.toInt());
    } else if (column == ColHardChannel && role == Qt::EditRole) {
        config.aecChannel = qMax(0, data.toInt());
    } else if (column == ColSensor && role == Qt::EditRole) {
        config.sensorId = data.toInt();
    } else if (column == ColLocation && role == Qt::EditRole) {
        config.location = data.toInt();
    } else if (column == ColPosition && role == Qt::EditRole) {
        config.position = data.toDouble();
    } else {
        return false;
    }

    return true;
}

Qt::ItemFlags ChannelConfigItem::flags(int column) const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (column == ColChannel || column == ColFeatureFilter || column == ColJudgments) {
        return flags;
    } else {
        return flags | Qt::ItemIsEditable;
    }
}

DialogGlobalConfig::DialogGlobalConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGlobalConfig)
{
    ui->setupUi(this);
    channelModel.setHeader(QStringList() << tr("通道编号")
                                         << tr("前端过滤(%)")
                                         << tr("后端过滤(%)")
                                         << tr("切换降噪")
                                         << tr("移动平均")
                                         << tr("最低帧数")
                                         << tr("特征值(0~7)")
                                         << tr("归并(0~1)")
                                         << tr("仪器")
                                         << tr("AEC卡")
                                         << tr("硬通道")
                                         << tr("传感器")
                                         << tr("位置号")
                                         << tr("安装位置(mm)")
                                         << tr("判伤原则")
                                         << tr("特征值过滤"));

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(&channelModel);
    ui->channels->setModel(proxyModel);

    // 暂时隐藏ChannelConfig的复制功能控件
    ui->labelRaw->hide();
    ui->channelRaw->hide();
    ui->labelStart->hide();
    ui->channelStart->hide();
    ui->labelEnd->hide();
    ui->channelEnd->hide();
    ui->labelSpace->hide();
    ui->channelSpace->hide();
    ui->copy->hide();
}

DialogGlobalConfig::~DialogGlobalConfig()
{
    delete ui;
}

GlobalConfigOptions DialogGlobalConfig::configOptions() const
{
    GlobalConfigOptions config;
    channelModel.forItemsAtLevel<1>([&](ChannelConfigItem *item) {
        config.channelConfig << item->config;
    });

    int itemCount = ui->wheelSetLayouts->count();
    for (int typeIndex = 0; typeIndex < itemCount; ++typeIndex) {
        config.wheelSetLayouts << ui->wheelSetLayouts->item(typeIndex)->data(Qt::UserRole).value<WheelSetLayout>();
    }

    QListWidgetItem *item = ui->wheelSetLayouts->currentItem();
    if (item && item->isSelected()) {
        config.trainType = item->text();
    } else {
        config.trainType = QString();
    }

    QList<ChannelGroup> groupConfigs;
    int groupCount = ui->channelGroups->count();
    for (int index = 0; index < groupCount; ++index) {
        groupConfigs << ui->channelGroups->item(index)->data(Qt::UserRole).value<ChannelGroup>();
    }

    config.channelGroups = groupConfigs;
    config.plcOpen = ui->plcOpen->isChecked();
    config.judgeOpen = ui->judge->isChecked();
    config.autoReport = ui->autoReport->isChecked();
    config.documentPath = ui->documentPath->text();
    config.templatePath = ui->templatePath->text();
    config.wheelConfig.rate = ui->rate->value();
    config.wheelConfig.radius = ui->radius->value();
    config.wheelConfig.average = ui->average->value();
    config.wheelConfig.standard = ui->standard->value();

    return config;
}

void DialogGlobalConfig::setConfigOptions(GlobalConfigOptions options)
{
    channelModel.clear();
    ui->wheelSetLayouts->clear();

    ui->wheelSetLayouts->clearSelection();
    for (auto wheelSetLayout: options.wheelSetLayouts) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(wheelSetLayout.trainType);
        item->setData(Qt::UserRole, QVariant::fromValue(wheelSetLayout));
        ui->wheelSetLayouts->addItem(item);

        if (item->text() == options.trainType) {
            ui->wheelSetLayouts->setCurrentItem(item);
        }
    }

    for (auto config: options.channelConfig) {
        ChannelConfigItem *item = new ChannelConfigItem;
        item->config = config;
        channelModel.rootItem()->appendChild(item);
    }

    for (int index = 0; index < channelModel.columnCount(QModelIndex()); ++index) {
        ui->channels->resizeColumnToContents(index);
    }

    ui->channelGroups->clear();
    for (auto groupConfig: options.channelGroups) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(groupConfig.name);
        item->setData(Qt::UserRole, QVariant::fromValue(groupConfig));
        ui->channelGroups->addItem(item);
    }

    ui->plcOpen->setChecked(options.plcOpen);
    ui->judge->setChecked(options.judgeOpen);
    ui->autoReport->setChecked(options.autoReport);
    ui->documentPath->setText(options.documentPath);
    ui->templatePath->setText(options.templatePath);
    ui->channels->setColumnWidth(0, 80);
    ui->rate->setValue(options.wheelConfig.rate);
    ui->radius->setValue(options.wheelConfig.radius);
    ui->average->setValue(options.wheelConfig.average);
    ui->standard->setValue(options.wheelConfig.standard);
}

void DialogGlobalConfig::on_add_clicked()
{
    bool ok = false;
    int channel = QInputDialog::getInt(this, "", tr("输入通道号："), 0, 0, 30000, 1, &ok);
    if (!ok) {
        return;
    }

    /* 检测有没有重复的通道号 */
    auto *channelItem = channelModel.findItemAtLevel<1>([&](ChannelConfigItem *item) {
        return (item->config.channel == channel);
    });

    if (channelItem) {
        QMessageBox::warning(this, QString(), tr("通道号 %1 已存在").arg(channel));
        return;
    }

    ChannelConfig config;
    ChannelConfigItem *item = new ChannelConfigItem;
    config.channel = channel;
    item->config = config;
    channelModel.rootItem()->appendChild(item);
}

void DialogGlobalConfig::on_remove_clicked()
{
    QModelIndex currentIndex = proxyModel->mapToSource(ui->channels->currentIndex());
    ChannelConfigItem *item = channelModel.itemForIndex(currentIndex);
    if (item) {
        channelModel.destroyItem(item);
    }
}

void DialogGlobalConfig::on_copy_clicked()
{
    const int probeAreaMaxNumber = 240;
    QModelIndex currentIndex = proxyModel->mapToSource(ui->channels->currentIndex());
    ChannelConfigItem *itemRaw = channelModel.itemForIndex(currentIndex);
    if (!itemRaw) {
        QMessageBox::warning(this, tr("参数复制"), tr("源通道无效"));
        return;
    }

    int raw = ui->channelRaw->value();
    int start = ui->channelStart->value();
    int end = ui->channelEnd->value();
    int space = ui->channelSpace->value();

    QVector<int> channels;
    for (int index = start; index <= end; index += space + 1) {
        channels << index;
    }

    if ((end < start) ||
        (end - start > probeAreaMaxNumber) ||
        (qAbs(end - raw) > probeAreaMaxNumber)) {
        QMessageBox::warning(this, tr("参数复制"), tr("您输入的通道号错误,请确保源,首,尾通道号为相同探头区域"));
        return;
    }

    channelModel.rootItem()->forFirstLevelChildren([&](ChannelConfigItem *item) {
        int channel = item->config.channel;
        if (channels.contains(channel)) {
            if (channel != raw) {
                item->config = itemRaw->config;
                item->config.channel = channel;
                channels.removeOne(channel);
                item->update();
            } else {
                channels.removeOne(channel);
            }
        }
    });

    if (channels.size() > 0) {
        QMessageBox::StandardButton bt = QMessageBox::information(this,
                                                                  tr("参数复制"),
                                                                  tr("是否创建列表中不存在的通道并复制"),
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (bt == QMessageBox::Yes) {
            for (auto channel: channels) {
                ChannelConfigItem *item = new ChannelConfigItem;
                ChannelConfig config = itemRaw->config;
                config.channel = channel;
                item->config = config;
                channelModel.rootItem()->appendChild(item);
            }
        } else {
            QMessageBox::information(this,
                                     tr("参数复制"),
                                     tr("成功将通道 %1 中的配置复制到 %2~%3 的部分通道中").arg(itemRaw->config.channel).arg(start).arg(end));
            return;
        }
    }

    QMessageBox::information(this,
                             tr("参数复制"),
                             tr("成功将通道 %1 的配置复制到了 %2~%3 的所有通道中").arg(itemRaw->config.channel).arg(start).arg(end));
}

void DialogGlobalConfig::on_addLayout_clicked()
{
    bool ok = false;
    const QString trainType = QInputDialog::getText(this, QString(), tr("车型:"), QLineEdit::Normal, "", &ok);
    if (!ok || trainType.isEmpty()) {
        return;
    }

    /* 检测重复车型 */
    int itemCount = ui->wheelSetLayouts->count();
    for (int index = 0; index < itemCount; ++index) {
        QString type = ui->wheelSetLayouts->item(index)->text();
        if (type == trainType) {
            QMessageBox::information(this, tr("添加配置"), tr("车型：%1 已存在").arg(type), QMessageBox::Yes);
            return on_addLayout_clicked();
        }
    }

    QListWidgetItem *item = new QListWidgetItem;
    item->setText(trainType);
    item->setData(Qt::UserRole, trainType);
    ui->wheelSetLayouts->addItem(item);
}

void DialogGlobalConfig::on_removeLayout_clicked()
{
    delete ui->wheelSetLayouts->currentItem();
}

void DialogGlobalConfig::on_wheelSetLayouts_itemDoubleClicked(QListWidgetItem *item)
{
    DialogTrainConfig dlg;
    WheelSetLayout config = item->data(Qt::UserRole).value<WheelSetLayout>();
    config.trainType = item->text();
    dlg.setConfig(config);

    if (dlg.exec() == QDialog::Accepted) {
        item->setText(dlg.config().trainType);
        item->setData(Qt::UserRole, QVariant::fromValue(dlg.config()));
    }
}

void DialogGlobalConfig::on_addChannelGroup_clicked()
{
    bool ok = false;
    const QString groupName = QInputDialog::getText(this, QString(), tr("添加通道组"), QLineEdit::Normal, "", &ok);
    if (!ok) {
        return;
    }

    ChannelGroup group;
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(groupName);
    item->setData(Qt::UserRole, QVariant::fromValue(group));
    ui->channelGroups->addItem(item);
}

void DialogGlobalConfig::on_removeChannelGroup_clicked()
{
    delete ui->channelGroups->currentItem();
}

void DialogGlobalConfig::on_channelGroups_itemDoubleClicked(QListWidgetItem *item)
{
    DialogChannelGroup dialog;
    dialog.setConfig(item->data(Qt::UserRole).value<ChannelGroup>());
    if (dialog.exec() == QDialog::Accepted) {
        item->setData(Qt::UserRole, QVariant::fromValue(dialog.config()));
    }
}

void DialogGlobalConfig::on_channels_clicked(const QModelIndex &index)
{
    QModelIndex currentIndex = proxyModel->mapToSource(index);
    ChannelConfigItem *itemRaw = channelModel.itemForIndex(currentIndex);
    if (!itemRaw) {
        return;
    }

    ui->channelRaw->setValue(itemRaw->config.channel);
}

void DialogGlobalConfig::on_channels_doubleClicked(const QModelIndex &index)
{
    int type = -1;
    switch (index.column()) {
    case Columns::ColFeatureFilter: type = 0; break;
    case Columns::ColJudgments:     type = 1; break;
    default:                                  return;
    }

    QModelIndex modelIndex = proxyModel->mapToSource(index);
    DialogFeatureFilters dlg;
    ChannelConfigItem *configItem = channelModel.itemForIndex(modelIndex);
    QList<FeatureFilterConfig> &config = type ? configItem->config.judgments : configItem->config.featureFilters;
    dlg.setConfig(config);
    if (dlg.exec() == QDialog::Accepted) {
        config = dlg.config();
        configItem->update();
    }
}

void DialogGlobalConfig::on_clear_clicked()
{
    ui->wheelSetLayouts->clearSelection();
}

void DialogGlobalConfig::on_load_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("文件读取"), tr("./"), "(*.csv)");
    static const char delimeter = ',';

    UTStreamFile file;
    file.setFileName(path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("文件读取"), tr("%1 打开失败").arg(path));
        return;
    }

    QTextStream stream(&file);
    QStringList names = stream.readLine().split(delimeter);
    for (auto &name: names) {
        name = name.section(" ", -1);
    }

    int columnChannel = names.indexOf("channel");
    QList<ChannelConfig> configs;
    const auto control = controlPanel();
    const auto channelConfigs = control->getChannelConfigs();

    while (!stream.atEnd()) {
        QStringList data = stream.readLine().split(delimeter);
        bool ok = false;
        int channel = data.value(columnChannel).toInt(&ok);
        if (!ok) {
            continue;
        }

        ChannelConfig config = channelConfigs[channel];
        for (int index = 0; index < names.count(); ++index) {
            QString name = names.value(index);

            if (name == "judgments" || name == "featureFilters") {
                QList<FeatureFilterConfig> filters;

                for (auto filter: data.value(index).split(' ')) {
                    if (filter.isEmpty()) {
                        continue;
                    }

                    filters << FeatureFilterConfig::fromString(filter);
                }

                MetaType::writeGadgetProperty(config, name, QVariant::fromValue(filters));
            } else {
                MetaType::writeGadgetProperty(config, name, data.value(index));
            }
        }

        configs << config;
    }

    GlobalConfigOptions globalConfig = control->getGlobalConfig();
    globalConfig.channelConfig = configs;
    setConfigOptions(globalConfig);
}

void DialogGlobalConfig::on_browseTemplate_clicked()
{
    const QString path = QFileDialog::getOpenFileName(this, QString(), tr("选择报表默认模板"), tr("(*.xlsx)"));
    QFileInfo info(path);
    if (!info.isReadable()) {
        QMessageBox::information(this, QString(), tr("无效的模板路径"));
        return;
    }

    ui->templatePath->setText(path);
}

void DialogGlobalConfig::on_browseDocument_clicked()
{
    const QString path = QFileDialog::getExistingDirectory(this, QString(), tr("选择数据保存路径"));
    if (path.isEmpty()) {
        return;
    }

    QDir dir(path);
    ui->documentPath->setText(dir.absoluteFilePath("{date}/{train}-{date}-{time}"));
}

void DialogGlobalConfig::on_autoReport_clicked(bool checked)
{
    const auto control = controlPanel();
    auto config = control->getGlobalConfig();
    if (config.autoReport != checked) {
        config.autoReport = checked;
        control->setGlobalConfig(config);
    }
}
