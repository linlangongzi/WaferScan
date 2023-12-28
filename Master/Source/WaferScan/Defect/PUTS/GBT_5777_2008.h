#pragma once

#include "PUTSRecognizer.h"

class GBT_5777_2008 : public PUTSRecognizer
{

private:
    DECLARE_RUNTIME(GBT_5777_2008);

public:
    GBT_5777_2008(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);
    ~GBT_5777_2008();
    virtual QString GetStandardName() const {return "GBT_5777_2008";}
    static void* CreateInstance(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options)
    {
        return new GBT_5777_2008(transform, imageList, options);
    }
};
