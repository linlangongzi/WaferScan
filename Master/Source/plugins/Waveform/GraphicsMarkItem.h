#pragma once

#include <QGraphicsItem>
#include <functional>

class GraphicsMarkItem : public QGraphicsPathItem
{
public:
    enum Style {
        Plus,
        Cross,
        Circle,
        Rectangle,
        RightArrowTriangle,
        UpArrowTriangle
    };

    typedef std::function<QPointF(QPointF, QPointF)> PositionCallback;

    GraphicsMarkItem(QGraphicsItem *parent = 0);
    void setShape(qreal size, Style style = Plus);
    void setPositionCallback(PositionCallback callback);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    PositionCallback positionCallback;
};
