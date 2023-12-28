#include "UTDefectProcessorOptions.h"
#include <allrising/Util.h>

QJsonValue UTDefectProcessorOptions::ToJson() const
{
    QJsonObject json;

    json["Threshold"] = threshold;
    json["ThresholdMode"] = EnumToKey(thresholdMode);
    json["HasDepth"] = hasDepth;
    json["HasBottomDepth"] = hasBottomDepth;
    json["DepthIndex"] = depthIndex;
    json["BottomDepthIndex"] = bottomDepthIndex;
    json["MaterialVelocity"] = materialVelocity;
    json["Dilation"] = dilation;
    json["MinOuterArea"] = minOuterArea;
    json["ROI"] = PolygonFToJson(roi);

    return json;
}

UTDefectProcessorOptions UTDefectProcessorOptions::FromJson(const QJsonValue &value)
{
    const QJsonObject json = value.toObject();
    UTDefectProcessorOptions options;

    options.threshold = json["Threshold"].toDouble();
    options.thresholdMode = KeyToEnum(json["ThresholdMode"].toString(), HIGH);
    options.hasDepth = json["HasDepth"].toBool();
    options.hasBottomDepth = json["HasBottomDepth"].toBool();
    options.depthIndex = json["DepthIndex"].toInt();
    options.bottomDepthIndex = json["BottomDepthIndex"].toInt();
    options.materialVelocity = json["MaterialVelocity"].toDouble();
    options.dilation = json["Dilation"].toInt();
    options.minOuterArea = json["MinOuterArea"].toDouble();
    options.roi = JsonToPolygonF(json["ROI"]).toPolygon();

    return options;
}
