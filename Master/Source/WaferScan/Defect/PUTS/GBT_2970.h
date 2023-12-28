#pragma once

#include "PUTSRecognizer.h"

/*!
 * \class GBT_2970
 * \brief GB/T 2970-2004 厚钢板超声波检验方法
 *
 * 选项
 *  DefectAmpIndex      伤波幅度图像的索引，默认值为0
 *  DefectDepthIndex    伤波深度图像的索引，默认值为1
 *  BottomAmpIndex      底波幅度图像的索引，默认值为2
 *  BottomDepthIndex    底波深度图像的索引，默认值为3
 *  DefectAmpThreshold  7.1.1 中，伤波判伤阈值(%)，高于此阈值判为有伤，默认值为50
 *  BottomAmpMax        7.1.2 中，判定底波不满幅值的阈值，默认值为100
 *  BottomDefectRatio   7.1.2 中，伤波与底波之比的阈值(比例)，底波不满且比例高于此阈值判为有伤，默认值为0.5
 *  BottomAmpThreshold  7.1.3 中，底波衰减判伤阈值(%)，低于此阈值判为有伤，默认值为50
 *
 *  MaxAllowedLength    8.1 钢板分级中，当前级别允许的最大指示长度(mm)，默认值为80，即I级钢板的标准
 *  MaxAllowedArea      8.1 钢板分级中，当前级别允许的最大缺陷面积(mm^2)，默认值为2500，即I级钢板的标准。
 *                      请注意， 8.1 中注明的最大缺陷面积单位为 cm^2 。
 *  MaxAllowedPercent   8.1 钢板分级中，当前级别允许的在任意1000mm x 1000mm区域内的缺陷占积率，默认值为0.03，即I级钢板的标准(3%)
 *
 *  MinDefectDistance   7.4.2 单个缺陷合并时，缺陷之间允许的最小边缘距离，默认值为100(mm)
 *
 *  EdgeZoneWidth       8.2 钢板边缘的宽度，默认值为50
 *  EdgeMaxLength       8.2 钢板边缘区域允许的最大指示长度，默认值为50
 */

class GBT_2970 : public PUTSRecognizer
{
private:
    DECLARE_RUNTIME(GBT_2970);
public:
    GBT_2970(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);
    virtual QString GetStandardName() const {return "GBT_2970";}
    static void* CreateInstance(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options)
    {
        return new GBT_2970(transform, imageList, options);
    }
private:
    virtual void SetPartProperty();
};
