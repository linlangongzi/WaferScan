#include "PUTSRecognizer.h"
#include "GBT_2970.h"
//#include "ASTM_435.h"
#include "ASTM_578.h"
#include "EN_10160.h"
#include "JIS_G0801.h"
#include "JIS_G0901.h"
#include "GBT_5777_2008.h"
#include "GBT_31925_2015.h"
#include "NBT_47013_2015.h"
#include "BS_EN_ISO_10893.h"
#include <QVector2D>
#include <time.h>
#include <QtMath>
#include <QMessageBox>
#include <allrising/Util.h>

#define Field(name, field) PUTSRecognizer::ObjectValueInfo( name, offsetof(PUTSRecognizer::Defect, field) )
IMPLEMENT_RUNTIME(PUTSRecognizer)
PUTSRecognizer::ObjectValueMap PUTSRecognizer::InitValueMap()
{
    ObjectValueMap map;
    map["Length"] =             Field("指示长度", length);
    map["RectWidth"] =          Field("宽度", rectWidth);
    map["RectLength"] =         Field("宽度", rectLength);
    map["Area"] =               Field("面积", area);
    map["Rank"] =               Field("分级", rank);
    map["Type"] =               Field("类型", type);

    map["DefectAmpAvg"] =       Field("平均缺陷波高", defectAmp.avg);
    map["DefectAmpMin"] =       Field("最小缺陷波高", defectAmp.min);
    map["DefectAmpMax"] =       Field("最大缺陷波高", defectAmp.max);
    map["DefectAmpMinLocX"] =   Field("最小缺陷波X", defectAmp.max);
    map["DefectAmpMinLocY"] =   Field("最小缺陷波Y", defectAmp.max);
    map["DefectAmpMaxLocX"] =   Field("最大缺陷波X", defectAmp.max);
    map["DefectAmpMaxLocY"] =   Field("最大缺陷波Y", defectAmp.max);

    map["DefectDepthAvg"] =     Field("平均缺陷深度", defectDepth.avg);
    map["DefectDepthMin"] =     Field("最小缺陷深度", defectDepth.min);
    map["DefectDepthMax"] =     Field("最大缺陷深度", defectDepth.max);
    map["DefectDepthMinLocX"] = Field("最小缺陷深X", defectAmp.max);
    map["DefectDepthMinLocY"] = Field("最小缺陷深Y", defectAmp.max);
    map["DefectDepthMaxLocX"] = Field("最大缺陷深X", defectAmp.max);
    map["DefectDepthMaxLocY"] = Field("最大缺陷深Y", defectAmp.max);

    map["BottomAmpAvg"] =       Field("平均底波波高", bottomAmp.avg);
    map["BottomAmpMin"] =       Field("最小底波波高", bottomAmp.min);
    map["BottomAmpMax"] =       Field("最大底波波高", bottomAmp.max);
    map["BottomAmpMinLocX"] =   Field("最小底波X", defectAmp.max);
    map["BottomAmpMinLocY"] =   Field("最小底波Y", defectAmp.max);
    map["BottomAmpMaxLocX"] =   Field("最大底波X", defectAmp.max);
    map["BottomAmpMaxLocY"] =   Field("最大底波Y", defectAmp.max);

    map["BottomDepthAvg"] =     Field("平均底波深度", bottomDepth.avg);
    map["BottomDepthMin"] =     Field("最小底波深度", bottomDepth.min);
    map["BottomDepthMax"] =     Field("最大底波深度", bottomDepth.max);
    map["BottomDepthMinLocX"] = Field("最小底波深X", defectAmp.max);
    map["BottomDepthMinLocY"] = Field("最小底波深Y", defectAmp.max);
    map["BottomDepthMaxLocX"] = Field("最大底波深X", defectAmp.max);
    map["BottomDepthMaxLocY"] = Field("最大底波深Y", defectAmp.max);
    return map;
}

PUTSRecognizer::ObjectValueMap PUTSRecognizer::valueMap = InitValueMap();

PUTSRecognizer::PUTSRecognizer(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options) :
    DefectRecognizer(transform, imageList, options),
    step( QSize(4, 4) ),
    combineMax(true),
    blockSize(QSize(1000, 1000))
{
    InitPartFuncMap();
}

PUTSRecognizer *PUTSRecognizer::Create(const QString &name, const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options)
{
    if (name == "GBT2970" || name == "JBT4730") {
        return new GBT_2970(transform, imageList, options);
    } else if (name == "ASTM 578" || name == "ASTM 578M") {
        return new ASTM_578(transform, imageList, options);
    } else if (name == "ASTM 435" || name == "ASTM 435M") {
    //    return new ASTM_435(transform, imageList, options);
    } else if (name == "PrEn10160") {
        return new EN_10160(transform, imageList, options);
    } else if (name == "JIS G0801") {
        return new JIS_G0801(transform, imageList, options);
    } else if (name == "JIS G0901") {
        return new JIS_G0901(transform, imageList, options);
    } else if (name == "BS_EN_ISO_10893") {
        return new BS_EN_ISO_10893(transform, imageList, options);
    } else if (name == "GBT_5777_2008") {
        return new GBT_5777_2008(transform, imageList, options);
    } else if (name == "GBT_31925_2005") {
        return new GBT_31925_2015(transform, imageList, options);
    } else if (name == "NBT_47013_2015") {
        return new NBT_47013_2015(transform, imageList, options);
    } else {
        QMessageBox::information(NULL, "PUTSRecognizer::Create", "无效标准");
    }

    return NULL;
}

QStringList PUTSRecognizer::GetPartKeys() const
{
//    1、最大单位面积缺陷数（本体，边沿，所有）；
//    2、平均缺陷率（本体，边沿）
//    3、最大缺陷率（本体，边沿）
//    4、最大指示距离（本体，边沿）
//    5、最大缺陷面积（本体，边沿）
//    6、最大长度（本体，边沿）
//    7、最大宽度（本体，边沿）
//    8、缺陷率（所有）
//    9、缺陷最大长度（所有）
//    10、缺陷最大宽度（所有）

    return QStringList() << "Class";
}

QString PUTSRecognizer::GetPartKeyDescription(const QString &key) const
{
    return mapPart.value(key).first;
}

CDataValue PUTSRecognizer::GetPartValue(const QString &key) const
{
    return mapPart.value(key).second;
}

QStringList PUTSRecognizer::GetObjectKeys() const
{
    return valueMap.keys();
}

QString PUTSRecognizer::GetObjectKeyDescription(const QString &key) const
{
    return valueMap[key].first;
}

int PUTSRecognizer::GetNumObjects() const
{
    return defects.count();
}

QPolygonF PUTSRecognizer::GetObjectBoundary(int index) const
{
    return defects.value(index).boundaryF;
}

CDataValue PUTSRecognizer::GetObjectValue(int index, const QString &key) const
{
    ARS_CHECK_RETURN(UpdateDefect(index), 0);
    ARS_CHECK_RETURN(valueMap.contains(key), 0);
    const Defect &defect = defects[index];
    ptrdiff_t offset = valueMap[key].second;
    return *reinterpret_cast<const CDataValue *>(reinterpret_cast<const char *>(&defect) + offset);
}

void PUTSRecognizer::InsertPartMap(const QString &key, PairPartInfo pairPartInfo)
{
    mapPart.insert(key, pairPartInfo);
}

void PUTSRecognizer::UpdateStatistics(PUTSRecognizer::Statistics &statistics, const UTImage &source, const UTImage &mask, const QRect &ROI) const
{
    UTImage filteredMask = source.Copy(ROI).And(mask);
    statistics.avg = source.GetMean(&filteredMask, ROI);
    QPoint minLoc;
    QPoint maxLoc;
    source.GetMinMax(&statistics.min, &statistics.max, &minLoc, &maxLoc, &filteredMask, ROI);
    QPointF minLocF = trans.MapFromGrid(minLoc);
    QPointF maxLocF = trans.MapFromGrid(maxLoc);
    statistics.minLocX = minLocF.x();
    statistics.minLocY = minLocF.y();
    statistics.maxLocX = maxLocF.x();
    statistics.maxLocY = maxLocF.y();
}

bool PUTSRecognizer::UpdateDefect(int index) const
{
    ARS_CHECK_BOUND_RETURN(0, index, defects.count() - 1, false);
    Defect &defect = defects[index];
    ARS_CHECK_RETURN(!defect.isValid, true);

//    static QMargins margin(5, 5, 5, 5);
    //QRect boundingRect = defect.boundary.boundingRect().marginsAdded(margin);
    QRect boundingRect = defect.boundary.boundingRect().adjusted(-5, -5, 5, 5);
    QRect ROI = defectImage.BoundRect(boundingRect);
    UTImage defectMask( ROI.size() );
    defectMask.DrawPolygon(defect.boundary.translated( -ROI.topLeft() ), 255);
    defectMask.Dilate(1);
//    defectMask.DebugPrint("DefectMask");

    UpdateStatistics(defect.defectAmp, images.value(defectAmpIndex), defectMask, ROI);
    UpdateStatistics(defect.defectDepth, images.value(defectDepthIndex), defectMask, ROI);
    UpdateStatistics(defect.bottomAmp, images.value(bottomAmpIndex), defectMask, ROI);
    UpdateStatistics(defect.bottomDepth, images.value(bottomDepthIndex), defectMask, ROI);

    defect.isValid = true;
    return true;
}

/*!
 * \brief 计算平均缺陷率(所有, 本体, 边沿)
 */
void PUTSRecognizer::CalcAvgDefectRatio()
{
    // 所有
    CalcAvgDefectRatioAll();

    // 本体
    CalcAvgDefectRatioBody();

    // 边沿
    CalcAvgDefectRatioEdge();

}

void PUTSRecognizer::CalcAvgDefectRatioAll()
{
    double avgRatio = defectImage.CalcDensity(NULL, defectImage.GetRect());
    InsertPartMap( "AvgRatioAll", PairPartInfo( "所有平均缺陷率", avgRatio ) );

}

void PUTSRecognizer::CalcAvgDefectRatioBody()
{
    double avgRatioBody = defectImage.CalcDensity(NULL, rectBody);
    InsertPartMap( "AvgRatioBody", PairPartInfo( "主体平均缺陷率", avgRatioBody ) );

}

void PUTSRecognizer::CalcAvgDefectRatioEdge()
{
    UTImage borderMask( defectImage.GetSize(), 1 );
    QPolygon polygonBody;
    polygonBody.append( rectBody.topLeft() );
    polygonBody.append( rectBody.bottomLeft() );
    polygonBody.append( rectBody.bottomRight() );
    polygonBody.append( rectBody.topRight() );
    borderMask.DrawPolygon(polygonBody, 0);
    double avgRatioEdge = defectImage.CalcDensity( &borderMask, defectImage.GetRect() );
    InsertPartMap( "AvgRatioEdge", PairPartInfo( "边沿平均缺陷率", avgRatioEdge ) );

}

/*! \brief 计算最大缺陷率(所有, 本体, 边沿) */
void PUTSRecognizer::CalcMaxDefectRatio()
{
    clock_t ctorStart = clock();
    // 所有
    CalcMaxDefectRatioAll();

    // 本体
    CalcMaxDefectRatioBody();

    // 边沿
    CalcMaxDefectRatioEdge();

    qDebug() << GetStandardName() << " max defect ratio took time " << clock() - ctorStart << "ms";

}

void PUTSRecognizer::CalcMaxDefectRatioAll()
{
    QSize block = trans.MapToGrid( blockSize );
//    QSize step = QSize(4, 4);
    CDataValue maxDensity = options.value("MaxAllowedPercent").toDouble(0.03);
    QPair<double, QRect> density = defectImage.FindMaxDensity(block);

    if (density.first > maxDensity)
    {
        Defect densityDefect;
        densityDefect.boundary = density.second;
        densityDefect.boundaryF = trans.MapFromGrid(densityDefect.boundary);
        densityDefect.area = density.first;
        densityDefect.type = DEFECT_DENSITY;
        densityDefect.rank = density.first / maxDensity;
        defects << densityDefect;
    }

    InsertPartMap( "MaxDensityAll", PairPartInfo( "所有最大缺陷率", density.first ) );

}

void PUTSRecognizer::CalcMaxDefectRatioBody()
{
    QSize block = trans.MapToGrid( blockSize );
    //   QSize step = QSize(4, 4);
    CDataValue maxDensity = options.value("MaxAllowedPercentBody").toDouble(0.15);
    QPair<double, QRect> density = defectImage.FindMaxDensity(block, step, rectBody);
    if (density.first > maxDensity)
    {
        Defect densityDefect;
        densityDefect.boundary = density.second;
        densityDefect.boundaryF = trans.MapFromGrid(densityDefect.boundary);
        densityDefect.area = density.first;
        densityDefect.type = DEFECT_DENSITY_BODY;
        densityDefect.rank = density.first / maxDensity;
        defects << densityDefect;
    }

    InsertPartMap( "MaxRatioBody", PairPartInfo( "主体最大缺陷率", density.first ) );
}

void PUTSRecognizer::CalcMaxDefectRatioEdge()
{
    QSize block = trans.MapToGrid( blockSize );
//    QSize step = QSize(4, 4);
    CDataValue maxDensity = options.value("MaxAllowedPercentEdge").toDouble(0.2);

//    QMap<double, int> vecValue;
    QMap<double, QPair<double, QRect>> vecValue;
    QPair<double, QRect> densityL = defectImage.FindMaxDensity(block, step, rectEdgeL);
    vecValue.insert(densityL.first, densityL);
    QPair<double, QRect> densityR = defectImage.FindMaxDensity(block, step, rectEdgeR);
    vecValue.insert(densityR.first, densityR);
    QPair<double, QRect> densityB = defectImage.FindMaxDensity(block, step, rectEdgeB);
    vecValue.insert(densityB.first, densityB);
    QPair<double, QRect> densityT = defectImage.FindMaxDensity(block, step, rectEdgeT);
    vecValue.insert(densityT.first, densityT);
//    vecValue.insert(defectImage.FindMaxDensity(block, step, rectEdgeL).first, 0);
//    vecValue.insert(defectImage.FindMaxDensity(block, step, rectEdgeR).first, 0);
//    vecValue.insert(defectImage.FindMaxDensity(block, step, rectEdgeB).first, 0);
//    vecValue.insert(defectImage.FindMaxDensity(block, step, rectEdgeC).first, 0);

    QList<double> lisValue = vecValue.keys();

    QPair<double, QRect> density = vecValue.value(lisValue.last());
    if (density.first > maxDensity)
    {
        Defect densityDefect;
        densityDefect.boundary = density.second;
        densityDefect.boundaryF = trans.MapFromGrid(densityDefect.boundary);
        densityDefect.area = density.first;
        densityDefect.type = DEFECT_DENSITY_EDGE;
        densityDefect.rank = density.first / maxDensity;
        defects << densityDefect;
    }

    InsertPartMap( "MaxRatioEdge", PairPartInfo( "边沿最大缺陷率", lisValue.last() ) );

}

/*!
 * \brief 最大指示距离（本体，边沿）
 *  最大缺陷面积（本体，边沿）
 *  最大长度（本体，边沿）
 *  最大宽度（本体，边沿）
 *  缺陷最大长度（所有）
 *  缺陷最大宽度（所有）
 */
void PUTSRecognizer::CalcDefectMaxs()
{
    double maxIndiLengthAll = 0.0f;                                             // 所有最大指示长度
    double maxIndiLengthBody = 0.0f;                                            // 主体最大指示长度
    double maxIndiLengthEdge = 0.0f;                                            // 边沿最大指示长度

    double maxAreaAll = 0.0f;                                                   // 所有最大缺陷面积
    double maxAreaBody = 0.0f;                                                  // 主体最大缺陷面积
    double maxAreaEdge = 0.0f;                                                  // 边沿最大缺陷面积

    double maxLengthBody = 0.0f;                                                // 主体缺陷最大长度
    double maxLengthEdge = 0.0f;                                                // 边沿缺陷最大长度
    double maxLengthAll = 0.0f;                                                 // 所有缺陷最大长度

    double maxWidthBody = 0.0f;                                                 // 主体缺陷最大宽度
    double maxWidthEdge = 0.0f;                                                 // 边沿缺陷最大宽度
    double maxWidthAll = 0.0f;                                                  // 所有缺陷最大宽度

    foreach (Defect defect, defects)
    {
        maxLengthAll = qMax<double>(maxLengthAll, defect.rectLength);
        maxWidthAll = qMax<double>(maxWidthAll, defect.rectWidth);
        maxIndiLengthAll = qMax<double>(maxIndiLengthAll, defect.length);
        maxAreaAll = qMax<double>(maxAreaAll, defect.area);

        if (defect.type == DEFECT_NORMAL)                                       // 主体
        {
            maxIndiLengthBody = qMax<double>(maxIndiLengthBody, defect.length);
            maxAreaBody = qMax<double>(maxAreaBody, defect.area);
            maxLengthBody = qMax<double>(maxLengthBody, defect.rectLength);
            maxWidthBody = qMax<double>(maxWidthBody, defect.rectWidth);
        }
        else if (defect.type == DEFECT_EDGE)                                    // 加沿
        {
            maxIndiLengthEdge = qMax<double>(maxIndiLengthEdge, defect.length);
            maxAreaEdge = qMax<double>(maxAreaEdge, defect.area);
            maxLengthEdge = qMax<double>(maxLengthEdge, defect.rectLength);
            maxWidthEdge = qMax<double>(maxWidthEdge, defect.rectWidth);
        }

    }

    InsertPartMap( "MaxIndiLengthAll", PairPartInfo( "所有最大缺陷指示长度", maxIndiLengthAll ) );
    InsertPartMap( "MaxIndiLengthBody", PairPartInfo( "主体最大缺陷指示长度", maxIndiLengthBody ) );
    InsertPartMap( "MaxIndiLengthEdge", PairPartInfo( "边沿最大缺陷指示长度", maxIndiLengthEdge ) );

    InsertPartMap( "MaxAreaBodyAll", PairPartInfo( "所有最大缺陷面积", maxAreaAll ) );
    InsertPartMap( "MaxAreaBody", PairPartInfo( "主体最大缺陷面积", maxAreaBody ) );
    InsertPartMap( "MaxAreaEdge", PairPartInfo( "边沿最大缺陷面积", maxAreaEdge ) );

    InsertPartMap( "MaxLengthBody", PairPartInfo( "边沿最大缺陷长度", maxLengthBody ) );
    InsertPartMap( "MaxLengthEdge", PairPartInfo( "边沿最大缺陷长度", maxLengthEdge ) );
    InsertPartMap( "MaxLengthAll", PairPartInfo( "边沿最大缺陷长度", maxLengthAll ) );

    InsertPartMap( "MaxWidthBody", PairPartInfo( "边沿最大缺陷宽度", maxWidthBody ) );
    InsertPartMap( "MaxWidthEdge", PairPartInfo( "边沿最大缺陷宽度", maxWidthEdge ) );
    InsertPartMap( "MaxWidthAll", PairPartInfo( "边沿最大缺陷宽度", maxWidthAll ) );
}

void PUTSRecognizer::CalcMaxIndiLengthAll()
{
    double max = 0.0f;

    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_NORMAL || defect.type == DEFECT_EDGE)
        {
            max = qMax<double>(max, defect.length);
        }
    }

    InsertPartMap( "MaxIndiLengthAll", PairPartInfo( "所有最大缺陷指示长度", max) );
}

void PUTSRecognizer::CalcMaxIndiLengthBody()
{
    double max = 0.0f;

    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_NORMAL)
        {
            max = qMax<double>(max, defect.length);
        }
    }

    InsertPartMap( "MaxIndiLengthBody", PairPartInfo( "主体最大缺陷指示长度", max) );

}

void PUTSRecognizer::CalcMaxIndiLengthEdge()
{
    double max = 0.0f;

    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_EDGE)
        {
            max = qMax<double>(max, defect.length);
        }
    }

    InsertPartMap( "MaxIndiLengthEdge", PairPartInfo( "边沿最大缺陷指示长度", max) );

}

void PUTSRecognizer::CalcMaxAreaAll()
{
    double max = 0.0f;

    foreach (Defect defect, defects)
    {
        if ( (defect.type == DEFECT_NORMAL) || (defect.type == DEFECT_EDGE)
        || ( defect.type == DEFECT_COMBINED_BODY) || ( defect.type == DEFECT_COMBINED_EDGE) )
        {
            max = qMax<double>(max, defect.area);
        }
    }

    InsertPartMap( "MaxAreaAll", PairPartInfo( "所有最大缺陷面积", max) );
}

void PUTSRecognizer::CalcMaxAreaBody()
{
    double max = 0.0f;

    foreach (Defect defect, defects)
    {
        if ( (defect.type == DEFECT_NORMAL) || (defect.type == DEFECT_COMBINED_BODY) )
        {
            max = qMax<double>(max, defect.area);
        }
    }

    InsertPartMap( "MaxAreaBody", PairPartInfo( "主体最大缺陷面积", max) );
}

void PUTSRecognizer::CalcMaxAreaEdge()
{
    double max = 0.0f;

    foreach (Defect defect, defects)
    {
        if ( (defect.type == DEFECT_EDGE) || (defect.type == DEFECT_COMBINED_EDGE) )
        {
            max = qMax<double>(max, defect.area);
        }
    }

    InsertPartMap( "MaxAreaEdge", PairPartInfo( "边沿最大缺陷面积", max) );
}

void PUTSRecognizer::CalcMaxLengthAll()
{
    double max = 0.0f;

    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_NORMAL || defect.type == DEFECT_EDGE
                || defect.type == DEFECT_COMBINED_BODY || defect.type == DEFECT_COMBINED_EDGE)
        {
            max = qMax<double>(max, defect.rectLength);
        }
    }

    InsertPartMap( "MaxLengthAll", PairPartInfo( "所有最大缺陷长度", max) );

}

void PUTSRecognizer::CalcMaxLengthBody()
{
    double max = 0.0f;

    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_NORMAL)
        {
            max = qMax<double>(max, defect.rectLength);
        }
    }

    InsertPartMap( "MaxLengthBody", PairPartInfo( "主体最大缺陷长度", max) );
}

void PUTSRecognizer::CalcMaxLengthEdge()
{
    double max = 0.0f;

    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_EDGE)
        {
            max = qMax<double>(max, defect.rectLength);
        }
    }

    InsertPartMap( "MaxLengthEdge", PairPartInfo( "边沿最大缺陷长度", max) );

}

void PUTSRecognizer::CalcMaxWidthAll()
{
    double max = 0.0f;

    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_NORMAL || defect.type == DEFECT_EDGE
                || defect.type == DEFECT_COMBINED_BODY || defect.type == DEFECT_COMBINED_EDGE)
        {
            max = qMax<double>(max, defect.rectWidth);
        }
    }

    InsertPartMap( "MaxWidthAll", PairPartInfo( "所有最大缺陷宽度", max) );

}

void PUTSRecognizer::CalcMaxWidthBody()
{
    double max = 0.0f;

    foreach (Defect defect, defects)
    {
        if ( (defect.type == DEFECT_NORMAL) || (defect.type == DEFECT_COMBINED_BODY) )
        {
            max = qMax<double>(max, defect.rectWidth);
        }
    }

    InsertPartMap( "MaxWidthBody", PairPartInfo( "主体最大缺陷宽度", max) );

}

void PUTSRecognizer::CalcMaxWidthEdge()
{
    double max = 0.0f;

    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_EDGE || defect.type == DEFECT_COMBINED_EDGE)
        {
            max = qMax<double>(max, defect.rectWidth);
        }
    }

    InsertPartMap( "MaxWidthEdge", PairPartInfo( "边沿最大缺陷宽度", max) );

}

void PUTSRecognizer::CalcPartResult()
{
    float maxRank = 0.0f;

    foreach ( Defect defect, defects )
    {
        maxRank = qMax(maxRank, defect.rank);
    }

    InsertPartMap( "Result", PairPartInfo( "判伤结果", static_cast<double>(maxRank) ) );

}

void PUTSRecognizer::CalcPartResultBody()
{
    float maxRank = 0.0f;

    foreach ( Defect defect, defects )
    {
        if ( (defect.type == DEFECT_NORMAL) || (defect.type == DEFECT_COMBINED_BODY) )
        {
            maxRank = qMax(maxRank, defect.rank);
        }
    }

    InsertPartMap( "ResultBody", PairPartInfo( "主体判伤结果", static_cast<double>(maxRank) ) );

}

void PUTSRecognizer::CalcPartResultEdge()
{
    float maxRank = 0.0f;

    foreach ( Defect defect, defects )
    {
        if ( (defect.type == DEFECT_EDGE) || (defect.type == DEFECT_COMBINED_EDGE) )
        {
            maxRank = qMax(maxRank, defect.rank);
        }
    }

    InsertPartMap( "ResultEdge", PairPartInfo( "边沿判伤结果", static_cast<double>(maxRank) ) );

}

void PUTSRecognizer::CalcAreaSum()
{
    float sumArea = 0.0f;
    foreach ( Defect defect, defects )
    {
        if ( (defect.type == DEFECT_EDGE) || (defect.type == DEFECT_NORMAL) )
        {
            sumArea = sumArea + defect.area;
        }
    }

    InsertPartMap( "AreaSum", PairPartInfo( "伤点总面积", static_cast<double>(sumArea) ) );
}

void PUTSRecognizer::CalcCirclecombination()
{
    double minDefectDistance = options.value("MaxAllowedLength").toDouble(75);
    double thickness = options.value("PlateThickness").toDouble(100);
    minDefectDistance = qMax(minDefectDistance, thickness / 2);
    float maxRank = 0.0f;
    foreach (Defect defect, defects)
    {
        if ((defect.type == DEFECT_COMBINED_BODY) || (defect.type == DEFECT_COMBINED_EDGE))
        {
            defect.rank = qMin(static_cast<CDataValue>(defect.length / defect.minInterval), static_cast<CDataValue>((defect.radius * 2) / minDefectDistance));

            if (defect.rank  > 1.0)
            {
                Defect densityDefect;
                densityDefect.boundary = defect.boundary;
                densityDefect.boundaryF = defect.boundaryF;
                densityDefect.area = defect.area;
                densityDefect.type = DEFECT_CITCLE_COMBINED;
                densityDefect.rank = defect.rank ;
                defects << densityDefect;
            }
            maxRank = qMax(maxRank, defect.rank);
        }
    }
    InsertPartMap( "Circlecombination", PairPartInfo( "伤点总面积", static_cast<double>(maxRank) ) );
}

void PUTSRecognizer::CalcMaxPerMeterNumEdge()
{
 //   QSize block = trans.MapToGrid( blockSize );
    QSize block = trans.MapToGrid(QSize(options.value("BlockSizeW").toInt(), 1000));
//    QSize step = QSize(4, 4);
    qint32 maxNum = options.value("MaxPerMeterNumEdge").toInt(2);
    QVector<QRect> vecObjRect;
    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_EDGE)
        {
            QRectF rect = defect.boundaryF.boundingRect();
            vecObjRect.append( trans.MapToGrid(rect) );
        }
    }

    QPair<int, QRect> densityL = FindMaxObjectDensity(vecObjRect, block, rectEdgeL);
    QPair<int, QRect> densityB = FindMaxObjectDensity(vecObjRect, block, rectEdgeB);
    QPair<int, QRect> densityR = FindMaxObjectDensity(vecObjRect, block, rectEdgeR);
    QPair<int, QRect> densityT = FindMaxObjectDensity(vecObjRect, block, rectEdgeT);

    QPair<int, QRect> density = densityL;
    density = densityB.first > density.first ? densityB : density;
    density = densityR.first > density.first ? densityR : density;
    density = densityT.first > density.first ? densityR : density;
    qint32 num = density.first;

    if (num > maxNum)
    {
        Defect densityDefect;
        densityDefect.boundary = density.second;
        densityDefect.boundaryF = trans.MapFromGrid(densityDefect.boundary);
        densityDefect.area = num;
        densityDefect.type = DEFECT_LENGTH_NUM;
        densityDefect.rank = num / maxNum;
        defects << densityDefect;
    }

    InsertPartMap( "PerMeterMaxNumEdge", PairPartInfo( "MaxPerMeterNumEdge", static_cast<double>(num) ) );
}

void PUTSRecognizer::CalcMaxDensityNumBody()
{
    QSize block = trans.MapToGrid( blockSize );
//    QSize step = QSize(4, 4);
    qint32 maxNum = options.value("MaxDensityNumBody").toInt(10);
    QVector<QRect> vecObjRect;
    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_NORMAL)
        {
            QRectF rect = defect.boundaryF.boundingRect();
            vecObjRect.append( trans.MapToGrid(rect) );
        }
    }

    QPair<int, QRect> density = FindMaxObjectDensity(vecObjRect, block, rectBody);
    qint32 num = density.first;

    if (num > maxNum)
    {
        Defect densityDefect;
        densityDefect.boundary = density.second;
        densityDefect.boundaryF = trans.MapFromGrid(densityDefect.boundary);
        densityDefect.area = num;
        densityDefect.type = DEFECT_DENSITY_NUM;
        densityDefect.rank = num / maxNum;
        defects << densityDefect;
    }

    InsertPartMap( "PerSquareMeterMaxNumBody", PairPartInfo( "主体每平米缺陷数量", static_cast<double>(num) ) );
}

void PUTSRecognizer::CalcMaxPerMeterNumBody()
{
       QSize block = trans.MapToGrid(QSize(options.value("BlockSizeW").toInt(), options.value("BlockSizeH").toInt()));
       qint32 maxNum = options.value("MaxPerMeterNumBody").toInt(1);
       double minArea = options.value("MinAllowedAreaBody").toDouble();
       double maxArea = options.value("MaxAllowedAreaBody").toDouble();
       QVector<QRect> vecObjRect;
       foreach (Defect defect, defects)
       {
           if (defect.type == DEFECT_NORMAL && defect.area >= minArea && defect.area <= maxArea)
           {
               QRectF rect = defect.boundaryF.boundingRect();
               vecObjRect.append( trans.MapToGrid(rect) );
           }
       }

       QPair<int, QRect> density = FindMaxObjectDensity(vecObjRect, block, rectBody);
       qint32 num = density.first;
       density.second.setTop(rectBody.top());
       density.second.setBottom(rectBody.bottom());
       if (num > maxNum)
       {
           Defect densityDefect;
           densityDefect.boundary = density.second;
           densityDefect.boundaryF = trans.MapFromGrid(densityDefect.boundary);
           densityDefect.area = num;
           densityDefect.type = DEFECT_LENGTH_NUM_BODY;
           densityDefect.rank = num / maxNum;
           defects << densityDefect;
       }

       InsertPartMap( "MaxPerMeterNumBody", PairPartInfo( "本体每米最大伤点数", static_cast<double>(num) ) );
}

void PUTSRecognizer::CalcAvgPerMeterNumBody()
{
    double plateLength = options.value("PlateLength").toDouble() / 100;  /*!< 除以100单位由毫米换为米 */
    double minArea = options.value("MinAllowedAreaBody").toDouble();
    double maxArea = options.value("MaxAllowedAreaBody").toDouble();
    QVector<QRect> vecObjRect;
    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_NORMAL && defect.area >= minArea && defect.area <= maxArea)
        {
            QRectF rect = defect.boundaryF.boundingRect();
            vecObjRect.append( trans.MapToGrid(rect) );
        }
    }
    double num = static_cast<double>(vecObjRect.size()) / plateLength ;
    InsertPartMap( "AvgPerMeterNumBody", PairPartInfo( "本体每米平均伤点数", static_cast<double>(num) ) );
}

void PUTSRecognizer::CalcMaxPerMeterRatioBody()
{
    QSize block = trans.MapToGrid(QSize(options.value("BlockSizeW").toInt(), options.value("BlockSizeH").toInt()));
    double blockArea = options.value("BlockSizeW").toInt() * options.value("BlockSizeH").toInt();
    qint32 maxNum = options.value("MaxPerMeterNumBody").toInt(1);
    double minArea = options.value("MinAllowedAreaBody").toDouble();
    double maxArea = options.value("MaxAllowedAreaBody").toDouble();
    double maxPercent = 0;
    QVector<QPair<QRect, CDataValue>> vecObjRect;

    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_NORMAL && defect.area >= minArea && defect.area <= maxArea)
        {
            QPair<QRect, CDataValue> temp;
            temp.first = trans.MapToGrid(defect.boundaryF.boundingRect());
            temp.second = defect.area;
            vecObjRect.append(temp);
        }
    }
    for (int i = rectBody.left(); i < rectBody.right() - block.width(); i++)
    {
        QRect rectBlock = QRect(i, rectBody.top(), block.width(), block.height());
        double sumArea = 0;
//        foreach (QPair<QRect, CDataValue> temp, vecObjRect)
//        {
//            if (rectBlock.intersects(temp.first))
//            {
//                sumArea = sumArea + temp.second;
//            }
//        }
        for (int i = 0; i < vecObjRect.count(); i++)
        {
            if (rectBlock.intersects(vecObjRect.value(i).first))
            {
                sumArea = sumArea + vecObjRect.value(i).second;
            }
        }
        double percent = sumArea / blockArea * 100;
        if (percent > maxNum)
        {
            Defect densityDefect;
            densityDefect.boundary = QPolygon(rectBlock);
            densityDefect.boundaryF = trans.MapFromGrid(densityDefect.boundary);
            densityDefect.area = sumArea;
            densityDefect.type = DEFECT_LENGTH_RATIO_BODY;
            densityDefect.rank = percent / maxNum;
            defects << densityDefect;
        }
        maxPercent = qMax(maxPercent, percent);
    }

    InsertPartMap( "MaxPerMeterRatioBody", PairPartInfo( "本体每米最大占积比", static_cast<double>(maxPercent) ) );
}

void PUTSRecognizer::CalcAvgPerMeterRatioBody()
{
    double plateLength = options.value("PlateLength").toDouble();
    double bodyHeight = options.value("BlockSizeH").toDouble();
    double minArea = options.value("MinAllowedAreaBody").toDouble();
    double maxArea = options.value("MaxAllowedAreaBody").toDouble();
    double bodyArea = plateLength * bodyHeight;
    double defectSumArea = 0;
    foreach (Defect defect, defects)
    {
        if (defect.type == DEFECT_NORMAL && defect.area >= minArea && defect.area <= maxArea)
        {
            defectSumArea += defect.area;
        }
    }
    double percent = defectSumArea / bodyArea * 100;
    InsertPartMap( "AvgPerMeterRatioBody", PairPartInfo( "本体平均占积比", static_cast<double>(percent) ) );
}

void PUTSRecognizer::GenerateDefectImage()
{
    UTImage F1 = images.value(defectAmpIndex);
    F1.DebugPrint("DefectAmp");
    UTImage B1 = images.value(bottomAmpIndex);
    B1.DebugPrint("BottomAmp");

    /* 7.1.1 缺陷第一次反射波(F1)波高大于或等于满刻度的50% */
    CDataValue defectAmpThreshold = options.value("DefectAmpThreshold").toDouble(50);
    UTImage rule1 = F1.Compare(defectAmpThreshold, UTImage::CMP_GT);
    rule1.DebugPrint("rule1");

    /* 7.1.2 当底面第一次反射波(B1)波高未达到满刻度时，缺陷第一次反射波(F1)波高与底面第一次反射波(B1)波高之比大于或等于50% */
    CDataValue bottomAmpMax = options.value("BottomAmpMax").toDouble(100);
    CDataValue bottomDefectRatio = options.value("BottomDefectRatio").toDouble(0.5);
    UTImage rule2 = B1.CompareRange(0, bottomAmpMax).And( F1.Copy().Divide(B1).Compare(bottomDefectRatio, UTImage::CMP_GT) );
    rule2.DebugPrint("rule2");

    /* 7.1.3 当底面(或板端部)第一次反射波(B1)波高低于满刻度的50% */
    CDataValue bottomAmpThreshold = options.value("BottomAmpThreshold").toDouble(50);
    UTImage rule3 = B1.CompareRange(0, bottomAmpThreshold);
    rule3.DebugPrint("rule3");

    defectImage = rule1.Or( rule2.Or(rule3) ).DilateDiscrete();
    defectImage.DebugPrint("DefectImage");
}

void PUTSRecognizer::CalcDefect()
{
//    clock_t ctorStart = clock();

    CDataValue edgeZoneWidth = options.value("EdgeZoneWidth").toDouble(50);
//    CDataValue edgeMaxLength = options.value("EdgeMaxLength").toDouble(50);
//    const QRectF centerZone = trans.sourceRect().marginsRemoved( QMarginsF(edgeZoneWidth, edgeZoneWidth, edgeZoneWidth, edgeZoneWidth) );
    const QRectF centerZone = trans.sourceRect().adjusted(edgeZoneWidth, edgeZoneWidth, -edgeZoneWidth, -edgeZoneWidth);

    // 边沿
    rectBody = trans.MapToGrid(centerZone);
    QRectF rectEdgeLF = trans.sourceRect().adjusted( 0, 0, -( edgeZoneWidth + centerZone.height() ), 0); // 左边边沿矩形
    QRectF rectEdgeBF = trans.sourceRect().adjusted( 0, ( edgeZoneWidth + centerZone.height() ), 0, 0 ); // 底部边沿矩形
    QRectF rectEdgeRF = trans.sourceRect().adjusted( ( edgeZoneWidth + centerZone.height() ), 0, 0, 0 ); // 右边边沿矩形
    QRectF rectEdgeTF = trans.sourceRect().adjusted( 0, 0, 0, -( edgeZoneWidth + centerZone.height() ) );// 顶部边沿矩形
    rectEdgeL = trans.MapToGrid(rectEdgeLF); // 左边边沿网格化后矩形
    rectEdgeB = trans.MapToGrid(rectEdgeBF); // 底部边沿网格化后矩形
    rectEdgeR = trans.MapToGrid(rectEdgeRF);// 右边边沿网格化后矩形
    rectEdgeT = trans.MapToGrid(rectEdgeTF);

    defectAmpIndex = options.value("DefectAmpIndex").toInt(0);
    defectDepthIndex = options.value("DefectDepthIndex").toInt(1);
    bottomAmpIndex = options.value("BottomAmpIndex").toInt(2);
    bottomDepthIndex = options.value("BottomDepthIndex").toInt(3);

    GenerateDefectImage();

//     qDebug() << GetStandardName() << "starting single defect" << defects.count() << clock() - ctorStart << "ms";

    /* 缺陷评定 */
    DefectAssessment(centerZone);

    //计算各项工件的缺陷属性
    CalcPartProperty();

//     qDebug() << GetStandardName() << "CalcDefect took time" << clock() - ctorStart << "ms defect num" << defects.count();
}

/*!
 * \brief 设置工件属性
 */
void PUTSRecognizer::SetPartProperty()
{}

/*!
 * \brief 初始化计算工件属性对应的函数指针的映射
 */
void PUTSRecognizer::InitPartFuncMap()
{
    mapPartFunc.insert("AvgRatio", &PUTSRecognizer::CalcAvgDefectRatio);
    mapPartFunc.insert("AvgRatioAll", &PUTSRecognizer::CalcAvgDefectRatioAll);
    mapPartFunc.insert("AvgRatioBody", &PUTSRecognizer::CalcAvgDefectRatioBody);
    mapPartFunc.insert("AvgRatioEdge", &PUTSRecognizer::CalcAvgDefectRatioEdge);

    mapPartFunc.insert("MaxRatio", &PUTSRecognizer::CalcMaxDefectRatio);
    mapPartFunc.insert("MaxRatioAll", &PUTSRecognizer::CalcMaxDefectRatioAll);
    mapPartFunc.insert("MaxRatioBody", &PUTSRecognizer::CalcMaxDefectRatioBody);
    mapPartFunc.insert("MaxRatioEdge", &PUTSRecognizer::CalcMaxDefectRatioEdge);

    mapPartFunc.insert("DefectMaxs", &PUTSRecognizer::CalcDefectMaxs);

    mapPartFunc.insert("MaxIndiLengthAll", &PUTSRecognizer::CalcMaxIndiLengthAll);
    mapPartFunc.insert("MaxIndiLengthBody", &PUTSRecognizer::CalcMaxIndiLengthBody);
    mapPartFunc.insert("MaxIndiLengthEdge", &PUTSRecognizer::CalcMaxIndiLengthEdge);

    mapPartFunc.insert("MaxAreaAll", &PUTSRecognizer::CalcMaxAreaAll);
    mapPartFunc.insert("MaxAreaBody", &PUTSRecognizer::CalcMaxAreaBody);
    mapPartFunc.insert("MaxAreaEdge", &PUTSRecognizer::CalcMaxAreaEdge);

    mapPartFunc.insert("MaxLengthAll", &PUTSRecognizer::CalcMaxLengthAll);
    mapPartFunc.insert("MaxLengthBody", &PUTSRecognizer::CalcMaxLengthBody);
    mapPartFunc.insert("MaxLengthEdge", &PUTSRecognizer::CalcMaxLengthEdge);

    mapPartFunc.insert("MaxWidthAll", &PUTSRecognizer::CalcMaxWidthAll);
    mapPartFunc.insert("MaxWidthBody", &PUTSRecognizer::CalcMaxWidthBody);
    mapPartFunc.insert("MaxWidthEdge", &PUTSRecognizer::CalcMaxWidthEdge);

    mapPartFunc.insert("PerMeterMaxNumEdge", &PUTSRecognizer::CalcMaxPerMeterNumEdge);
    mapPartFunc.insert("PerSquareMeterMaxNumBody", &PUTSRecognizer::CalcMaxDensityNumBody);
    mapPartFunc.insert("PerMeterMaxNumBody", &PUTSRecognizer::CalcMaxPerMeterNumBody);      //本体长度最大每米缺陷数
    mapPartFunc.insert("PerMeterAvgNumBody", &PUTSRecognizer::CalcAvgPerMeterNumBody);
    mapPartFunc.insert("PerMeterMaxRatioBody", &PUTSRecognizer::CalcMaxPerMeterRatioBody);  //本体每米长度最大占积比
    mapPartFunc.insert("PerMeterAvgRatioBody", &PUTSRecognizer::CalcAvgPerMeterRatioBody);

    mapPartFunc.insert("Result", &PUTSRecognizer::CalcPartResult);
    mapPartFunc.insert("ResultBody", &PUTSRecognizer::CalcPartResultBody);
    mapPartFunc.insert("ResultEdge", &PUTSRecognizer::CalcPartResultEdge);

    mapPartFunc.insert("AreaSum", &PUTSRecognizer::CalcAreaSum);
    mapPartFunc.insert("Circlecombination", &PUTSRecognizer::CalcCirclecombination);

}

/*!
 * \brief 计算单个工件缺陷属性
 * \param strPartProperty 属性名称
 */
void PUTSRecognizer::CalcSinglePartProperty(QString strPartProperty)
{
    if ( mapPartProperty.find(strPartProperty) != mapPartProperty.end() )
    {
        if (!mapPartProperty[strPartProperty])
        {
            (this->*mapPartFunc[strPartProperty])();
            mapPartProperty[strPartProperty] = true;
        }
    }
}

/*!
 * \brief 计算所有工件缺陷属性
 */
void PUTSRecognizer::CalcPartProperty()
{
    foreach ( QString strPartFunc, mapPartFunc.keys() )
    {
        foreach ( QString strPartProperty, mapPartProperty.keys() )
        {
            if (strPartProperty == strPartFunc)
            {
                CalcSinglePartProperty(strPartProperty);
            }
        }
    }
}

/*!
 * \brief 缺陷评定
 */
void PUTSRecognizer::DefectAssessment(const QRectF &centerZone)
{
    clock_t ctorStart = clock();
    /* 8.2 在钢板周边 50 mm(板厚大于 100 mm 时，取板厚的一半)可检验区域内及坡口预定线两侧各 25 mm 内，单个缺陷的指示长度不得大于或等于 50 mm. */
    //CDataValue edgeMaxLength = options.value("EdgeMaxLength").toDouble(50);
    /* 7.4.1 单个缺陷评定 */
    //CDataValue maxLength = options.value("MaxAllowedLength").toDouble(80);
    //CDataValue maxArea = options.value("MaxAllowedArea").toDouble(2500);
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

        if ( !centerZone.contains( defect.boundaryF.boundingRect() ) ) {          /* 不在中央区域，则使用边缘判伤标准 */
            defect.type = DEFECT_EDGE;
            //defect.rank = qMax(defect.length / edgeMaxLength, defect.area / maxArea);
        } else {
            defect.type = DEFECT_NORMAL;
            // defect.rank = qMax(defect.length / maxLength, defect.area / maxArea);
        }

        SetDefectRank(defect);
        defects.append(defect);
        boundaryPolygons.append(QPair<QPolygonF, double>(defect.boundaryF, defect.length));
        UpdateDefect(index++);
    }

    qDebug() << GetStandardName() << "single defect took time" << clock() - ctorStart << "ms defect num" <<  defects.count();

    /*QVector<QPolygonF> boundaryPolygonsTemp;
    int size = boundaryPolygons.size();
    for (int i = 0; i < size; ++i)
    {
        boundaryPolygonsTemp.append(boundaryPolygons[i].first);
    }*/
    DefectFilter();
    DefectCombine(centerZone, boundaryPolygons);
}

void PUTSRecognizer::DefectCombine(const QRectF &centerZone, QVector<QPolygonF> boundaryPolygons)
{
    /* 7.4.2 当多个缺陷的相邻间距小于 100 mm 或间距小于相邻缺陷(以指示长度来比较)的指示长度(取
     *  其较大值)时，其各块缺陷面积之和也作为单个缺陷指示面积。 */
    clock_t ctorStart = clock();
    double minDefectDistance = options.value("MinDefectDistance").toDouble(100);
    if (minDefectDistance > 0)
    {
        /* 使用 GroupPolygons 将缺陷按距离分组，得到各缺陷的分组标签 */
        int groupCount = 0;                                                     /*!< 分组结果中的总分组数 */
        const QVector<int> groupLabels = GroupPolygons(boundaryPolygons, minDefectDistance, groupCount);
        Q_ASSERT( groupLabels.count() == defects.count() );

        /* 新增组合缺陷 */
        defects.resize(defects.size() + groupCount);
        /* 遍历单缺陷列表，按标签将其特征合并入组合缺陷 */
        for (int index = 0; index < groupLabels.count(); ++index)
        {
            Q_ASSERT(groupLabels[index] < groupCount);
            Defect &defect = defects[index];                                    /*!< 当前单缺陷的引用 */
            int groupLabel = groupLabels[index];
            Defect &combined = defects[groupLabels.count() + groupLabel];       /*!< 当前单缺陷对应的组合缺陷的引用 */
            combined.area += defect.area;
            combined.length = qMax(combined.length, defect.length);
            combined.rectLength = qMax(combined.rectLength, defect.rectLength);
            combined.rectWidth = qMax(combined.rectWidth, defect.rectWidth);
            combined.boundaryF = combined.boundaryF.boundingRect().united( defect.boundaryF.boundingRect() );
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
    }

    qDebug() << GetStandardName() << "combine defect took time" << clock() - ctorStart << "ms defect num" <<  defects.count();
}

void PUTSRecognizer::DefectCombine(const QRectF &centerZone, QVector<QPair<QPolygonF, double>> boundaryPolygons)
{
    /* 7.4.2 当多个缺陷的相邻间距小于 100 mm 或间距小于相邻缺陷(以指示长度来比较)的指示长度(取
     *  其较大值)时，其各块缺陷面积之和也作为单个缺陷指示面积。 */
    clock_t ctorStart = clock();
    double minDefectDistance = options.value("MinDefectDistance").toDouble(100);
    if (minDefectDistance > 0)
    {
        /* 使用 GroupPolygons 将缺陷按距离分组，得到各缺陷的分组标签 */
        int groupCount = 0;                                                     /*!< 分组结果中的总分组数 */
        const QVector<int> groupLabels = GroupPolygons(boundaryPolygons, minDefectDistance,
                                                       static_cast<double>(1), groupCount, combineMax);
        Q_ASSERT( groupLabels.count() == defects.count() );

        /* 新增组合缺陷 */
        defects.resize( defects.size() + groupLabels.count() );
        /* 遍历单缺陷列表，按标签将其特征合并入组合缺陷 */
        for (int index = 0; index < groupLabels.count(); ++index)
        {
            Q_ASSERT(groupLabels[index] < groupCount);
            Defect &defect = defects[index];                                    /*!< 当前单缺陷的引用 */
            int groupLabel = groupLabels[index];
            Defect &combined = defects[groupLabels.count() + groupLabel];       /*!< 当前单缺陷对应的组合缺陷的引用 */
            combined.area += defect.area;
            combined.length = qMax(combined.length, defect.length);
            combined.rectLength = qMax(combined.rectLength, defect.rectLength);
            combined.rectWidth = qMax(combined.rectWidth, defect.rectWidth);
            combined.boundaryF = combined.boundaryF.boundingRect().united( defect.boundaryF.boundingRect() );
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
    }

    qDebug() << GetStandardName() << "combine defect took time" << clock() - ctorStart << "ms defect num" <<  defects.count();

}

void PUTSRecognizer::SetDefectRank(Defect &defect)
{
    double edgeMaxLength = options.value("EdgeMaxLength").toDouble(50);
    double maxArea = options.value("MaxAllowedArea").toDouble(2500);
    double maxLength = options.value("MaxAllowedLength").toDouble(80);

    if (defect.type == DEFECT_NORMAL)
    {
        defect.rank = qMax(defect.length / maxLength, defect.area / maxArea);
    }
    else if (defect.type == DEFECT_EDGE)
    {
        defect.rank = qMax(defect.length / edgeMaxLength, defect.area / maxArea);
    }
    else if (defect.type == DEFECT_COMBINED_BODY)
    {
        defect.rank = qMax(defect.length / maxLength, defect.area / maxArea);
    }
    else if (defect.type == DEFECT_COMBINED_EDGE)
    {
        defect.rank = qMax(defect.length / edgeMaxLength, defect.area / maxArea);
    }
    else
    {
        ;//
    }

}

void PUTSRecognizer::DefectFilter(const QRectF &rect, QVector<QPolygonF> boundaryPolygons)
{
    Q_UNUSED(rect);
    double minDefectDistance = options.value("DiffferenceY1").toDouble();
    qreal pointStartX1 = options.value("PointStartX1").toDouble();
    qreal pointStartY1 = options.value("PointStartY1").toDouble();
    qreal pointEndX1 = options.value("PointEndX1").toDouble();
    qreal pointEndY1 = options.value("PointEndY1").toDouble();
    QRectF filterRect(QRectF(QPointF(pointStartX1, pointStartY1),
                             QPointF(pointEndX1, pointEndY1)));
    bool isFilter = options.value("IsFilter").toBool();
    if(isFilter)
    {
        if (!qFuzzyIsNull(minDefectDistance))
        {
            /* 使用 GroupPolygons 将缺陷按距离分组，得到各缺陷的分组标签 */
            int groupCount = 0;                                                     /*!< 分组结果中的总分组数 */
            const QVector<int> groupLabels = GroupPolygons(boundaryPolygons, minDefectDistance, groupCount);
            //const QVector<int> groupLabels = GroupPolygons(boundaryPolygons, minDefectDistance,
            //                                               static_cast<double>(1), groupCount, false);
            Q_ASSERT( groupLabels.count() == defects.count() );

            QList<Defect* > listDefect;
            QVector<QList<Defect* >> vecDefect;
            vecDefect.resize(groupCount);
            /* 遍历单缺陷列表，按标签将其特征合并入组合缺陷 */
            for (int index = 0; index < groupLabels.count(); ++index)
            {
                Q_ASSERT(groupLabels[index] < groupCount);
                //Defect &defect = defects[index];                                    /*!< 当前单缺陷的引用 */
                int groupLabel = groupLabels[index];
                //Defect &combined = defects[groupLabel];       /*!< 当前单缺陷对应的组合缺陷的引用 */
                vecDefect[groupLabel].append(&defects[index]);
            }

            foreach (QList<Defect* > list, vecDefect)
            {
                int listSize = list.size();
                for(int j = 0; j < listSize - 1; ++j)
                {
                    if(filterRect.contains(list.value(j)->boundaryF.boundingRect())
                            && filterRect.contains(list.value(j + 1)->boundaryF.boundingRect()))
                    {
                        if(list.value(j + 1)->area < list.value(j)->area)
                        {
                            list[j + 1]->area = list.value(j)->area;
                            list[j + 1]->length = list.value(j)->length;
                            list[j + 1]->rectLength = list.value(j)->rectLength;
                            list[j + 1]->rectWidth = list.value(j)->rectWidth;
                            list[j + 1]->rank = list.value(j)->rank;
                            list[j + 1]->type = list.value(j)->type;
                            list[j + 1]->minInterval = list.value(j)->minInterval;
                            list[j + 1]->radius = list.value(j)->radius;
                            list[j + 1]->isValid = list.value(j)->isValid;
                            list[j + 1]->defectAmp = list.value(j)->defectAmp;
                            list[j + 1]->defectDepth = list.value(j)->defectDepth;
                            list[j + 1]->bottomAmp = list.value(j)->bottomAmp;
                            list[j + 1]->bottomDepth = list.value(j)->bottomDepth;
                            list[j + 1]->boundaryF.clear();
                            foreach (QPointF point, list.value(j)->boundaryF)
                            {
                                list[j + 1]->boundaryF.append(point);
                            }
                            list[j + 1]->boundary.clear();
                            foreach (QPoint point, list.value(j)->boundary)
                            {
                                list[j + 1]->boundary.append(point);
                            }
                        }

                        list[j]->area = 0;
                        list[j]->length = 0;
                        list[j]->rectLength = 0;
                        list[j]->rectWidth = 0;
                        list[j]->boundaryF.clear();
                        list[j]->boundary.clear();
                    }
                }
            }
        }
    }
}

void PUTSRecognizer::DefectFilter()
{
    bool isFilter = options.value("IsFilter").toBool();
    if(isFilter)
    {
        qreal pointStartX1 = options.value("PointStartX1").toDouble();
        qreal pointStartY1 = options.value("PointStartY1").toDouble();
        qreal pointEndX1 = options.value("PointEndX1").toDouble();
        qreal pointEndY1 = options.value("PointEndY1").toDouble();
        DefectFilterInArea(QRectF(QPointF(pointStartX1, pointStartY1),
                                  QPointF(pointEndX1, pointEndY1)), 0);

        qreal pointStartX2 = options.value("PointStartX2").toDouble();
        qreal pointStartY2 = options.value("PointStartY2").toDouble();
        qreal pointEndX2 = options.value("PointEndX2").toDouble();
        qreal pointEndY2 = options.value("PointEndY2").toDouble();
        DefectFilterInArea(QRectF(QPointF(pointStartX2, pointStartY2),
                                  QPointF(pointEndX2, pointEndY2)), 1);
        //DefectFilterInArea(QRectF(350, 0, 100, 2800));
        //DefectFilterInArea(QRectF(650, 0, 100, 2800));
    }
}

void PUTSRecognizer::DefectFilterInArea(const QRectF &filterRect, int rectIdx)
{
    QList<Defect* > listDefect;
    /*foreach (Defect defect, defects)
    {
        if (filterRect.contains(defect.boundaryF.boundingRect()))
        {
            listDefect.append(&defect);
        }
    }*/
    int size = defects.size();
    for (int i = 0; i < size; ++i)
    {
        if (filterRect.contains(defects.value(i).boundaryF.boundingRect()))
        {
            listDefect.append(&defects[i]);
        }
    }

    double differenceX = 0.0f;
    double differenceY = 0.0f;
    if(rectIdx == 0)
    {
        differenceX = GetFilterDiffValue("IsCompareX1", "DifferenceX1");
        differenceY = GetFilterDiffValue("IsCompareY1", "DifferenceY1");
    }
    else
    {
        differenceX = GetFilterDiffValue("IsCompareX2", "DifferenceX2");
        differenceY = GetFilterDiffValue("IsCompareY2", "DifferenceY2");
    }

    CompareDefectAndFilter(listDefect, 0, 1, differenceX, differenceY);
}

double PUTSRecognizer::GetFilterDiffValue(QString compare, QString value)
{
    bool isCompare = options.value(compare).toBool();
    if(isCompare)
    {
        return options.value(value).toDouble();
    }

    return 0;
}

void PUTSRecognizer::ClearDefect(Defect* defect)
{
    defect->area = 0;
    defect->length = 0;
    defect->rectLength = 0;
    defect->rectWidth = 0;
    defect->boundaryF.clear();
    defect->boundary.clear();
}

bool PUTSRecognizer::CompareDefectAndFilterByX(QList<Defect* > list, int curIdx, int nexIdx, double difference)
{
    if(!qFuzzyIsNull(difference))
    {
        int size = list.size();
        if(curIdx < size && nexIdx < size)
        {
            QRectF rectFirst = list.value(curIdx)->boundaryF.boundingRect();
            QRectF rectSecond = list.value(nexIdx)->boundaryF.boundingRect();

            if(qAbs(rectFirst.bottomRight().rx() - rectSecond.bottomRight().rx()) >= difference)
            {
                ClearDefect(list[nexIdx]);
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}

bool PUTSRecognizer::CompareDefectAndFilterByY(QList<Defect* > list, int curIdx, int nexIdx, double difference)
{
    if(!qFuzzyIsNull(difference))
    {
        int size = list.size();
        if(curIdx < size && nexIdx < size)
        {
            QRectF rectFirst = list.value(curIdx)->boundaryF.boundingRect();
            QRectF rectSecond = list.value(nexIdx)->boundaryF.boundingRect();

            if (qAbs(rectFirst.topLeft().ry() - rectSecond.bottomRight().ry()) < difference)
            {
                ClearDefect(list[nexIdx]);
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}

void PUTSRecognizer::CompareDefectAndFilter(QList<Defect* > list, int curIdx, int nexIdx,
                                            double differencX, double differenceY)
{
    int size = list.size();
    if(curIdx < size && nexIdx < size)
    {
        if(!qFuzzyIsNull(differenceY))
        {
            if(CompareDefectAndFilterByY(list, curIdx, nexIdx, differenceY))
            {
                CompareDefectAndFilter(list, curIdx, nexIdx + 1, differencX, differenceY);
            }
            else
            {
                if(CompareDefectAndFilterByX(list, curIdx, nexIdx, differencX))
                {
                    CompareDefectAndFilter(list, curIdx, nexIdx + 1, differencX, differenceY);
                }
                else
                {
                    CompareDefectAndFilter(list, nexIdx, nexIdx + 1, differencX, differenceY);
                }
            }

        }
        else
        {
            if(CompareDefectAndFilterByX(list, curIdx, nexIdx, differencX))
            {
                CompareDefectAndFilter(list, curIdx, nexIdx + 1, differencX, differenceY);
            }
            else
            {
                CompareDefectAndFilter(list, nexIdx, nexIdx + 1, differencX, differenceY);
            }
        }
    }

    return;
}
