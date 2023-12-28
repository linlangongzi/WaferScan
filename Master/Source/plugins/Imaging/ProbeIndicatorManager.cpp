#include "ProbeIndicatorManager.h"
#include "CScanProbeIndicator.h"
#include <Core/Platform.h>
#include <Imaging/UTDocument.h>
#include <Imaging/UTLayer.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <Waveform/UTGraphicsScene.h>

ProbeIndicatorManager::ProbeIndicatorManager(QGraphicsItem *root, QObject *parent) :
    CScanGraphicsItemManager(parent),
    indicatorParent(new QGraphicsItemGroup(root))
{
    connect(UltrasonicManager::instance(), &UltrasonicManager::channelsChanged, this, &ProbeIndicatorManager::refreshChannels);
    refreshChannels();
}

void ProbeIndicatorManager::setVisible(bool visible)
{
    indicatorParent->setVisible(visible);
}

void ProbeIndicatorManager::ConnectToLayer(int layerID)
{
    CScanGraphicsItemManager::ConnectToLayer(layerID);
    UTLayer *layer = document()->GetLayerByID(layerID);
    if (!layer) {
        return;
    }

    connect(layer, &UTLayer::ImageOptionsChanged, this, &ProbeIndicatorManager::OnImageOptionsChanged);
    OnImageOptionsChanged(layer->GetImageOptions());
}

void ProbeIndicatorManager::refreshChannels()
{
    qDeleteAll(indicators);
    indicators.clear();

    for (UTChannel *channel: UltrasonicManager::channels()) {
        auto *indicator = new CScanProbeIndicator(indicatorParent);
        indicators.append(indicator);
        indicator->SetName(channel->name());
        connect(channel, &UTChannel::MonitorDataArrived, this, &ProbeIndicatorManager::OnMonitorDataReceived, Qt::UniqueConnection);
        OnMonitorDataReceived(channel->lastFrame());
    }
}

void ProbeIndicatorManager::OnMonitorDataReceived(UTDataFrame frame)
{
    int channelID = frame.Context.GetChannelID();
    CScanProbeIndicator *indicator = indicators.value(channelID);
    if (!indicator) {
        return;
    }
    QPointF point = imageOptions.MapToSource(frame, channelID);
    QPointF sourcePos = imageOptions.MapToSource(point);
    QPointF gridPos = imageOptions.MapFromGrid(sourcePos);
    indicator->setPos(gridPos);
}

void ProbeIndicatorManager::OnImageOptionsChanged(const ImageOptions &options)
{
    Axis *axisX = MotionManager::axis(options.axisX());
    Axis *axisY = MotionManager::axis(options.axisY());
    if (!axisX || !axisY) {
        return;
    }

    for (int index = 0; index < indicators.count(); ++index) {
        indicators[index]->setVisible(options.isChannelEnabled(index));
    }
    imageOptions = options;
}
