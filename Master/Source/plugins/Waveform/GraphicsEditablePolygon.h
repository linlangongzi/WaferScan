#pragma once

#include <QGraphicsObject>
#include "GraphicsMarkItem.h"
#include <QPen>

class GraphicsEditablePolygon : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

    Q_PROPERTY(QPolygonF polygon READ polygon WRITE setPolygon NOTIFY polygonChanged)
    Q_PROPERTY(bool editable READ isEditable WRITE setEditable NOTIFY editableChanged)
    Q_PROPERTY(QPen pen READ pen WRITE setPen)

public:
    typedef std::function<QPointF(QPointF, QPolygonF, int)> MoveValidator;

    GraphicsEditablePolygon(QGraphicsItem *parent = nullptr);

    QPolygonF polygon() const;
    bool isEditable() const;
    QPen pen() const;

public slots:
    void setPolygon(const QPolygonF &polygon);
    void setEditable(bool editable);
    void setPen(const QPen &pen);
    void setMoveValidator(MoveValidator validator);
    void startRecord();
    void reset();

signals:
    void polygonChanged(QPolygonF polygon);
    void editableChanged(bool editable);

protected:
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void createPoint();
    void createLine();

    QGraphicsPathItem *polygonItem;
    QList<GraphicsMarkItem *> points;
    QList<QGraphicsLineItem *> lines;
    QPolygonF m_polygon;
    bool m_editable;
    MoveValidator m_validator;

};
