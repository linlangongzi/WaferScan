#include "GBT_2970.h"
IMPLEMENT_RUNTIME(GBT_2970)

GBT_2970::GBT_2970(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options) :
    PUTSRecognizer(transform, imageList, options)
{
    SetPartProperty();
}

void GBT_2970::SetPartProperty()
{
    mapPartProperty.insert("AvgRatioAll", false);
    mapPartProperty.insert("MaxRatioAll", false);
    mapPartProperty.insert("MaxIndiLengthBody", false);
    mapPartProperty.insert("MaxIndiLengthEdge", false);
    mapPartProperty.insert("MaxIndiLengthAll", false);
    mapPartProperty.insert("AreaSum", false);
    mapPartProperty.insert("Result", false);
}
