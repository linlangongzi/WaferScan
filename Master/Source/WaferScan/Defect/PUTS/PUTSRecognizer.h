#pragma once

#include "../DefectRecognizer.h"
#include "ClassFactory.h"

/*!
 * \class PUTSRecognizer
 * \brief 缺陷识别器的基类。
 */
class PUTSRecognizer : public DefectRecognizer
{
private:
    DECLARE_RUNTIME(PUTSRecognizer);
public:
    enum DefectType
    {
        DEFECT_NORMAL = 0,
        DEFECT_EDGE = 1,
        DEFECT_COMBINED_BODY = 2,
        DEFECT_COMBINED_EDGE = 3,
        DEFECT_DENSITY = 4,
        DEFECT_LENGTH_NUM = 5,
        DEFECT_DENSITY_NUM = 6,
        DEFECT_DENSITY_BODY = 7,
        DEFECT_DENSITY_EDGE = 8,
        DEFECT_LENGTH_NUM_BODY = 9,    //本体长度最大每米缺陷数
        DEFECT_CITCLE_COMBINED = 10,   //圆内范围组合伤
        DEFECT_LENGTH_RATIO_BODY = 11,   //本体长度最大每米占机率

    };

public:
    PUTSRecognizer(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);

    virtual QString GetStandardName() const {return "PUTS";}

    virtual QStringList GetPartKeys() const;
    virtual QString GetPartKeyDescription(const QString &key) const;
    virtual CDataValue GetPartValue(const QString &key) const;

    virtual QStringList GetObjectKeys() const;
    virtual QString GetObjectKeyDescription(const QString &key) const;
    virtual int GetNumObjects() const;
    virtual QPolygonF GetObjectBoundary(int index) const;
    virtual CDataValue GetObjectValue(int index, const QString &key) const;

    /*! 按照名称构造缺陷识别器，返回缺陷识别器的指针。
     *  建议使用 QScopedPointer 存放该指针，以确保识别器能被正常析构 */
    static PUTSRecognizer *Create(const QString &name, const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options);
    virtual void CalcDefect();
    static void* CreateInstance(const ImageOptions &transform, const UTImageList &imageList, const QJsonObject &options)
    {
        return new PUTSRecognizer(transform, imageList, options);
    }

protected:
    typedef QPair<QString, ptrdiff_t> ObjectValueInfo;
    typedef QMap<QString, ObjectValueInfo> ObjectValueMap;
    struct Statistics
    {
        CDataValue max;
        CDataValue min;
        CDataValue avg;
        CDataValue minLocX;
        CDataValue minLocY;
        CDataValue maxLocX;
        CDataValue maxLocY;
        Statistics() : max(0), min(0), avg(0), minLocX(0), minLocY(0), maxLocX(0), maxLocY(0) {}
    };

    struct Defect
    {
        QPolygon boundary;
        QPolygonF boundaryF;
        CDataValue length;
        CDataValue rectWidth;
        CDataValue rectLength;
        CDataValue area;
        CDataValue rank;
        CDataValue type;
        CDataValue minInterval;
        CDataValue radius;
        bool isValid;
        Statistics defectAmp;
        Statistics defectDepth;
        Statistics bottomAmp;
        Statistics bottomDepth;
        Defect() : length(0), rectWidth(0), rectLength(0), area(0), rank(0), type(DEFECT_NORMAL), isValid(false) {}
    };
    typedef QVector<Defect> DefectList;
    typedef QPair<QString, double> PairPartInfo;
//    typedef QPair<bool, double> PairPartValue;
    typedef QMap<QString, bool> MapPartProperty;
    typedef QMap<QString, PairPartInfo> MapPart;
    typedef void (PUTSRecognizer::*CALC_FUN)(void);
    typedef QMap<QString, CALC_FUN> MapPartFunc;

protected:
    int defectAmpIndex;
    int defectDepthIndex;
    int bottomAmpIndex;
    int bottomDepthIndex;
    mutable DefectList defects;
    UTImage defectImage;                                                        /*!< 按标准判伤后的缺陷图像 */
    UTImage borderMask;                                                         /*!< 钢板边缘的掩模图像 */
    static ObjectValueMap valueMap;
    MapPart mapPart;
    QRect rectBody;                                                             /*本体部分矩形*/
    QRect rectEdgeL;                                                            /*边沿左边部分矩形*/
    QRect rectEdgeB;                                                            /*边沿底部部分矩形*/
    QRect rectEdgeR;                                                            /*边沿右边部分矩形*/
    QRect rectEdgeT;                                                            /*边沿顶部部分矩形*/
    MapPartProperty mapPartProperty;
    QSize step;
    bool combineMax;                                                            /*!< true取相邻两个多边形的大值合并，false取小值合值*/
    QSize blockSize;                                                            /*!< 区块大小 */

protected:
    virtual bool UpdateDefect(int index) const;
    static PUTSRecognizer::ObjectValueMap InitValueMap();
    void InsertPartMap( const QString &key, PairPartInfo pairPartInfo );
    void UpdateStatistics(Statistics &statistics, const UTImage &source, const UTImage &mask, const QRect &ROI) const;
    void CalcAvgDefectRatio();
    void CalcMaxDefectRatio();
    virtual void CalcAvgDefectRatioAll();
    virtual void CalcAvgDefectRatioBody();
    virtual void CalcAvgDefectRatioEdge();
    virtual void CalcMaxDefectRatioAll();
    virtual void CalcMaxDefectRatioBody();
    virtual void CalcMaxDefectRatioEdge();
    virtual void CalcDefectMaxs();
    virtual void CalcMaxIndiLengthAll();
    virtual void CalcMaxIndiLengthBody();
    virtual void CalcMaxIndiLengthEdge();
    virtual void CalcMaxAreaAll();
    virtual void CalcMaxAreaBody();
    virtual void CalcMaxAreaEdge();
    virtual void CalcMaxLengthAll();
    virtual void CalcMaxLengthBody();
    virtual void CalcMaxLengthEdge();
    virtual void CalcMaxWidthAll();
    virtual void CalcMaxWidthBody();
    virtual void CalcMaxWidthEdge();
    virtual void CalcMaxPerMeterNumEdge();
    virtual void CalcMaxDensityNumBody();
    virtual void CalcMaxPerMeterNumBody();
    virtual void CalcAvgPerMeterNumBody();
    virtual void CalcMaxPerMeterRatioBody();
    virtual void CalcAvgPerMeterRatioBody();
    virtual void CalcPartResult();
    virtual void CalcPartResultBody();
    virtual void CalcPartResultEdge();
    virtual void CalcAreaSum();
    virtual void CalcCirclecombination();

    virtual void GenerateDefectImage();
    virtual void SetPartProperty();
    virtual void DefectAssessment( const QRectF &centerZone);
    virtual void DefectCombine(const QRectF &centerZone, QVector<QPolygonF> boundaryPolygons);
    virtual void DefectCombine(const QRectF &centerZone, QVector<QPair<QPolygonF, double>> boundaryPolygons);
    virtual void SetDefectRank(Defect &defect);
    void DefectFilter(const QRectF &rect, QVector<QPolygonF> boundaryPolygons);
    void DefectFilter();
    double GetFilterDiffValue(QString compare, QString value);
    void ClearDefect(Defect* defect);
    void DefectFilterInArea(const QRectF &filterRect, int rectIdx);
    void CompareDefectAndFilter(QList<Defect* > list, int curIdx, int nexIdx, double differencX, double differenceY);
    bool CompareDefectAndFilterByX(QList<Defect* > list, int curIdx, int nexIdx, double difference);
    bool CompareDefectAndFilterByY(QList<Defect* > list, int curIdx, int nexIdx, double difference);

private:
    MapPartFunc mapPartFunc;

private:
    void InitPartFuncMap();
    void CalcSinglePartProperty(QString strPartProperty);
    void CalcPartProperty();
};
