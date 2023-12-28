#include "DialogImageOptions.h"
#include "DialogImageRecord.h"
#include "ui_DialogImageRecord.h"

QVariant RecordLayerItem::data(int column, int role) const
{
    if (column == Columns::LayerIndex && role == Qt::DisplayRole) {
        return index().row();
    } else if (column == Columns::LayerEnable && role == Qt::DisplayRole) {
        return config.enable ? QObject::tr("开启") : QObject::tr("关闭");
    } else if (column == Columns::LayerEnable && role == Qt::CheckStateRole) {
        return config.enable ? Qt::Checked : Qt::Unchecked;
    } else if (column == Columns::FeatureName && role == Qt::DisplayRole) {
        return config.featureName;
    } else if (column == Columns::FileName && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.fileName;
    } else if (column == Columns::LayerComment && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        return config.comment;
    }
    return QVariant();
}

bool RecordLayerItem::setData(int column, const QVariant &data, int role)
{
    if (column == Columns::LayerEnable && role == Qt::CheckStateRole) {
        config.enable = data.toBool();
    } else if (column == Columns::FileName && role == Qt::EditRole) {
        config.fileName = data.toString();
    } else if (column == Columns::LayerComment && role == Qt::EditRole) {
        config.comment = data.toString();
    } else {
        return false;
    }
    return true;
}

Qt::ItemFlags RecordLayerItem::flags(int column) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (column == Columns::FileName || column == Columns::LayerComment) {
        return flags | Qt::ItemIsEditable;
    } else if (column == Columns::LayerEnable) {
        return flags | Qt::ItemIsUserCheckable;
    } else {
        return flags;
    }
}

DialogImageRecord::DialogImageRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImageRecord)
{
    ui->setupUi(this);

    ui->layers->setModel(&m_layerModel);
    m_layerModel.setHeader(QStringList() << tr("序号") << tr("开关") << tr("特征值") << tr("文件名") << tr("备注"));
}

DialogImageRecord::~DialogImageRecord()
{
    delete ui;
}

void DialogImageRecord::setConfig(const RecordConfig &config)
{
    if (m_config == config) {
        return;
    }
    m_config = config;

    m_layerModel.clear();
    for (RecordLayerConfig layer: config.layers) {
        RecordLayerItem *item = new RecordLayerItem;
        item->config = layer;
        m_layerModel.rootItem()->appendChild(item);
    }

    for (int column = 0; column < m_layerModel.columnCount(QModelIndex()); ++column) {
        ui->layers->resizeColumnToContents(column);
    }

    ui->hasWaveform->setChecked(config.hasWaveform);
    ui->blockSize->setValue(config.blockSize);
    ui->fileSizeMax->setValue(config.fileSizeMax);
}

RecordConfig DialogImageRecord::config() const
{
    auto config = m_config;
    config.layers.clear();

    m_layerModel.rootItem()->forFirstLevelChildren([&](RecordLayerItem *item) {
        config.layers << item->config;
    });

    config.blockSize = ui->blockSize->value();
    config.fileSizeMax = ui->fileSizeMax->value();
    config.hasWaveform = ui->hasWaveform->isChecked();

    return config;
}

void DialogImageRecord::on_imageOptions_clicked()
{
    DialogImageOptions dlg;
    dlg.setOptions(m_config.imageOptions);
    if (dlg.exec() == QDialog::Accepted) {
        m_config.imageOptions = dlg.options();
    }
}

void DialogImageRecord::on_add_clicked()
{
    RecordLayerConfig config;
    config.featureName = tr("自定义");
    RecordLayerItem *item = new RecordLayerItem;
    item->config = config;
    m_layerModel.rootItem()->appendChild(item);
}

void DialogImageRecord::on_remove_clicked()
{
    QModelIndex currentIndex = ui->layers->currentIndex();
    RecordLayerItem *item = m_layerModel.itemForIndex(currentIndex);
    if (m_layerModel.rootItem() != item && item->config.feature < 0) {
        m_layerModel.destroyItem(item);
    }
}
