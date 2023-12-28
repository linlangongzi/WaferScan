#pragma once

#include "Ars1000/PipeDataDef.h"
#include "DataManagerBase.h"
#include "PanelStatusFrame.h"
#include "PipePanelBScan.h"
#include "PipeParameter.h"
#include <QFrame>
#include <QMap>
#include <QVector>

namespace Ui {
class PipePanelBScanMain;
}

class PipePanelBScanMain : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(QList<PlotView> viewOptions READ viewOptions WRITE setViewOptions NOTIFY viewOptionsChanged)

public:
    using OrderInfo = PipeDetect::OrderInfo;

public:
    explicit PipePanelBScanMain(QWidget *parent = nullptr);
    ~PipePanelBScanMain();
    QList<PlotView> viewOptions() const;
    void recalculateAlarms();
    qreal getPositionMax() const;

signals:
    void UpdateRulerLabel();
    void viewOptionsChanged(QList<PlotView> viewOptions);
    void alarmsUpdated(QList<PlotAlarmItem> alarms);

public slots:
    void SetOrderInfo(const OrderInfo &info);
    void OnSegmentUpdate(const UTDataSegment &segment);
    void setViewOptions(const QList<PlotView> &views);
    void addViewOption(const PlotView &option);
    void removeView(int index);
    void resetData();

private slots:
    void createView(const PlotView &option);
    void on_addView_clicked();
    void on_removeView_clicked();

private:
    Ui::PipePanelBScanMain *ui;
    QList<PlotView> m_viewOptions;
    QList<PipePanelBScan *> m_views;
};
