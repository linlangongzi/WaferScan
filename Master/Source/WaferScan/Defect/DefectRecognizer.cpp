#include "DefectRecognizer.h"

DefectRecognizer::DefectRecognizer(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options) :
    trans(transform),
    images(imageList),
    options(options)
{}
/*
DefectRecognizer *DefectRecognizer::Create(const QString &name, const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options)
{
   if (name == "GBT_2970")
   {
       return new GBT_2970(transform, imageList, options);
   }
    return NULL;
}*/
