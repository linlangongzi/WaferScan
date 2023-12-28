#pragma once

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneContextMenuEvent>

class GraphicsDotItem : public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(QPointF point READ GetPoint WRITE SetPoint)
    Q_PROPERTY(double size READ GetSize WRITE SetSize)
public:
    enum Shape {
        CIRCLE,
        SQUARE
    };

public:
    explicit GraphicsDotItem(QGraphicsItem *parent = 0);
    QPointF GetPoint() const;
    double GetSize() const;

    void SetPoint(QPointF arg);
    void SetPoint(float x, float y);
    void SetSize(double arg);
    void SetInColor(QColor color);
    void SetOutColor(QColor color);

    virtual QRectF boundingRect() const;

    void SetDragEnabled(bool enable);
    void SetShape(Shape shape = CIRCLE);
    void SetBackLineShow(bool enabled);
    void SetIfFillColor(bool value);


signals:
    void DotMoved(const QPointF &pos);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QPointF point;
    double size;
    QTransform worldTrans;
    bool dragable;
    Shape shape;
    QColor inColor;
    QColor outColor;
    bool backLineShow;
    bool fillColor;
};
