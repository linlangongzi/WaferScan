#pragma once

#include <QGraphicsItem>



class CharacteristicItem : public QGraphicsItem
{
public:
    CharacteristicItem(QGraphicsItem *parent = 0);
    void SetColor(QColor value);
    void SetSize(qreal value);
    virtual QRectF boundingRect() const;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPointF pos;
    qreal size;
    QColor color;
    QTransform worldTrans;
};
