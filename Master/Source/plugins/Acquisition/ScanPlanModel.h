#pragma once

#include <QAbstractItemModel>
#include <QIcon>
#include "ScanPlan.h"

class QItemSelectionModel;

class ScanPlanModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ScanPlanModel(QObject *parent = nullptr);
    ~ScanPlanModel() override;

    static ScanPlanModel *instance();

public:
    bool removeRows( int row, int count, const QModelIndex & parent = QModelIndex() ) override;
    bool moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild) override;
    QModelIndex parent(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    ScanPlan *scanPlanFromIndex(const QModelIndex &index) const;
    ScanPlan *insertNewScanPlan(const QJsonObject &obj, const QModelIndex &parent, int index = -1);
    QString newScanPlanName(ScanPlan::MotionType motionType);
    QString newScanPlanName(const QString &displayName);
    void reloadJson(ScanPlan *scanPlan, const QJsonObject &obj);
    int maxDepth() const;
    ScanPlan *root();
    QModelIndex indexOf(ScanPlan *scanPlan, int column = 0);
    QItemSelectionModel *selectionModel();

    void fromJson(const QJsonArray &array);
    QJsonArray toJson() const;
    void loadSettings();
    void saveSettings() const;

private:
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    QStringList mimeTypes() const override;
    static QString timeString(float value);
    static QString defaultName(ScanPlan::MotionType motionType);

private slots:
    void updateTotalTime() const;

private:
    static const int DEFAULT_MAX_DEPTH = 5;
    ScanPlan *m_root = nullptr;         //根节点
    QMap<QString, QIcon> icons;
    QVariantList headers;
    QItemSelectionModel *m_selectionModel;
};
