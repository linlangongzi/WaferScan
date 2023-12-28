#pragma once

#include "DefectItem.h"
#include <QGraphicsItemGroup>
#include <QGraphicsOpacityEffect>
#include <QPointer>
#include <QPropertyAnimation>

class GraphicsPolygonItem;
class GraphicsFlashingEffect : public QGraphicsOpacityEffect
{
    Q_OBJECT
    Q_PROPERTY(qreal progress READ GetProgress WRITE SetProgress)
    qreal m_progress;

public:
    qreal GetProgress() const;

public slots:
    void SetProgress(qreal progress);
};

class DefectGraphicsItem : public QGraphicsItemGroup
{
public:
    DefectGraphicsItem(DefectItem *item, QGraphicsItem *parent = 0);
    void SetDefectItem(DefectItem *item);
    DefectItem *GetDefectItem() {return defectItem;}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool contains(const QPointF &point) const override;

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    DefectItem *defectItem = nullptr;
    GraphicsPolygonItem *minRect = nullptr;
    QGraphicsPixmapItem *pixmap = nullptr;
    QRectF rect;

    static QPropertyAnimation animation;
};
