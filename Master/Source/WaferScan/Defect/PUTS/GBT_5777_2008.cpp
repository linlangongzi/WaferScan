#include "GBT_5777_2008.h"

GBT_5777_2008::GBT_5777_2008(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options) :
    PUTSRecognizer(transform, imageList, options)
{
    SetPartProperty();
}

GBT_5777_2008::~GBT_5777_2008()
{

}
