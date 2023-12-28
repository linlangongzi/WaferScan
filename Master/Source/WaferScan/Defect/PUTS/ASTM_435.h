#pragma once

#include "PUTSRecognizer.h"

/*!
 * \class ASTM_435
 * \brief ASTM_435 2001 厚钢板超声波检验方法
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
 *  MaxAllowedLength    允许的最大长度(mm)，默认值为75，即合格标准
 *  PlateThickness      钢板厚度
 *  EdgeZoneWidth
 */

class ASTM_435 : public PUTSRecognizer
{
private:
    DECLARE_RUNTIME(ASTM_435);
public:
    ASTM_435(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);
    virtual QString GetStandardName() const {return "ASTM_435";}
    static void* CreateInstance(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options)
    {
        return new ASTM_435(transform, imageList, options);
    }
private:
    virtual void SetPartProperty();
    virtual void SetDefectRank(Defect &defect);
    virtual void CalcPartResult();
    virtual void DefectCombine(const QRectF &centerZone, QVector<QPolygonF> boundaryPolygons);
};

IMPLEMENT_RUNTIME(ASTM_435)
