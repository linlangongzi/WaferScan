#pragma once

#include <QGraphicsLineItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QObject>
#include <QPainter>

class RecordLineItem : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    explicit RecordLineItem(QGraphicsItem *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


    virtual QPainterPath shape() const;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    void SetInColor(QColor color);
    void SetBackLineShow(bool enabled);

signals:
    void contextMenuRequest(QGraphicsSceneContextMenuEvent *event);

private:

private:
    QTransform worldTrans;
    QColor inColor;
    bool backLineShow;
};
