#pragma once

#include <QGraphicsItem>

#include <Waveform/GraphicsInvertedLine.h>

class QGraphicsScene;

class AnnotationManager : public QObject
{
    Q_OBJECT

public:

    enum Mode {
        None,
        Line,
        Circle,
        CustomCircle,
        Rect
    };
    enum State {
        ZeroPoint = 0,
        OnePoint,
        TwoPoint
    };

    AnnotationManager(QGraphicsItem *item, QObject  *parent = nullptr);
    ~AnnotationManager();

    bool mouseMoveHandler(const QPointF &pos);
    bool doubleClickHandler(const QPointF &pos);
    void setMode(AnnotationManager::Mode customMode);
    void setDiameter(qreal custom);

private:
    QRectF rectAt(const QPointF &pos);
    void setCirclePos(const QPointF &pos);
    void setRectPos(const QPointF &pos);
    void toggleShapeState(const QPointF &pos);
    void updateShapes(const QPointF &pos);
    void tip(const QString &text, const QPointF &pos) const;

private:
    GraphicsInvertedShape<QGraphicsLineItem> *line = nullptr;
    GraphicsInvertedShape<QGraphicsLineItem> *leftEndLine = nullptr;
    GraphicsInvertedShape<QGraphicsLineItem> *rightEndLine = nullptr;
    GraphicsInvertedShape<QGraphicsEllipseItem> *circle = nullptr;
    GraphicsInvertedShape<QGraphicsRectItem> *rectangle = nullptr;
    QGraphicsItem *parentItem = nullptr;
    qreal diameter = 0;
    State state = ZeroPoint;
    Mode mode = None;
};
