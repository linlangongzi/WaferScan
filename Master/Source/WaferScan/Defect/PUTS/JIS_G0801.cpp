#include "JIS_G0801.h"
IMPLEMENT_RUNTIME(JIS_G0801)

JIS_G0801::JIS_G0801(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options) :
    PUTSRecognizer(transform, imageList, options)
{
    SetPartProperty();
}

JIS_G0801::~JIS_G0801()
{

}

void JIS_G0801::SetPartProperty()
{
    mapPartProperty.insert("MaxIndiLengthBody", false);
    mapPartProperty.insert("MaxIndiLengthEdge", false);
    mapPartProperty.insert("MaxRatioBody", false);
    mapPartProperty.insert("MaxRatioEdge", false);
    mapPartProperty.insert("PerMeterMaxNumEdge", false);
    mapPartProperty.insert("PerSquareMeterMaxNumBody", false);
    mapPartProperty.insert("AreaSum", false);
    mapPartProperty.insert("Result", false);
}

void JIS_G0801::SetDefectRank(PUTSRecognizer::Defect &defect)
{
    // 默认合格等级
    double bodymaxLength = options.value("MinAllowedLengthBody").toDouble(100);
    double edgemaxLength = options.value("MaxAllowedLengthEdge").toDouble(50);

    if (defect.type == DEFECT_NORMAL)
    {
        defect.rank = defect.length / bodymaxLength;
    }
    else if (defect.type == DEFECT_EDGE)
    {
        defect.rank = defect.length / edgemaxLength;
    }
    else
    {
        ;//
    }

}

void JIS_G0801::CalcPartResult()
{
    float maxRank = 0.0f;

    foreach( Defect defect, defects )
    {
        if((defect.type == DEFECT_NORMAL) || (defect.type == DEFECT_EDGE) || (defect.type == DEFECT_LENGTH_NUM) || (defect.type == DEFECT_DENSITY_NUM) ||
                (defect.type == DEFECT_DENSITY_BODY) || (defect.type == DEFECT_DENSITY_EDGE))
        {
            maxRank = qMax(maxRank, defect.rank);
        }
    }

    InsertPartMap( "Result", PairPartInfo( "判伤结果", static_cast<double>(maxRank)) );

    return;
}

