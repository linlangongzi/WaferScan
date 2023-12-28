#include "DefectItemModel.h"
#include "DefectItem.h"
#include <Core/Platform.h>
#include <Imaging/UTDocument.h>
#include <QItemSelectionModel>
#include <QLoggingCategory>
#include <QProgressDialog>

static Q_LOGGING_CATEGORY(debug, "image.defectModel")

DefectItemModel::DefectItemModel(QObject *parent) : Utils::TreeModel<DefectItemModelRoot, DefectItem>(parent)
{
    m_selectionModel = new QItemSelectionModel(this);
    setHeader({tr("面积(mm²)"), tr("长度(mm)"), tr("宽度(mm)"), tr("特征长度(mm)"),
                    tr("像素数"), tr("中心坐标"), tr("最大幅值(%)"), tr("缺陷占积比(%)"), tr("平均深度(mm)"),
                    tr("最大深度(mm)"), tr("最小深度(mm)"), tr("底面深度(mm)"), tr("注释")});
}

void DefectItemModel::sort(int column, Qt::SortOrder order)
{
    rootItem()->sortChildren([=] (const DefectItem *first, const DefectItem *second) {
        if (order == Qt::AscendingOrder) {
            return first->data(column, Qt::EditRole) > second->data(column, Qt::EditRole);
        }
        return first->data(column, Qt::EditRole) < second->data(column, Qt::EditRole);
    });
}

/*!
 * \brief DefectItemModel::FromDefectList 实现从UTDefectList中取数据，按照数据序列进行新建各级Item
 * \param list Defect数据列表
 */
void DefectItemModel::fromDefectList(const UTDefectList &list)
{
    auto *root = rootItem();
    if (root == nullptr) {
        return;
    }
    clear();
    foreach (const UTDefect &defect, list) {
        auto *item = new DefectItem();
        item->setDefect(defect);
        rootItem()->appendChild(item);
    }
}

/*!
 * \brief DefectItemModel::FromJson 从JSON中取值并设置Item的值
 * \param value JSON值
 */
void DefectItemModel::loadJson(const QJsonValue &value)
{
    beginResetModel();
    QJsonArray array = value.toArray();
    UTDefectList list;
    foreach (const QJsonValue &json, array) {
        list.append(UTDefect::fromJson(json));
    }
    appendDefectList(list);
    document()->setSaveFile(true);
    endResetModel();
}

/*!
 * \brief DefectItemModel::AppendDefectList 添加一系列defect
 * \param list 添加的defectList
 */
void DefectItemModel::appendDefectList(const UTDefectList &list)
{
    QProgressDialog progress(tr("正在添加缺陷"), tr("取消"), 0, list.count());
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    int index = 0;
    clear();

    foreach (const UTDefect &defect, list) {
        if (progress.wasCanceled()) {
            break;
        }
        progress.setValue(index++);
        auto *item = new DefectItem();
        item->setDefect(defect);
        rootItem()->appendChild(item);
    }
}

/*!
 * \brief DefectItemModel::GetSelectionModel
 * \return 返回当前ItemModel创建的SelectionMode
 */
QItemSelectionModel *DefectItemModel::selectionModel()
{
    return m_selectionModel;
}

/*!
 * \brief DefectItemModel::ToJson 反序列化成json
 * \return
 */
QJsonArray DefectItemModel::toJson() const
{
    QJsonArray json;
    forItemsAtLevel<1>([&] (DefectItem *item) {json << item->defect().toJson();});
    return json;
}

/*!
 * \brief DefectItemModel::ReloadItem 重新加载某一项
 * \param item 被重新加载的项
 */
void DefectItemModel::reloadItem(DefectItem *item)
{
    QModelIndex index = indexForItem(item);
    QModelIndex lefttop = createIndex(index.row(), 0, item);
    emit dataChanged(lefttop, lefttop);
}

int DefectItemModel::pixelCount() const
{
    int count = 0;
    forItemsAtLevel<1>([&](DefectItem *item) {
        count += item->defect().pixelCount;
    });
    return count;
}

/*!
 * \brief DefectItemModel::OnOriginChanged  原点改变的接口，需要所有项的坐标点
 */
void DefectItemModel::setOrigin(const QVector2D &origin)
{
    int row = rowCount(QModelIndex());
    if (row <= 0) {
        return;
    }
    forItemsAtLevel<1>([&origin] (DefectItem *item) ->void {
        item->origin = origin;
    });
}
