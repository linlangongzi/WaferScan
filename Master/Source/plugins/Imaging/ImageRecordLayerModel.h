#pragma once

#include <QPointer>
#include <utils/treemodel.h>
#include "ImageRecord.h"

class ImageRecordLayerItem : public Utils::TypedTreeItem<Utils::TreeItem, Utils::StaticTreeItem>
{
public:
    QVariant data(int column, int role) const override;

    int index = -1;
};

class ImageRecordLayerModel : public Utils::TreeModel<Utils::TreeItem, ImageRecordLayerItem>
{
    Q_OBJECT

public:
    ImageRecordLayerModel(QObject *parent = Q_NULLPTR);

    void setImageRecord(ImageRecord *record);
    int featureForIndex(const QModelIndex &index) const;

public slots:
    void refreshRecordConfig();

private:
    QPointer<ImageRecord> m_record;

    friend class ImageRecordLayerItem;
};
