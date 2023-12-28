#include "DefectTreeView.h"
#include "DefectItemModel.h"
#include "DialogDefectProperty.h"
#include "PanelCScan.h"
#include <Core/ICore.h>
#include <Core/Platform.h>
#include <Imaging/EditDelegate.h>
#include <Imaging/UTDocument.h>
#include <QDragMoveEvent>
#include <QHeaderView>
#include <QInputDialog>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QPainter>
#include <Waveform/PanelAScan.h>

static Q_LOGGING_CATEGORY(debug, "image.defect")

DefectTreeView::DefectTreeView(QWidget *parent) :
    QTableView(parent)
{
    Init();
}

/*!
 * \brief DefectTreeView::Init 初始化
 */
void DefectTreeView::Init()
{
    setAcceptDrops(true);
    setDragDropMode(NoDragDrop);                             // 暂时不允许拖放
    setHorizontalScrollMode(ScrollPerPixel);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setHighlightSections(false);

    setEditTriggers(NoEditTriggers);
    setSelectionBehavior(SelectRows);                        // 设置行选择
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSortingEnabled(true);
    setShowGrid(false);
    resizeColumnsToContents();

    connect( this, &QWidget::customContextMenuRequested, this, &DefectTreeView::OnCustomContextMenu );
    connect( this, &QAbstractItemView::doubleClicked, this, &DefectTreeView::OnDoubleClicked );
}

void DefectTreeView::SetModel(int layerID)
{
    DefectItemModel *defectModel = document()->GetDefectModel(layerID);
    if (defectModel == nullptr) {
        return;
    }
    setModel(defectModel);
    setSelectionModel(defectModel->selectionModel());
    int columnCount = model()->columnCount();

    for (int index = 0; index < columnCount; ++index) {
        setColumnWidth(index, 120);
    }
    // 给各列设置编辑框委托
    auto *editor = new EditDelegate(this);
    setItemDelegateForColumn(0, editor);
}

void DefectTreeView::setCurrentItem(int row, const QModelIndex &parent)
{
    auto *parentDefect = defectModel()->itemForIndex(parent);
    if (parentDefect != nullptr) {
        clearSelection();
        int rowCount = parentDefect->childCount();
        if ((rowCount > row) && (rowCount > 0)) {
            setCurrentIndex(model()->index(row, 0, parent) );
        } else if ((rowCount <= row) && (rowCount > 0)) {
            setCurrentIndex(model()->index(rowCount - 1, 0, parent) );
        } else if (rowCount <= 0) {
            setCurrentIndex(parent);
        }
    }
}

DefectItemModel *DefectTreeView::defectModel() const
{
    return dynamic_cast<DefectItemModel *>(model());
}

DefectItem *DefectTreeView::currentItem()
{
    QModelIndex index = currentIndex();
    if ( index.isValid() ) {
        return static_cast<DefectItem *>(index.internalPointer());
    }
     return nullptr;
}

void DefectTreeView::OnCustomContextMenu(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu;
    if (currentIndex().isValid()) {
        menu.addAction(tr("编辑"), this, &DefectTreeView::Edit);
        menu.addAction(tr("删除"), this, &DefectTreeView::deleteSelectedItem);
        menu.addAction(tr("注释"), this, &DefectTreeView::Comment);
        menu.addSeparator();
        menu.addAction(tr("查看缺陷图像"), this, &DefectTreeView::LocateView);
        menu.addAction(tr("移动到此缺陷位置"), this, &DefectTreeView::Locate);
        menu.addAction(tr("添加截图"), this, &DefectTreeView::AddImage);
        menu.addSeparator();
    }
    menu.addAction(tr("全部删除"), this, &DefectTreeView::Clear);
    menu.exec(QCursor::pos());
}

void DefectTreeView::OnDoubleClicked(const QModelIndex &index)
{
    auto *curItem = defectModel()->itemForIndex(index);
    if ((curItem) && index.isValid()) {
        Edit();
    }
}

void DefectTreeView::Edit()
{
    DefectItem *defect = currentItem();
    if (defect != nullptr ) {
        DialogDefectProperty dlg(defect);
        if (dlg.exec() == QDialog::Accepted) {
            defectModel()->reloadItem(defect);
        }
    }
}

void DefectTreeView::deleteSelectedItem()
{
    QModelIndexList indexes = selectItemsIndexes();
    if (indexes.isEmpty()) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定删除选中的缺陷吗?"), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel) {
        return;
    }
    foreach (const QModelIndex &index, indexes) {
        if (index.isValid()) {
            auto *item = defectModel()->itemForIndex(index);
            defectModel()->destroyItem(item);
        }
    }
}

void DefectTreeView::Comment()
{
    QModelIndexList indexes = selectItemsIndexes();
    if (indexes.isEmpty()) {
        return;
    }
    DefectItem *item = dynamic_cast<DefectItem *>(defectModel()->itemForIndex( indexes.value(0)));
    if (item) {
        DialogDefectProperty dlg(item);
        dlg.exec();
    }
}

/*!
 * \brief DefectTreeView::Clear 清空列表
 */
void DefectTreeView::Clear()
{
    if (defectModel() == nullptr) {
        return;
    }
    defectModel()->clear();
}

void DefectTreeView::LocateView()
{
    DefectItem *item = currentItem();
    if (item != nullptr) {
        qDebug() << "DefectTree: locating to " << item->defect().GetCenter().toPointF();
        QRectF rect = item->defect().minRect.boundingRect();
        emit DefectRectUpdated(rect.adjusted(-rect.width() - 0.5, -rect.height() - 0.5, rect.width() + 0.5, rect.height() + 0.5));
    }
}

void DefectTreeView::Locate()
{
    DefectItem *item = currentItem();
    if (item != nullptr) {
        qDebug() << "DefectTree: locating to " << item->defect().GetCenter().toPointF();
        emit RequestLocate(item->defect().GetCenter().toPointF());
    }
}

void DefectTreeView::AddImage()
{
    QList<PanelAScan *> panels = Core::ICore::mainWindow()->findChildren<PanelAScan *>();
    qDebug() << "Add image from AScan: " << panels;
    PanelAScan *panel = panels.value(0);
    if (panels.count() > 1) {
        QStringList titles;
        foreach (PanelAScan *panel, panels) {
            titles << panel->windowTitle();
        }
        bool ok = false;
        QString title = QInputDialog::getItem(this, QString(), tr("选择一个面板上的图像"), titles, 0, false, &ok);
        if (!ok) {
            return;
        }
        panel = panels.value(titles.indexOf(title));
    }
    if (panel) {
        QModelIndex index = currentIndex();
        auto *curItem = dynamic_cast<DefectItem *>(defectModel()->itemForIndex(index));
        auto *defect = dynamic_cast<DefectItem *>(curItem);
        if ((defect != nullptr) && index.isValid()) {
            UTDefect d = defect->defect();
            QList<QImage> images = d.screenshots();
            QPixmap pixmap = panel->RenderPixmap();
            // 在此添加坐标和时间
            PanelCScan *panelCScan = nullptr;
            foreach (PanelCScan *cscan, Core::ICore::mainWindow()->findChildren<PanelCScan *>()) {
                if (cscan->HasDefectTreeView(this)) {
                    panelCScan = cscan;
                    break;
                }
            }
            if (panelCScan) {
                UTLayer *layer = panelCScan->GetLayer();
                if (layer) {
                    const qreal ratio = 0.015;
                    const int minFont = 12;
                    QSize pixmapSize = panel->size();
                    int fontSize = qMax(minFont, int(pixmapSize.width() * ratio));
                    QPixmap newPixmap(pixmapSize.width(), pixmapSize.height() + fontSize * 2);
                    newPixmap.fill(Qt::white);
                    QPainter painter(&newPixmap);
                    painter.drawPixmap(QRect(QPoint(0, 0), pixmapSize), pixmap);

                    QFont font;
                    font.setPixelSize(fontSize);
                    painter.setFont(font);

                    ImageOptions options = layer->GetImageOptions();
                    UTDataFrame frame = panel->GetCurrentData();
                    QPointF point = options.MapToSource(frame, frame.Context.GetChannelID());
                    QString pointText(tr("(%1, %2)").arg(point.x()).arg(point.y()));
                    int posLength = painter.fontMetrics().width(pointText);
                    QString timeText = QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate);
                    int timeLength = painter.fontMetrics().width(timeText);
                    QRect posRect(pixmapSize.width() - posLength, pixmapSize.height(), posLength, fontSize);
                    QRect timeRect(pixmapSize.width() - timeLength, pixmapSize.height() + fontSize, timeLength, fontSize);
                    painter.drawText(posRect, pointText);
                    painter.drawText(timeRect, timeText);
                    painter.end();
                    pixmap = newPixmap;
                }
            }
            images << pixmap.toImage();
            d.setScreenshots(images);
            defect->setDefect(d);
        }
    }
}

QModelIndexList DefectTreeView::selectItemsIndexes()
{
    QModelIndexList indexes;
    if (defectModel() == nullptr) {
        return indexes;
    }
    QItemSelectionModel *selectionModel = defectModel()->selectionModel();
    if (selectionModel == nullptr) {
        return indexes;
    }
    QItemSelection selection = selectionModel->selection();
    indexes = selection.indexes();
    return indexes;
}
