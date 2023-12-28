#pragma once

#include <QMetaType>
#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
#    include <Ultrasonic/UTData.h>
#endif

#include <Imaging/ImageOptions.h>
#include "UTImagePalette.h"
#include "UTDefect.h"
#include "UTDefectProcessorOptions.h"

class UTImage;
class UTImagePrivate;

typedef QVector<UTImage> UTImageList;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#    include "UTImagePrivate.h"
#endif

typedef QSharedPointer<UTImagePrivate> UTSharedImagePrivate;

class UTImage : protected UTSharedImagePrivate
{
public:
    typedef QPair<QRect, double> FillInArea;
    typedef QVector<FillInArea> FillInAreas;
    typedef QPair<QPolygon, QVector<QPolygon> > Boundary;
    typedef QVector<Boundary> Boundaries;
    enum ThresholdMode
    {
        THRESHOLD_ABOVE = 1,
        THRESHOLD_BELOW = -1
    };

    enum CompareMode { CMP_EQ = 0, CMP_GT = 1, CMP_GE = 2, CMP_LT = 3, CMP_LE = 4, CMP_NE = 5 };

    UTImage(const QSize &size = QSize(), CDataValue fill = 0);
    UTImage(int width, int height, CDataValue fill = 0);
    UTImage(UTImagePrivate *image);

    bool IsValid() const;

    Q_REQUIRED_RESULT UTImage Copy() const;                                     /*!< 深度复制 */
    Q_REQUIRED_RESULT UTImage Copy(const QRect &rect) const;

    void CopyTo(UTImage target, const UTImage *mask = nullptr) const;
    Q_REQUIRED_RESULT UTImage CreateSameSize(CDataValue fill = 0) const;

    Q_REQUIRED_RESULT QSize GetSize() const;                                    /*!< 获取图像尺寸 */
    Q_REQUIRED_RESULT QRect GetRect() const;                                    /*!< 获取图像区域，返回一个左上角在(0, 0)，尺寸与图像相同的 QRect */
    Q_REQUIRED_RESULT int GetWidth() const;                                     /*!< 获取图像宽度 */
    Q_REQUIRED_RESULT int GetHeight() const;                                    /*!< 获取图像高度 */
    Q_REQUIRED_RESULT int GetPixelCount( const QRect &ROI = QRect() ) const;    /*!< 获取图像面积，即总像素数 */
    Q_REQUIRED_RESULT QRect BoundRect(const QRect &rect) const;                 /*!< 将一个 Rect 限制在图像范围 */
    Q_REQUIRED_RESULT bool Contains(int x, int y) const;                        /*!< 检测目标坐标是否在图像内 */
    Q_REQUIRED_RESULT inline bool Contains(const QPoint &point) const {return Contains( point.x(), point.y() );}

    void SetPoint(int x, int y, CDataValue v);
    void SetPointFast(int x, int y, CDataValue v);
#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
    void SetPoint(UTDataFrame frame, const ImageOptions &trans, int index, int xIndex, int yIndex);
    void SetPoints(const UTDataSegment &segment, const ImageOptions &trans, int index, int xIndex, int yIndex);
    void SetPointFast(UTDataFrame frame, const ImageOptions &trans, int index, int xIndex, int yIndex);
    void SetPointsFast(const UTDataSegment &segment, const ImageOptions &trans, int index, int xIndex, int yIndex);
#endif

    Q_REQUIRED_RESULT QImage ToImage(const UTImagePalette &palette, const UTImage *mask = nullptr) const;
    Q_REQUIRED_RESULT QImage ToIndexImage(CDataValue min, CDataValue max, const UTImage *mask = nullptr) const;
    Q_REQUIRED_RESULT QPixmap ToPixmap(const UTImagePalette &palette, const UTImage *mask = nullptr) const;

    void GetMinMax( CDataValue *min, CDataValue *max, QPoint *minLoc, QPoint *maxLoc, const UTImage *mask = nullptr, const QRect &ROI = QRect() ) const;
    Q_REQUIRED_RESULT int CountNonZero( const UTImage *mask = nullptr, const QRect &ROI = QRect() ) const;
    Q_REQUIRED_RESULT double CalcDensity( const UTImage *mask = nullptr, const QRect &ROI = QRect() );   /*!< 计算非零像素占比 */
    Q_REQUIRED_RESULT CDataValue GetSum() const;
    Q_REQUIRED_RESULT CDataValue GetMean( const UTImage *mask = nullptr, const QRect &ROI = QRect() ) const;
    void meanStdDev(CDataValue *mean, CDataValue *stddev, const UTImage *mask, const QRect &ROI = QRect()) const;

    Q_REQUIRED_RESULT FillInAreas CheckBlockDensity( CDataValue threshold, const QSize &block, const QSize &step = QSize(1, 1) ) const;
    Q_REQUIRED_RESULT QPair<double, QRect> FindMaxDensity( const QSize &block, const QSize &step = QSize(1, 1), const QRect &ROI = QRect() ) const;
    Q_REQUIRED_RESULT Boundaries FindBoundaries(CDataValue threshold, ThresholdMode mode, const QRect &ROI = QRect(), const UTImage *mask = nullptr) const;
    Q_REQUIRED_RESULT QVector<int> CalcHistogram(int resolution, CDataValue min, CDataValue max, const QRect &ROI = QRect(), const UTImage *mask = nullptr) const;

    UTImage &Dilate(int pixels);
    UTImage &Erode(int pixels);
    UTImage &DilateDiscrete();

    UTImage &DrawEllipse(const QPoint &center, const QSize &size, double angle, double startAngle = 0, double endAngle = 360, CDataValue fill = 1);
    UTImage &DrawPolygon(const QPolygon &polygon, CDataValue fill = 1);

    UTImage &Add(const UTImage &other);
    UTImage &Add(CDataValue value);
    UTImage &Subtract(const UTImage &other);
    UTImage &Subtract(CDataValue value);
    UTImage &Multiply(const UTImage &other);
    UTImage &Multiply(CDataValue value);
    UTImage &Divide(const UTImage &other);
    UTImage &DividedBy(CDataValue value);
    UTImage &Abs();

    Q_REQUIRED_RESULT UTImage Compare(const UTImage &other, CompareMode mode) const;
    Q_REQUIRED_RESULT UTImage Compare(CDataValue value, CompareMode mode) const;
    Q_REQUIRED_RESULT UTImage CompareRange(CDataValue min, CDataValue max, bool minAllowEq = false, bool maxAllowEq = false) const;

    Q_REQUIRED_RESULT UTImage And(const UTImage &other) const;
    Q_REQUIRED_RESULT UTImage Or(const UTImage &other) const;

    Q_REQUIRED_RESULT UTDefectList FindDefects(const UTImageList &images, const UTDefectProcessorOptions &options) const;

    void DebugPrint(const QString &name) const;

    CDataValue &operator ()(int x, int y);
    const CDataValue &operator ()(int x, int y) const;
    CDataValue &operator ()(const QPoint &point);
    const CDataValue &operator ()(const QPoint &point) const;

    UTImagePrivate *GetImage() const {return data();}

    Q_REQUIRED_RESULT static UTImage FromAData(const UTDataSegment &segment, Qt::Orientation direction, int offset = 0, bool relativeOffset = false);

protected:
    friend QDebug operator <<(QDebug debug, const UTImage &image);
};

Q_DECLARE_METATYPE(UTImage)
Q_DECLARE_METATYPE(UTImageList)

#ifndef QT_NO_DEBUG_STREAM
QDebug operator <<(QDebug debug, const UTImage &image);
#endif

/*! 物体结构分析 */
float FindEnclosingCircle(const QPolygonF &points, QPointF *center);
//static QPolygonF FindEnclosingTriangle(const QPolygon &points);
QPolygonF FindEnclosingRect(const QPolygonF &polygon);
double PolygonArea(const QPolygonF &polygon);
QPolygon ApproximateCircle(const QPoint &center, int radius);
//static double PolygonLength(const QVector<QPointF> &curve);
//static QPolygon FindConvexHull(const QPolygon &polygon);

double PolygonDistance(QPolygonF p1, QPolygonF p2);
QVector<int> GroupPolygons(const QVector<QPolygonF> &polygon, double minDistance, int &labelCount);
QVector<int> GroupPolygons(const QVector<QPair<QPolygonF, double> > &polygon, double minDistance, double minEPS, int &labelCount, bool epsWithMax = true);
QPair<int, QRect> FindMaxObjectDensity(const QVector<QRect> &objects, const QSize &block, const QRect &ROI = QRect());

template<typename Iterator>
QRect UniteRects(Iterator begin, Iterator end)
{
    QRect united;
    for (Iterator it = begin; it != end; ++it)
    {
        united.united(*it);
    }
    return united;
}

template<typename Container>
QRect UniteRects(Container rects)
{
    return UniteRects<Container::ConstIterator>(rects.constBegin(), rects.constEnd());
}
