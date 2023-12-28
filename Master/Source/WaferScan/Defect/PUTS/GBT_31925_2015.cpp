#include "GBT_31925_2015.h"

GBT_31925_2015::GBT_31925_2015(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options) :
    PUTSRecognizer(transform, imageList, options)
{
    SetPartProperty();
}

GBT_31925_2015::~GBT_31925_2015()
{

}
