#include "ASTM_435.h"

ASTM_435::ASTM_435(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options) :
    PUTSRecognizer(transform, imageList, options)
{
    SetPartProperty();
}

void ASTM_435::SetPartProperty()
{
    mapPartProperty.insert("MaxIndiLengthAll", false);
    mapPartProperty.insert("AvgRatioAll", false);
    mapPartProperty.insert("AreaSum", false);
    mapPartProperty.insert("Result", false);

}

void ASTM_435::SetDefectRank(Defect &defect)
{
    // 默认合格等级
    double maxLength = options.value("MaxAllowedLength").toDouble(75);
    double thickness = options.value("PlateThickness").toDouble(100);

    if ( (defect.type == DEFECT_NORMAL) || (defect.type == DEFECT_EDGE) )
    {
        defect.rank = defect.length / qMax(maxLength, thickness / 2);
    }
    else
    {
        ;//
    }

}

void ASTM_435::CalcPartResult()
{
    float maxRank = 0.0f;

    foreach ( Defect defect, defects )
    {
        if ( (defect.type == DEFECT_NORMAL) || (defect.type == DEFECT_EDGE) )
        {
            maxRank = qMax(maxRank, defect.rank);
        }
    }

    InsertPartMap( "Result", PairPartInfo( "判伤结果", static_cast<double>(maxRank) ) );

}

void ASTM_435::DefectCombine(const QRectF &centerZone, QVector<QPolygonF> boundaryPolygons)
{
    Q_UNUSED(centerZone);
    Q_UNUSED(boundaryPolygons);

}
