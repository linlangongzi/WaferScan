#pragma once

#include "PUTSRecognizer.h"

class BS_EN_ISO_10893 : public PUTSRecognizer
{
private:
    DECLARE_RUNTIME(BS_EN_ISO_10893);
public:
    BS_EN_ISO_10893(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);
    ~BS_EN_ISO_10893();
    virtual QString GetStandardName() const {return "BS_EN_ISO_10893";}
    static void* CreateInstance(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options)
    {
        return new BS_EN_ISO_10893(transform, imageList, options);
    }
private:
    virtual void SetPartProperty();
    virtual void SetDefectRank(Defect &defect);
    virtual void CalcPartResult();

};
