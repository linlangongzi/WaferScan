#pragma once

#include <QObject>
#include <QGraphicsItemGroup>
#include <QKeyEvent>
#include <QGraphicsSceneContextMenuEvent>
#include "Imaging/GraphicsDotItem.h"
#include "Imaging/RecordLineItem.h"

class PolygonRecordItem :  public QGraphicsObject
{
    Q_OBJECT
public:
    explicit PolygonRecordItem(QGraphicsItem * parent = 0) :
        QGraphicsObject(parent)
    {
    }

public:
    virtual QRectF boundingRect() const
    {
        QRectF rect;
        foreach (QGraphicsItem *item, childItems())
        {
            rect = rect.united(item->boundingRect());
        }
        return rect;
    }

signals:
    void RecordAddRequest(const QPointF &pos);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        QGraphicsObject::mousePressEvent(event);
        setFocus(Qt::MouseFocusReason);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        QGraphicsObject::mouseReleaseEvent(event);
        if (event->button() == Qt::LeftButton)
        {
            emit RecordAddRequest(event->pos());
        }
    }

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0)
    {
        Q_UNUSED(painter);
        Q_UNUSED(option)
        Q_UNUSED(widget);
    }

private:
};

class PolygonRecordManager : public QObject
{
    Q_OBJECT
public:
    explicit PolygonRecordManager(QGraphicsItemGroup *rootItem, QObject *parent = nullptr);

signals:
    void recordCompleted();
    void recordPolygonChanged(const QPolygonF &polygon);

public:
    virtual QList<QAction *> UpdateActions();
    QPolygonF GetRecordPolygon();
    bool IsRecord();
    bool IsRecordCompleted();

public slots:
    void StartRecord();
    void AddRecordPoint(const QPointF &pos, int index = -1);
    void RecordingFinished();
    void Reset();
    void SetPolygon(const QPolygonF &points);
    void SetRecordVisible(bool visible);
    void OnKeyPressed(int key);

private:
    void Refresh();

private slots:
    void OnRecordAddRequst(const QPointF &pos);
    void OnItemMove(const QPointF &pos);
    void onLineItemContextMenuRequest(QGraphicsSceneContextMenuEvent *event);
    void OnAddPointOnLine();

private:
    PolygonRecordItem *recordItem;
    QList<GraphicsDotItem *> pointList;
    QList<RecordLineItem *> lineList;
    QAction *addPointOnLineAction;

    QPointF prevDotItemPoint;
    QPointF nextDotItemPoint;
    QPointF startLinePoint;
    RecordLineItem *currentLine = nullptr;
    QPointF currentPos;
    QPolygonF polygon;
    const double SIZE_RECORDPOINT = 5;
};
