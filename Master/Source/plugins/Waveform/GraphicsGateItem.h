#pragma once

#include <QGraphicsObject>
#include <Ultrasonic/UTSetup.h>
#include "GraphicsMarkItem.h"
#include "Ultrasonic/UTData.h"
class GraphicsGateItem : public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(bool horizontalLock READ horizontalLock WRITE setHorizontalLock)
    Q_PROPERTY(bool verticalLock READ verticalLock WRITE setVerticalLock)
    Q_PROPERTY(bool lengthLock READ lengthLock WRITE setLengthLock)

public:
    enum DragType {
        DragNone,
        DragMainLine,
        DragLeft,
        DragRight,
    };
    Q_ENUM(DragType)

    GraphicsGateItem(QGraphicsItem *parent = nullptr);

    qreal syncPos() const;
    bool horizontalLock() const;
    bool verticalLock() const;
    bool lengthLock() const;

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

public slots:
    void setGate(const UTSetupGate &newGate);
    void setTimebase(const UTSetupTimebase &newTimebase);
    void setGeometry(const QLineF &line);
    void setHorizontalLock(bool locked);
    void setVerticalLock(bool locked);
    void setLengthLock(bool locked);
    void onUnlock();
    void setMarkPos(qreal time, qreal amp);
    void setMarkVisible(bool visible);

signals:
    void lineDragged(QLineF line);
    void gateItemMenuRequested();

protected:
    void updateCursor();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
    QGraphicsLineItem *mainLine = nullptr;
    QGraphicsLineItem *leftEnding = nullptr;
    QGraphicsLineItem *rightEnding = nullptr;
    GraphicsMarkItem *mark = nullptr;

    UTSetupGate m_gate;
    UTSetupTimebase timebase;
    bool m_horizontalLock = false;
    bool m_verticalLock = false;
    bool m_lengthLock = false;

    DragType dragType = DragNone;
    QLineF dragStart;
};
