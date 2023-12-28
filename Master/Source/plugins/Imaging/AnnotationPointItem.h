#pragma once

#include <QGraphicsItem>

class AnnotationPointItem : public QGraphicsPathItem
{

public:
    AnnotationPointItem(QGraphicsItem *parent = nullptr);

    using PositionCallback = std::function<QPointF (QPointF, QPointF)>;
    void setPositionCallback(const PositionCallback &callback);

    qreal diameter() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    PositionCallback m_positionCallback;
};
