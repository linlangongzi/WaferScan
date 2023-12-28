#pragma once

#include "PUTSRecognizer.h"

/*!
 * \class EN_10160
 * \brief EN 10160-1999 厚钢板超声波检验方法
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
 *  MaxAllowedLengthEdge 11 Table5钢板边沿分级中，当前级别允许的最大长度(mm改成使用指示长度)，默认值为20，即E4级钢板的标准
 *  MinAllowedLengthEdge 11 Table5钢板边沿分级中, 最小指示长度（每米最大缺陷数量中使用），默认值为10，即E4级钢板的标准
 *  MaxAllowedAreaEdge   11 Table5钢板边沿分级中，当前级别允许的最大指示指示面积(mm^2)，默认值为50，即E4级钢板的标准
 *  MaxPerMeterNumEdge   11 Table5钢板边沿分级中， 每米充许的最大缺陷数量，默认值为2，即E4级钢板的标准
 *  MaxAllowedAreaBody   11 Table3 钢板主体分级中，当前级别允许的最大缺陷指示面积(mm^2)，默认值为50，即S3级钢板的标准。
 *  MaxDensityNumBody   11 Table3 钢板主体分级中，当前级别允许的在任意1000mm x 1000mm区域内的最大缺陷数量，
 *                      默认值为10，即S3级钢板的标准
 *
 *  MinDefectDistance   GBT_2970 7.4.2 单个缺陷合并时，缺陷之间允许的最小边缘距离，默认值为100(mm)
 *                      9.2.1.1 当间距小于两个缺陷指示长度中的最小值时合并
 *
 *  EdgeZoneWidth       8.3 Table2 钢板边缘的宽度，分三种宽度，默认值为50(板厚6-50)，默认值为75(板厚50-100)，默认值为100(板厚100-200)
 *                      默认值为50(板厚6-50)，
 */

class EN_10160 : public PUTSRecognizer
{
private:
    DECLARE_RUNTIME(EN_10160);
public:
    EN_10160(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);
    virtual QString GetStandardName() const {return "EN_10160";}
    static void* CreateInstance(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options)
    {
        return new EN_10160(transform, imageList, options);
    }
private:
    virtual void SetPartProperty();
    virtual void SetDefectRank(Defect &defect);
    virtual void CalcPartResultBody();
    virtual void CalcPartResultEdge();
};
