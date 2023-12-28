#pragma once

#include <allrising/Range.h>
#include "GraphicsAxis.h"
#include <QGraphicsView>
#include <QPropertyAnimation>

class UTGraphicsScene;

class WAVEFORM_EXPORT UTGraphicsView : public QGraphicsView
{
    Q_OBJECT

    Q_PROPERTY(bool gridEnable READ isGridEnabled WRITE setGridEnable)
    Q_PROPERTY(QPointF center READ center WRITE setCenter)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
//    Q_PROPERTY(QRectF viewRect READ GetViewRect WRITE SetViewRect)
    friend class AbstractGraphicsView;

public:
    UTGraphicsView(QWidget *parent = 0);
    ~UTGraphicsView();

public:
    bool isGridEnabled() const {return gridEnable;}
    QPointF center();
    qreal scale() const;
    void setZoomEnable(bool enabled) {isZoomEnable = enabled;}
    bool isZoomEnabled() {return isZoomEnable;}
    void setCenterAnimated(const QPointF &newCenter);
    void setScaleAnimated(double newScale);
    QTransform sceneTransform() const;
    QTransform userTransform() const;
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;

public slots:
    void setAxis(GraphicsAxis *x, GraphicsAxis *y);
    void setGridEnable(bool enable);
    void setCenter(const QPointF &pos);
    void setScale(qreal value);
    void updateViewport();
    void updateBackground();
    void zoomIn();
    void zoomOut();

signals:
    void mouseMoved(QPointF scenePos);
    void mousePressed();
    void mouseReleased();
    void contextMenuRequest(QPoint pos);
    void mouseDoubleClicked(QPointF pos);
    void viewportUpdated();

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual bool event(QEvent *event) override;

private:
    void onMouseScrolled(int direction, const QPointF &center = QPointF());

private:
    bool gridEnable = false;
    bool isMousePressed = false;
    bool isZoomEnable = false;

    QPoint mousePressedPos;
    QPointF mousePressedCenter;

    QPropertyAnimation centerAnimation;
    QPropertyAnimation scaleAnimation;

    QPointer<GraphicsAxis> axisX;
    QPointer<GraphicsAxis> axisY;
};
