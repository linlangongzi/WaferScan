#pragma once

#include <QDialog>
#include <utils/treemodel.h>
#include "ImageRecord.h"
#include "ImageRecordLayer.h"

class RecordLayerItem : public Utils::TypedTreeItem<RecordLayerItem>
{
public:
    enum Columns
    {
        LayerIndex,
        LayerEnable,
        FeatureName,
        FileName,
        LayerComment
    };

    QVariant data(int column, int role) const override;
    bool setData(int column, const QVariant &data, int role) override;
    Qt::ItemFlags flags(int column) const override;

    RecordLayerConfig config;
};

namespace Ui {
class DialogImageRecord;
}

class DialogImageRecord : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(RecordConfig config READ config WRITE setConfig)

public:
    explicit DialogImageRecord(QWidget *parent = 0);
    ~DialogImageRecord();

    void setConfig(const RecordConfig &config);
    RecordConfig config() const;

private slots:
    void on_imageOptions_clicked();
    void on_add_clicked();
    void on_remove_clicked();

private:
    Ui::DialogImageRecord *ui;
    RecordConfig m_config;
    Utils::TreeModel<RecordLayerItem> m_layerModel;
};
