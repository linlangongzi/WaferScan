#include "ImageRecordLayerModel.h"
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "image.layerModel");

using namespace Utils;

QVariant ImageRecordLayerItem::data(int column, int role) const
{
    Q_UNUSED(column);
    if (role == Qt::DisplayRole) {
        auto *layerModel = qobject_cast<ImageRecordLayerModel *>(model());
        if (!layerModel || !layerModel->m_record) {
            return QVariant();
        }
        auto layer = layerModel->m_record->layerConfig(index);
        if (layer.feature >= 0) {
            return layerModel->m_record->recordConfig().header.featureName(layer.feature);
        } else {
            return QString("自定义特征值");
        }
    }
    return QVariant();
}

ImageRecordLayerModel::ImageRecordLayerModel(QObject *parent) :
    TreeModel<TreeItem, ImageRecordLayerItem>(parent)
{
}

void ImageRecordLayerModel::setImageRecord(ImageRecord *record)
{
    m_record = record;
    refreshRecordConfig();
}

int ImageRecordLayerModel::featureForIndex(const QModelIndex &index) const
{
    if (!index.isValid() || !index.parent().isValid()) {
        return -1;
    }
    auto *item = itemForIndexAtLevel<1>(index);
    if (item) {
        return item->index;
    }
    return -1;
}

void ImageRecordLayerModel::refreshRecordConfig()
{
    if (!m_record) {
        return;
    }

    rootItem()->removeChildren();
    const auto layers = m_record->recordConfig().layers;
    qCDebug(debug) << "Adding" << layers.count() << "layers";
    for (int index = 0; index < layers.size(); ++index) {
        auto *item = new ImageRecordLayerItem;
        item->index = index;
        rootItem()->appendChild(item);
    }
}
