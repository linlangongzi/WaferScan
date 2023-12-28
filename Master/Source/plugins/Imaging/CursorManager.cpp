#include "CursorManager.h"
#include <QDebug>
#include <QGraphicsView>
#include <Waveform/GraphicsInvertedLine.h>
#include <Waveform/UTGraphicsScene.h>
#include <allrising/QtExtend.h>

CursorManager::CursorManager(QGraphicsItem *rootItem, QObject *parent) :
    AbstractGraphicsItemManager(parent),
    verticalLine(new GraphicsInvertedLine),
    horizontalLine(new GraphicsInvertedLine),
    orientations(Qt::Horizontal | Qt::Vertical)
{
    verticalLine->setPen(QPen(Qt::black, 0));
    verticalLine->setZValue(1);

    horizontalLine->setPen(QPen(Qt::black, 0));
    horizontalLine->setZValue(1);

    rootItem->scene()->addItem(verticalLine);
    rootItem->scene()->addItem(horizontalLine);
}

/*! 设置光标显示方向 */
void CursorManager::SetCursorOrientations(Qt::Orientations arg)
{
    orientations = arg;
    updateVisibility();
}

void CursorManager::SetCursorPos(const QPointF &scenePos)
{
    auto *scene = verticalLine->scene();
    if (scene == nullptr) {
        return;
    }
    if (!qIsFinite(scenePos)) {
        verticalLine->setLine(QLineF());
        horizontalLine->setLine(QLineF());
    } else {
        QRectF boundary = scene->sceneRect();                                   /*!< 场景边界 */
        if (!qIsFinite(boundary.width()) || !qIsFinite(boundary.height())) {    /* 当场景尺寸为无穷时，取最大视口作为场景边界 */
            boundary = QRectF();
            foreach (QGraphicsView *view, scene->views()) {
                boundary = boundary.united(view->mapToScene(view->rect()).boundingRect());
            }
        }
        verticalLine->setLine(scenePos.x(), boundary.top(), scenePos.x(), boundary.bottom());
        horizontalLine->setLine(boundary.left(), scenePos.y(), boundary.right(), scenePos.y());
    }
    updateVisibility();
}

void CursorManager::updateVisibility()
{
    verticalLine->setVisible(orientations & Qt::Vertical);
    horizontalLine->setVisible(orientations & Qt::Horizontal);
}
