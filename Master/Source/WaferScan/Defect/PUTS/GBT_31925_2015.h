#pragma once

#include "PUTSRecognizer.h"

class GBT_31925_2015 : public PUTSRecognizer
{

private:
    DECLARE_RUNTIME(GBT_31925_2015);

public:
    GBT_31925_2015(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);
    ~GBT_31925_2015();
    virtual QString GetStandardName() const {return "GBT_31925_2015";}
    static void* CreateInstance(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options)
    {
        return new GBT_31925_2015(transform, imageList, options);
    }
};
