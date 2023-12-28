#include "EN_10160.h"

IMPLEMENT_RUNTIME(EN_10160)
EN_10160::EN_10160(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options) :
    PUTSRecognizer(transform, imageList, options)
{
    combineMax =false;
    SetPartProperty();
}

void EN_10160::SetPartProperty()
{
    mapPartProperty.insert("MaxIndiLengthBody", false);
    mapPartProperty.insert("MaxIndiLengthEdge", false);
    mapPartProperty.insert("AvgRatioAll", false);
    mapPartProperty.insert("MaxAreaBody", false);
    mapPartProperty.insert("MaxAreaEdge", false);
    mapPartProperty.insert("PerMeterMaxNumEdge", false);
    mapPartProperty.insert("PerSquareMeterMaxNumBody", false);
    mapPartProperty.insert("AreaSum", false);
    mapPartProperty.insert("Result", false);
    mapPartProperty.insert("ResultBody", false);
    mapPartProperty.insert("ResultEdge", false);

}

void EN_10160::SetDefectRank(Defect &defect)
{
    // 默认E4等级
    double maxAreaEdge = options.value("MaxAllowedAreaEdge").toDouble(50);
    double maxLengthEdge = options.value("MaxAllowedLengthEdge").toDouble(20);
    // 默认S3等级
    double maxAreaBody = options.value("MaxAllowedAreaBody").toDouble(50);

    if (defect.type == DEFECT_NORMAL)
    {
        defect.rank = defect.area / maxAreaBody;
    }
    else if (defect.type == DEFECT_EDGE)
    {
        defect.rank = qMax(defect.length / maxLengthEdge, defect.area / maxAreaEdge);
    }
    else if (defect.type == DEFECT_COMBINED_EDGE)
    {
        defect.rank = qMax(defect.length / maxLengthEdge, defect.area / maxAreaEdge);
    }
    else if (defect.type == DEFECT_COMBINED_BODY)
    {
        defect.rank = defect.area / maxAreaBody;
    }
    else
    {
        ;//
    }

}

void EN_10160::CalcPartResultBody()
{
    float maxRank = 0.0f;

    foreach ( Defect defect, defects )
    {
        if ( (defect.type == DEFECT_NORMAL) || (defect.type == DEFECT_COMBINED_BODY)
        || (defect.type == DEFECT_DENSITY_NUM) )
        {
            maxRank = qMax(maxRank, defect.rank);
        }
    }

    InsertPartMap( "ResultBody", PairPartInfo( "主体判伤结果", static_cast<double>(maxRank) ) );

}

void EN_10160::CalcPartResultEdge()
{
    float maxRank = 0.0f;

    foreach ( Defect defect, defects )
    {
        if ( (defect.type == DEFECT_EDGE) || (defect.type == DEFECT_COMBINED_EDGE)
        || ( defect.type == DEFECT_LENGTH_NUM) )
        {
            maxRank = qMax(maxRank, defect.rank);
        }
    }

    InsertPartMap( "ResultEdge", PairPartInfo( "边沿判伤结果", static_cast<double>(maxRank) ) );

}
