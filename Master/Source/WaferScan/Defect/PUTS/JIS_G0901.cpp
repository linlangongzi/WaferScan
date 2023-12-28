#include "JIS_G0901.h"

IMPLEMENT_RUNTIME(JIS_G0901)
JIS_G0901::JIS_G0901(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options) :
    PUTSRecognizer(transform, imageList, options)
{
    SetPartProperty();
}

JIS_G0901::~JIS_G0901()
{

}

void JIS_G0901::SetPartProperty()
{
    mapPartProperty.insert("MaxRatioAll", false);
    mapPartProperty.insert("AvgRatioAll", false);
    mapPartProperty.insert("AreaSum", false);
    mapPartProperty.insert("Result", false);
}

void JIS_G0901::SetDefectRank(PUTSRecognizer::Defect &defect)
{
    Q_UNUSED(defect);
}

void JIS_G0901::CalcPartResult()
{
    float maxRank = 0.0f;
    double avgAllowedPercent = options.value("AvgAllowedPercent").toDouble(0.07);
    foreach( Defect defect, defects )
    {
        if(defect.type == DEFECT_DENSITY)
        {
            maxRank = qMax(maxRank, defect.rank);
        }
    }
    maxRank = qMax(static_cast<float>(GetPartValue("AvgRatioAll") / avgAllowedPercent), maxRank);

    InsertPartMap( "Result", PairPartInfo( "判伤结果", static_cast<double>(maxRank)) );

}

