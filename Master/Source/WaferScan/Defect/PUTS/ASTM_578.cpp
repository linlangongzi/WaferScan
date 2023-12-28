#include "ASTM_578.h"
#include <QtMath>

IMPLEMENT_RUNTIME(ASTM_578)
ASTM_578::ASTM_578(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options) :
    PUTSRecognizer(transform, imageList, options)
{
    SetPartProperty();
}

void ASTM_578::SetPartProperty()
{
    mapPartProperty.insert("MaxIndiLengthAll", false);
    mapPartProperty.insert("AvgRatioAll", false);
    mapPartProperty.insert("AreaSum", false);
    mapPartProperty.insert("Circlecombination", false);
    mapPartProperty.insert("Result", false);

    return;
}

void ASTM_578::SetDefectRank(Defect &defect)
{
    // 默认合格等级
    double maxLength = options.value("MaxAllowedLength").toDouble(75);
    double thickness = options.value("PlateThickness").toDouble(100);

    if(defect.type == DEFECT_NORMAL || defect.type == DEFECT_EDGE)
    {
        defect.rank = defect.length / qMax(maxLength, thickness / 2);
    }
    else
    {
        ;//
    }

    return;
}

void ASTM_578::CalcPartResult()
{
    float maxRank = 0.0f;

    foreach( Defect defect, defects )
    {
        if((defect.type == DEFECT_NORMAL) || (defect.type == DEFECT_EDGE) || defect.type == (DEFECT_CITCLE_COMBINED))
        {
            maxRank = qMax(maxRank, defect.rank);
        }
    }

    InsertPartMap( "Result", PairPartInfo( "判伤结果", static_cast<double>(maxRank)) );

    return;
}

void ASTM_578::DefectCombine(const QRectF &centerZone, QVector<QPolygonF> boundaryPolygons)
{
    Q_UNUSED(centerZone);
    Q_UNUSED(boundaryPolygons);

    return;
}

/*!
 * \brief 缺陷评定
 */
void ASTM_578::DefectAssessment(const QRectF &centerZone)
{
//    clock_t ctorStart = clock();
    /* 8.2 在钢板周边 50 mm(板厚大于 100 mm 时，取板厚的一半)可检验区域内及坡口预定线两侧各 25 mm 内，单个缺陷的指示长度不得大于或等于 50 mm. */
    //CDataValue edgeMaxLength = options.value("EdgeMaxLength").toDouble(50);
    /* 7.4.1 单个缺陷评定 */
    //CDataValue maxLength = options.value("MaxAllowedLength").toDouble(80);
    //CDataValue maxArea = options.value("MaxAllowedArea").toDouble(2500);
    double maxLength = options.value("MaxAllowedLength").toDouble(75);
    UTImage::Boundaries boundaries = defectImage.FindBoundaries(0, UTImage::THRESHOLD_ABOVE);
  //  QVector<QPolygonF> boundaryPolygons;
    QVector<QPair<QPolygonF, double>> boundaryPolygons;
    int index = 0;

    foreach (UTImage::Boundary boundary, boundaries)
    {
        Defect defect;
        defect.boundary = boundary.first;
        defect.boundaryF = trans.MapFromGrid(boundary.first);
        QPolygonF minRect = FindEnclosingRect(defect.boundaryF);
//        defect.length = QVector2D( minRect.value(0) ).distanceToPoint( QVector2D( minRect.value(2) ) );
        defect.length = qSqrt( ( minRect.value(2).x() - minRect.value(0).x() ) * ( minRect.value(2).x() - minRect.value(0).x() )
            + ( minRect.value(2).y() - minRect.value(0).y() ) * ( minRect.value(2).y() - minRect.value(0).y() ) );
        CDataValue rectWidth = qAbs( minRect.value(1).x() - minRect.value(0).x() );
        CDataValue rectLength = qAbs( minRect.value(2).y() - minRect.value(0).y() );

        defect.rectLength = qMax(rectLength, rectWidth);
        defect.rectWidth = qMin(rectLength, rectWidth);

        defect.area = PolygonArea(defect.boundaryF);

        if ( !centerZone.contains( defect.boundaryF.boundingRect() ) )          /* 不在中央区域，则使用边缘判伤标准 */
        {
            defect.type = DEFECT_EDGE;
            //defect.rank = qMax(defect.length / edgeMaxLength, defect.area / maxArea);
        }
        else
        {
            defect.type = DEFECT_NORMAL;
            // defect.rank = qMax(defect.length / maxLength, defect.area / maxArea);
        }
        SetDefectRank( defect);

        defects.append(defect);
        if (defect.length < maxLength)
        {
            qualifiedDefects.append(defect);
            boundaryPolygons.append(QPair<QPolygonF, double>(defect.boundaryF, defect.length));
        }
        UpdateDefect(index++);
    }

//    qDebug() << GetStandardName() << "single defect took time" << clock() - ctorStart << "ms defect num" <<  defects.count();

    DefectCombine(centerZone, boundaryPolygons);

}

void ASTM_578::DefectCombine(const QRectF &centerZone, QVector<QPair<QPolygonF, double> > boundaryPolygons)
{
    QString jiBie = options.value("JiBie").toString();
    if (jiBie != "B")
    {
        return PUTSRecognizer::DefectCombine(centerZone, boundaryPolygons);
    }
    /* 7.4.2 当多个缺陷的相邻间距小于 100 mm 或间距小于相邻缺陷(以指示长度来比较)的指示长度(取
     *  其较大值)时，其各块缺陷面积之和也作为单个缺陷指示面积。 */
//    clock_t ctorStart = clock();
    double minDefectDistance = qMax(options.value("MaxAllowedLength").toDouble(75), options.value("PlateThickness").toDouble(150));
    if (minDefectDistance > 0)
    {
        /* 使用 GroupPolygons 将缺陷按距离分组，得到各缺陷的分组标签 */
        int groupCount = 0;                                                     /*!< 分组结果中的总分组数 */
        const QVector<int> groupLabels = GroupPolygons(boundaryPolygons, minDefectDistance,
                                                       static_cast<double>(1), groupCount, combineMax);
        Q_ASSERT( groupLabels.count() == qualifiedDefects.count() );

        int defectstart = defects.count();
        /* 新增组合缺陷 */
        defects.resize( defects.size() + groupLabels.count() );

        /* 遍历单缺陷列表，按标签将其特征合并入组合缺陷 */
        for (int index = 0; index < groupLabels.count(); ++index)
        {
            Q_ASSERT(groupLabels[index] < groupCount);
            Defect &defect = qualifiedDefects[index];                                    /*!< 当前单缺陷的引用 */
            int groupLabel = groupLabels[index];
            Defect &combined = defects[defectstart + groupLabel];       /*!< 当前单缺陷对应的组合缺陷的引用 */
            combined.area += defect.area;
            combined.length = qMax(combined.length, defect.length);             /*!< 最大组合伤指示长度 */
            combined.rectLength = qMax(combined.rectLength, defect.rectLength);
            combined.rectWidth = qMax(combined.rectWidth, defect.rectWidth);
          //  combined.boundaryF = combined.boundaryF.boundingRect().united( defect.boundaryF.boundingRect() );
            foreach (QPointF point, defect.boundaryF)
            {
                if (defect.length < 75)
                {
                    combined.boundaryF << point;
                }
            }
            combined.minInterval = 75;
  /*          for (int i = 0; i < index; ++i)
            {
                if (groupLabel == groupLabels[i])
                {
                    combined.minInterval = qMin(static_cast<CDataValue>(PolygonDistance(qualifiedDefects[i].boundaryF, defect.boundaryF)), combined.minInterval);
                }
            }*/

            if ( !centerZone.contains( defect.boundaryF.boundingRect() ) )
            {
                combined.type = DEFECT_COMBINED_EDGE;
            }
            else
            {
                combined.type = DEFECT_COMBINED_BODY;
            }
            SetDefectRank( combined );
        }
        for (int index = 0; index < defects.size(); ++index)
        {
            Defect &combined = defects[index];                                    /*!< 当前单缺陷的引用 */
            if (combined.type == DEFECT_COMBINED_EDGE || combined.type == DEFECT_COMBINED_BODY)
            {
                QPointF center;
                float radius = FindEnclosingCircle(combined.boundaryF, &center);
                combined.radius = radius;
                combined.boundaryF = ApproximateCircle(center.toPoint(), radius);
            }
        }

        for (int i = 0; i < groupLabels.size(); ++i)
        {
            for (int j = 0; j < qualifiedDefects.size(); j++)
            {
                if ((groupLabels[i] == groupLabels[j]) && (i != j))
                {
                    defects[defectstart + i].minInterval = qMin(static_cast<CDataValue>(PolygonDistance(qualifiedDefects[j].boundaryF, qualifiedDefects[i].boundaryF)), defects[defectstart + i].minInterval);
                }
            }
        }
    }

//    qDebug() << GetStandardName() << "combine defect took time" << clock() - ctorStart << "ms defect num" <<  defects.count();

}
