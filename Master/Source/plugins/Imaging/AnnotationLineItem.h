#pragma once

#include <QGraphicsItem>
#include "AnnotationPointItem.h"

class AnnotationLineItem : public QGraphicsItemGroup
{
    enum State {
        ZeroPoint = 0,
        OnePoint,
        TwoPoint
    };

public:
    AnnotationLineItem(QGraphicsItem *parent = nullptr);
    ~AnnotationLineItem() override;

    void setLine(const QPointF &start, const QPointF &end);
    void setStart(const QPointF &newStart);
    void setEnd(const QPointF &newEnd);
    void updateLine(const QPointF &start, const QPointF &end);
    void updatePoint(const QPointF &start, const QPointF &end);

    QRectF boundingRect() const override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QGraphicsLineItem *m_line = nullptr;
    AnnotationPointItem *m_startPoint = nullptr;
    AnnotationPointItem *m_endPoint = nullptr;
    State m_state = ZeroPoint;
    bool m_isSelected = false;
};
