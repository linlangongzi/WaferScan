#pragma once

#include "PUTSRecognizer.h"

class JIS_G0901 : public PUTSRecognizer
{
private:
    DECLARE_RUNTIME(JIS_G0901);
public:
    JIS_G0901(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);
    ~JIS_G0901();
    virtual QString GetStandardName() const {return "JIS_G0901";}
    static void* CreateInstance(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options)
    {
        return new JIS_G0901(transform, imageList, options);
    }
private:
    virtual void SetPartProperty();
    virtual void SetDefectRank(Defect &defect);
    virtual void CalcPartResult();
};
