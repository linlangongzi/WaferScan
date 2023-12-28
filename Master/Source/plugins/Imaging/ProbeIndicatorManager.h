#pragma once

#include "CScanGraphicsItemManager.h"
#include <Imaging/ImageOptions.h>
#include <QGraphicsItem>
#include <Ultrasonic/UTData.h>

class CScanProbeIndicator;
class QGraphicsItem;

class ProbeIndicatorManager : public CScanGraphicsItemManager
{
    Q_OBJECT

public:
    ProbeIndicatorManager(QGraphicsItem *root, QObject *parent = nullptr);

public:
    void setVisible(bool visible);

protected slots:
    virtual void ConnectToLayer(int layerID);

private slots:
    void refreshChannels();
    void OnMonitorDataReceived(UTDataFrame frame);
    void OnImageOptionsChanged(const ImageOptions &options);

private:
    QGraphicsItem *indicatorParent = nullptr;
    QList<CScanProbeIndicator *> indicators;
    ImageOptions imageOptions;
};
