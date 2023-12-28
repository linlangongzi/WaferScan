#include "UTGraphicsScene.h"
#include <QPen>
#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QPainter>
#include <QKeyEvent>

UTGraphicsScene::UTGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

/*! 右键菜单事件 */
void UTGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsScene::contextMenuEvent(event);
    actions.clear();
    emit contextMenuRequest();
    emit contextMenuTriggered(actions);
}

/*! 接收各个manager的右键菜单 */
void UTGraphicsScene::onManagerContextMenuTriggered(const QList<QAction *> &actionList)
{
    actions.append(actionList);
}
