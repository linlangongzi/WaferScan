#pragma once

#include "PUTSRecognizer.h"

class NBT_47013_2015 : public PUTSRecognizer
{
private:
    DECLARE_RUNTIME(NBT_47013_2015);

public:
    NBT_47013_2015(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);
    ~NBT_47013_2015();
    virtual QString GetStandardName() const {return "NBT_47013_2015";}
    static void* CreateInstance(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options)
    {
        return new NBT_47013_2015(transform, imageList, options);
    }

private:
    virtual void SetPartProperty();
	virtual void SetDefectRank(Defect &defect);
};
