#include "DefectManager.h"
#include "DefectGraphicsItem.h"
#include "DefectItemModel.h"
#include "DialogDefectProperty.h"
#include <Core/Platform.h>
#include <Imaging/UTDocument.h>
#include <QLoggingCategory>
#include <QMessageBox>
#include <Waveform/UTGraphicsScene.h>

static Q_LOGGING_CATEGORY(debug, "image.defect")

DefectManager::DefectManager(QGraphicsItem *rootItem, QObject *parent) :
    CScanGraphicsItemManager(parent)
{
    root = new QGraphicsPathItem(rootItem);
    connect(rootItem->scene(), &QGraphicsScene::selectionChanged, this, &DefectManager::onGraphicsSelectionChanged);
}

/*! 清空整个scene */
void DefectManager::clear()
{
    qDeleteAll(itemMap);
    itemMap.clear();
}

void DefectManager::setVisible(bool visible)
{
    isVisible = visible;
    foreach (DefectGraphicsItem *defectItem, itemMap) {
        defectItem->setVisible(visible);
    }
}

void DefectManager::updateScene()
{
    clear();
    auto *root = model->rootItem();
    if (root == nullptr) {
        return;
    }
    onRowsInserted(QModelIndex(), 0, root->childCount() - 1);
}

void DefectManager::ConnectToLayer(int layerID)
{
    CScanGraphicsItemManager::ConnectToLayer(layerID);
    // 更新model
    model = document()->GetDefectModel(layerID);
    if (model != nullptr) {
        QItemSelectionModel *selectionModel = model->selectionModel();
        connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &DefectManager::onItemSelectionChanged);
        connect(model, &DefectItemModel::rowsAboutToBeRemoved, this, &DefectManager::onItemRemoved);
        connect(model, &DefectItemModel::modelReset, this, &DefectManager::updateScene);
        connect(model, &DefectItemModel::rowsInserted, this, &DefectManager::onRowsInserted);
    }
}

void DefectManager::DisconnectToLayer(int layerID)
{
    CScanGraphicsItemManager::DisconnectToLayer(layerID);
    // 断开原来model的连接
    if (model != nullptr) {
        QItemSelectionModel *selectionModel = model->selectionModel();
        disconnect(selectionModel, nullptr, this, nullptr);
        disconnect(model, nullptr, this, nullptr);
    }
}

/*! 根据图元选中状态，更新缺陷选择列表 */
void DefectManager::onGraphicsSelectionChanged()
{
    if (!(scene && model && isVisible)) {    // 只有在缺陷显示的时候，才更新列表
        return;
    }
    QItemSelection selection;
    for (QGraphicsItem *graphicsItem: scene->selectedItems()) {
        auto *item = dynamic_cast<DefectGraphicsItem *>(graphicsItem);
        if (item == nullptr) {
            continue;
        }
        DefectItem *defectItem = item->GetDefectItem();
        QModelIndex index = model->indexForItem(defectItem);
        selection << QItemSelectionRange(index);
    }
    // 如果没有被选中的defect项，则需要将model中的选中状态修改掉
    model->selectionModel()->select(selection, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
}

QModelIndexList DefectManager::selectedItems()
{
    QModelIndexList indexes;
    if (model == nullptr) {
        return indexes;
    }
    QItemSelectionModel *pSelectionModel = model->selectionModel();
    if (pSelectionModel == nullptr) {
        return indexes;
    }
    QItemSelection selection = pSelectionModel->selection();
    indexes = selection.indexes();
    return indexes;
}

/*!
 * \brief DefectManager::ItemSelectionChanged 选择项改变的槽函数
 * \param selected 当前选中的项
 * \param deselected 原来选中的项
 */
void DefectManager::onItemSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (model == nullptr) {
        return;
    }
    foreach (const QModelIndex &index, deselected.indexes()) {
        auto *defect = dynamic_cast<DefectItem *>(model->itemForIndex(index));
        DefectGraphicsItem *item = itemMap.value(defect);
        if (item) {
            item->setSelected(false);
        }
    }
    foreach (const QModelIndex &index, selected.indexes()) {
        auto *defect = dynamic_cast<DefectItem*>(model->itemForIndex(index));
        DefectGraphicsItem *item = itemMap.value(defect);
        if (item) {
            item->setSelected(true);
        }
    }
}

void DefectManager::onRowsInserted(const QModelIndex &parentIndex, int first, int last)
{
    if (model == nullptr) {
        return;
    }
    for (int index = first; index <= last; ++index) {
        QModelIndex rowIndex = model->index(index, 0, parentIndex);
        auto *defectItem = dynamic_cast<DefectItem *>(model->itemForIndex(rowIndex));
        if (defectItem == nullptr) {
            qCCritical(debug) << "Invalid defectItem pointer";
            continue;
        }
        if (itemMap.contains(defectItem)) {
            continue;
        }
        auto *item = new DefectGraphicsItem(defectItem);
        item->setParentItem(root);
        item->setVisible(isVisible);
        item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        itemMap.insert(defectItem, item);
    }
}

void DefectManager::onItemRemoved(const QModelIndex &parent, int first, int last)
{
    if (model == nullptr) {
        return;
    }
    Q_ASSERT(parent.model() == model || !parent.isValid());
    for (int row = last; row >= first; --row) {
        QModelIndex rowIndex = model->index(row, 0 , parent);
        auto *treeItem = model->itemForIndex(rowIndex);
        DefectItem *item = dynamic_cast<DefectItem *>(treeItem);
        if (item == nullptr) {
            qCDebug(debug) << "Selected Item doesn't exits" << item << treeItem;
            return;
        }
        DefectGraphicsItem *graphicItem = itemMap.value(item);
        scene->removeItem(graphicItem);
        itemMap.remove(item);
        delete graphicItem;
        graphicItem = nullptr;
    }
}
