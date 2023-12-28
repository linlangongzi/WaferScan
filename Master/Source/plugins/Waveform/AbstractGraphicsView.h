#pragma once

#include "WaveformGlobal.h"
#include <allrising/Range.h>
#include "GraphicsAxis.h"
#include "UTGraphicsView.h"

namespace Ui {
class AbstractGraphicsView;
}

class UTGraphicsScene;

class WAVEFORM_EXPORT AbstractGraphicsView : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractGraphicsView(QWidget *parent = nullptr);
    ~AbstractGraphicsView() override;

public:
    void setRulerPos(Qt::Edges edges);
    QList<GraphicsAxis *> axes(Qt::Edges edge);
    GraphicsAxis *axis(Qt::Edge edg);
    void setReversed(Qt::Orientations arg, bool isReverse = false);
    bool isReversed(Qt::Orientation arg);
    QPointF mapScenePosFromGlobal(const QPoint &pos);
    QPoint mapToGlobalFromScene(const QPointF &pos);
    QRectF mapFromScene(const QRectF &rect);
    QPointF mapFromScene(const QPointF &pos);
    QPoint mapFromScene(const QPoint &pos);
    QGraphicsScene *graphicsScene();

public:
    UTGraphicsView *view() const;
    UTGraphicsScene *scene() const;
    QGraphicsItemGroup *rootItem() const;
    void setZoomEnable(bool enable);
    void setGridEnable(bool enable);
    void setBackgroundColor(const QColor &color);
    void setRulerUnitName(Qt::Edges edges, const QString &arg);
    void setOrigin(const QPointF &origin);
    void setRulerScale(Qt::Edges edges, qreal ratio);

public:
    double origin(Qt::Orientation orientation);
    QRectF sceneRect();
    Range selection(Qt::Orientation org);
    void ensureVisible(const QPointF &pos, int xmargin = 1, int ymargin = 1);

public slots:
    void onZoomIn();
    void onZoomOut();
    void setRulerSelection(Qt::Orientations org, const Range &range);
    void setRange(Qt::Orientations orientation, const Range &range);
    void setCenter(const QPointF &center);
    void fitInViewAnimated(const QRectF &rect, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);

signals:
    void mouseMoved(QPointF scenePos);
    void contextMenuRequest(QPoint pos);
    void mouseDoubleClicked(QPointF pos);
    void selectionChanged(Qt::Orientation ori, const Range &range);
    void rangeChanged(Qt::Orientation ori, const Range &range);
    void rubberBandChanged(QRect rubberBandRect, QPointF fromScenePoint, QPointF toScenePoint);

protected:
    UTGraphicsScene *m_scene = nullptr;
    QList<QAction *> actions;

private slots:
    void onAxisUpdated();
    void onContextMenuTriggered(const QList<QAction *> &actionList);
    void onAxisRangeChanged();
    void refreshRuler();

private:
    void initView();
    QList<QWidget *> rulerWidgets(Qt::Edges edges) const;
    QWidget *rulerWidget(Qt::Edge edge) const;
    void paintEvent(QPaintEvent *e) override;

protected:
    Ui::AbstractGraphicsView *ui;
    QGraphicsItemGroup *m_sceneRoot = Q_NULLPTR;
    QList<QWidget *> m_rulerWidgets;

    QPointer<GraphicsAxis> m_horAxis;
    QPointer<GraphicsAxis> m_verAxis;
};
