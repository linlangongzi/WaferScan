#include "BS_EN_ISO_10893.h"
IMPLEMENT_RUNTIME(BS_EN_ISO_10893)

BS_EN_ISO_10893::BS_EN_ISO_10893(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options) :
    PUTSRecognizer(transform, imageList, options)
{
    SetPartProperty();
}

BS_EN_ISO_10893::~BS_EN_ISO_10893()
{

}

void BS_EN_ISO_10893::SetPartProperty()
{
    mapPartProperty.insert("MaxIndiLengthBody", false);
    mapPartProperty.insert("MaxIndiLengthEdge", false);
    mapPartProperty.insert("MaxRatioBody", false);          //本体最大缺陷率
    mapPartProperty.insert("MaxRatioEdge", false);
    mapPartProperty.insert("MaxAreaBody", false);
    mapPartProperty.insert("MaxAreaEdge", false);
    mapPartProperty.insert("PerMeterMaxRatioBody", false);  //本体长度最大每米占机率
    mapPartProperty.insert("PerMeterAvgRatioBody", false);
    mapPartProperty.insert("PerSquareMeterMaxNumBody", false); //本体单位面积最大缺陷数
    mapPartProperty.insert("PerMeterMaxNumEdge", false);    //边缘长度最大每米缺陷数
    mapPartProperty.insert("AreaSum", false);
    mapPartProperty.insert("Result", false);
}

void BS_EN_ISO_10893::SetDefectRank(PUTSRecognizer::Defect &defect)
{
    // 默认E1等级
    double maxAreaEdge = options.value("MaxAllowedAreaEdge").toDouble(250);
    double maxLengthEdge = options.value("MaxAllowedLengthEdge").toDouble(20);
    // 默认B1等级
    double maxAreaBody = options.value("MaxAllowedAreaBody").toDouble(2500);

    if (defect.type == DEFECT_NORMAL)
    {
        defect.rank = defect.area / maxAreaBody;
    }
    else if (defect.type == DEFECT_EDGE)
    {
        defect.rank = qMax(defect.length / maxLengthEdge, defect.area / maxAreaEdge);
    }
    else if (defect.type == DEFECT_COMBINED_BODY)
    {
        defect.rank = defect.area / maxAreaBody;
    }
    else if (defect.type == DEFECT_COMBINED_EDGE)
    {
        defect.rank = qMax(defect.length / maxLengthEdge, defect.area / maxAreaEdge);
    }
    else
    {
        ;//
    }
}

void BS_EN_ISO_10893::CalcPartResult()
{
    float maxRank = 0.0f;

    double avgNum = options.value("AvgPerMeterNumBody").toDouble(0.5);
    foreach( Defect defect, defects )
    {
        if((defect.type == DEFECT_NORMAL) || (defect.type == DEFECT_EDGE) || (defect.type == DEFECT_COMBINED_BODY) || (defect.type == DEFECT_COMBINED_EDGE)
            || (defect.type == DEFECT_LENGTH_RATIO_BODY))
        {
            maxRank = qMax(maxRank, defect.rank);
        }
    }

    maxRank = qMax(static_cast<float>(GetPartValue("AvgPerMeterRatioBody") / avgNum), maxRank);

    InsertPartMap( "Result", PairPartInfo( "判伤结果", static_cast<double>(maxRank)) );

    return;
}

