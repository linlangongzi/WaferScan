#pragma once

#include <QObject>
#include <QPolygonF>

class UTDefectProcessorOptions
{
    Q_GADGET
public:
    enum ThresholdMode {
        LOW,
        HIGH
    };
    Q_ENUM(ThresholdMode)

    QJsonValue ToJson() const;
    static UTDefectProcessorOptions FromJson(const QJsonValue &value);

    qreal threshold = 50;
    ThresholdMode thresholdMode = HIGH;
    bool hasDepth = false;
    bool hasBottomDepth = false;
    int depthIndex = 0;
    int bottomDepthIndex = 0;
    qreal materialVelocity = 1500;
    int dilation = 0;
    qreal minOuterArea = 0;
    QPolygon roi;
};
