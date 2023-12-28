#pragma once

#include "PUTSRecognizer.h"

/*!
 * \class JIS_G0801
 * \brief JIS_G0801 1993 压力容器用钢板超声波检验方法
 *
 * 选项
 *  DefectAmpIndex      伤波幅度图像的索引，默认值为0
 *  DefectDepthIndex    伤波深度图像的索引，默认值为1
 *  BottomAmpIndex      底波幅度图像的索引，默认值为2
 *  BottomDepthIndex    底波深度图像的索引，默认值为3
 *  DefectAmpThreshold  GBT_2970 7.1.1 中，伤波判伤阈值(%)，高于此阈值判为有伤，默认值为50
 *  BottomAmpMax        GBT_2970 7.1.2 中，判定底波不满幅值的阈值，默认值为100
 *  BottomDefectRatio   GBT_2970 7.1.2 中，伤波与底波之比的阈值(比例)，底波不满且比例高于此阈值判为有伤，默认值为0.5
 *  BottomAmpThreshold  GBT_2970 7.1.3 中，底波衰减判伤阈值(%)，低于此阈值判为有伤，默认值为50
 *
 *  MaxAllowedLengthEdge 当前级别允许的边沿最大指示长度，默认值为50
 *  MinAllowedLengthBody 当前级别允许的本体最大指示长度，默认值为100
 *  MaxAllowedPercentBody
 */

class JIS_G0801 : public PUTSRecognizer
{
private:
    DECLARE_RUNTIME(JIS_G0801);
public:
    JIS_G0801(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);
    ~JIS_G0801();
    virtual QString GetStandardName() const {return "JIS_G0801";}
    static void* CreateInstance(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options)
    {
        return new JIS_G0801(transform, imageList, options);
    }
private:
    virtual void SetPartProperty();
    virtual void SetDefectRank(Defect &defect);
    virtual void CalcPartResult();
};
