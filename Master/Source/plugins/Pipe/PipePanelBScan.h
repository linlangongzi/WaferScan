#pragma once

#include <QLabel>
#include <QtMath>
#include <QFrame>
#include <QGraphicsLineItem>
#include "PanelStatusFrame.h"
#include <Imaging/CursorManager.h>
#include <Plot/PlotConfig.h>
#include <Ultrasonic/UTData.h>

class GraphicsLineSeries;
class QCheckBox;

namespace Ui {
class PipePanelBScan;
}

class PipePanelBScan : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(PlotView viewOptions READ viewOptions WRITE setViewOptions NOTIFY viewOptionsChanged)

public:
    explicit PipePanelBScan(QWidget *parent = 0);
    ~PipePanelBScan();
    PlotView viewOptions() const;
    qreal getPositionMax() const;

public slots:
    bool appendData(const UTDataSegment &segment);
    void setViewOptions(PlotView viewOptions);
    void resetData();
    QList<PlotAlarmItem> alarmsTriggered() const;
    void OnMouseMoved(const QPointF &scenePos);

signals:
    void viewOptionsChanged(PlotView viewOptions);

private slots:
    void on_optionsDlg_clicked();

private:
    void updateView();
    qreal Angle2Cos(const qreal &angle);

private:
    Ui::PipePanelBScan *ui;
    PlotView m_viewOptions;

    QWidgetList legendWidgets;
    QGraphicsItemGroup *root;
    QGraphicsRectItem *deadZoneLeft;
    QGraphicsRectItem *deadZoneRight;
    QList<QGraphicsLineItem *> lines;

    QList<GraphicsLineSeries *> curves;
    CursorManager *cursorManager = nullptr;            // 鼠标Pos线
    qreal positionMax = 0;
};
