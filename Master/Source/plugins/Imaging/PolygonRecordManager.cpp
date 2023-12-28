#include "PolygonRecordManager.h"
#include <Waveform/UTGraphicsScene.h>
#include <QAction>
#include <QLoggingCategory>

PolygonRecordManager::PolygonRecordManager(QGraphicsItemGroup *rootItem, QObject *parent) :
    QObject(parent),
    recordItem(new PolygonRecordItem(rootItem)),
    addPointOnLineAction(new QAction(tr("添加标记点"), this))
{
    recordItem->setTransform(QTransform::fromScale(1, -1));
    connect(recordItem, &PolygonRecordItem::RecordAddRequest, this, &PolygonRecordManager::OnRecordAddRequst);
    connect(addPointOnLineAction, &QAction::triggered, this, &PolygonRecordManager::OnAddPointOnLine);
}

/*! 重载的更新actions */
QList<QAction *> PolygonRecordManager::UpdateActions()
{
    QList<QAction *> actions;
    for (RecordLineItem *line: lineList) {
        if (!line->isVisible()) {
            break;
        }
        if (line->isSelected()) {
           actions.append(addPointOnLineAction);
           break;
        }
    }
    return actions;
}

/*! 获取标记的polygon区域 */
QPolygonF PolygonRecordManager::GetRecordPolygon()
{
    return polygon;
}

/*! 判断是是否已经标记过 */
bool PolygonRecordManager::IsRecord()
{
    if (polygon.isEmpty() || !parent()) {
        return false;
    }
    return true;
}

/*! 标记是否已经完成 */
bool PolygonRecordManager::IsRecordCompleted()
{
    if (polygon.isEmpty() || polygon.isClosed()) {
        return true;
    }
    return false;
}

/*! 添加到scene中（即启动多边形标记） */
void PolygonRecordManager::StartRecord()
{
    recordItem->grabMouse();
}

/*! 刷新绘图 */
void PolygonRecordManager::Refresh()
{
    int count = (polygon.isClosed() && polygon.count() > 1) ? polygon.count() - 1 : polygon.count();     // 关闭的polygon最后一个点与第一个点重复
    for (int index = 0; index < count; ++index) {
        // 每增加一个点，就增加一条线
        if (!pointList.value(index)) {
            auto *point = new GraphicsDotItem(recordItem);
            connect(point, SIGNAL(DotMoved(QPointF)), SLOT(OnItemMove(QPointF)));
            point->SetShape(GraphicsDotItem::SQUARE);
            point->SetSize(SIZE_RECORDPOINT);
            point->SetIfFillColor(true);
            point->SetPoint(polygon.value(index));
            point->SetDragEnabled(true);
            point->setZValue(1);
            pointList.insert(index, point);
        }
        if (!lineList.value(index)) {
            auto *lineItem = new RecordLineItem(recordItem);
            lineItem->setVisible(false);
            connect( lineItem, &RecordLineItem::contextMenuRequest, this, &PolygonRecordManager::onLineItemContextMenuRequest);
            lineList.insert(index, lineItem);
        }
        // 如果只有一个点，那么该直线不做显示
        pointList.value(index)->SetPoint(polygon.value(index));
        pointList.value(index)->setVisible(true);
        if (index == 0) {
            lineList.value(index)->setVisible(false);
        } else if (index == count - 1) {                                        // 最后一个点需要判断是否封闭
            if (polygon.isClosed()) {
                lineList.value(index - 1)->setVisible(true);
                lineList.value(index - 1)->setLine( QLineF( polygon.value(index - 1), polygon.value(index) ) );

                lineList.value(index)->setVisible(true);
                lineList.value(index)->setLine(QLineF( polygon.value(index), polygon.value(0) ));
            } else {
                lineList.value(index - 1)->setVisible(true);
                lineList.value(index - 1)->setLine( QLineF( polygon.value(index - 1), polygon.value(index) ) );
            }
        } else {
            lineList.value(index - 1)->setVisible(true);
            lineList.value(index - 1)->setLine( QLineF( polygon.value(index - 1), polygon.value(index) ) );
        }
    }
    recordItem->update();
}

void PolygonRecordManager::AddRecordPoint(const QPointF &pos, int index)
{
    if (index < 0) {
        polygon.append(pos);
    } else {
        polygon.insert(index, pos);
    }
    Refresh();
}

/*! 标记完成（完成之后需要将当前多边形封闭） */
void PolygonRecordManager::RecordingFinished()
{
    if (polygon.count() < 3) {
        qWarning() << "PolygonRecordManager: can't finish polygon, insufficient points";
        return;
    }
    if (!polygon.isClosed()) {
        polygon.append(polygon.value(0));
    }
    Refresh();

    recordItem->ungrabMouse();
    recordItem->update();

    emit recordCompleted();
}

/*! 清空选区 */
void PolygonRecordManager::Reset()
{
    auto *scene = qobject_cast<QGraphicsScene *>(parent());
    if (scene && scene->mouseGrabberItem() == recordItem) {
        recordItem->ungrabMouse();
    }
    polygon.clear();
    foreach (GraphicsDotItem *dot, pointList) {
        dot->setVisible(false);
    }
    foreach (RecordLineItem *line, lineList) {
        line->setVisible(false);
    }
}

/*! 设置标记的polygon */
void PolygonRecordManager::SetPolygon(const QPolygonF &points)
{
    Reset();
    StartRecord();
    polygon = points;
    RecordingFinished();
}

/*! 设置recordItem显示与否 */
void PolygonRecordManager::SetRecordVisible(bool visible)
{
    recordItem->setVisible(visible);
    if (visible) {
        if (!IsRecordCompleted()) {
            recordItem->grabMouse();
        }
    }
}

/*! 接受recordItem发送的添加点的请求 */
void PolygonRecordManager::OnRecordAddRequst(const QPointF &pos)
{
    AddRecordPoint(pos);
}

/*! 键盘按下事件的槽函数 */
void PolygonRecordManager::OnKeyPressed(int key)
{
    if (IsRecordCompleted()) {
        return;
    }
    if (key == Qt::Key_Return || key == Qt::Key_Enter || key == Qt::Key_Space) {
        RecordingFinished();
    } else if (key == Qt::Key_Escape) {

    }
}

void PolygonRecordManager::OnItemMove(const QPointF &pos)
{
    auto *dotItem = dynamic_cast<GraphicsDotItem *>(sender());
    if (!dotItem) {
        return;
    }
    int index = pointList.indexOf(dotItem);
    if (index == 0 && polygon.isClosed()) {
        polygon.replace(polygon.count() - 1, pos);
    }
    polygon.replace(index, pos);
    Refresh();
    emit recordPolygonChanged(GetRecordPolygon());
}

/*! 直线右键菜单的相应接口 */
void PolygonRecordManager::onLineItemContextMenuRequest(QGraphicsSceneContextMenuEvent *event)
{
    currentLine = qobject_cast<RecordLineItem *>(sender());
    currentPos = event->scenePos();
}

void PolygonRecordManager::OnAddPointOnLine()
{
    if (!currentLine) {
        return;
    }
    int index = lineList.indexOf(currentLine);
    if (index < 0) {
        return;
    }
    AddRecordPoint(currentPos, index + 1);
}
