#pragma once

#include <Ultrasonic/UTData.h>
#include "Motion/MotionAxisList.h"
#include <QDebug>
#include <QMetaType>
#include <QRectF>
#include <QSizeF>
#include <QTransform>
#include <QSharedDataPointer>

/*!
 * \brief ImageOptions 是一个源区域到目标区域的坐标变换。
 * 源区域为一个QRectF，原点不定，坐标为浮点；目标区域以(0, 0)为原点，坐标为整数。
 *
 * 除了默认构造体和复制构造体， ImageOptions 有3种构造方式：
 *  指定源区域和目标区域，网格尺寸由此计算而来。
 *  指定源区域和网格尺寸，目标区域由此计算而来。
 *  指定网格尺寸和目标区域，源区域由此计算而来。
 *
 * 可以使用MapToGrid将PointF或PolygonF从源区域映射到目标区域
 * MapFromGrid为类似的逆向映射
 *
 * 更高级的变换可以使用变换矩阵：
 * 使用ForwardTransform()获取正向变换矩阵，用于将源区域的坐标变换到目标区域
 * 使用InverseTransform()获取逆向变换矩阵，用于将目标区域的坐标变换到源区域
 */
class ImageOptions
{
    Q_GADGET

    /* 位置和坐标变换 */
    Q_PROPERTY(QRectF sourceRect READ sourceRect)
    Q_PROPERTY(QSizeF gridSize READ gridSize)
    Q_PROPERTY(int axisX READ axisX WRITE setAxisX)
    Q_PROPERTY(int axisY READ axisY WRITE setAxisY)
    Q_PROPERTY(CoordinateType coordinateType READ coordinateType WRITE setCoordinateType)
    Q_PROPERTY(float cyclularCenter READ cyclularCenter WRITE setCyclularCenter)

    /* 数据和归并 */
    Q_PROPERTY(int featureIndex READ featureIndex WRITE setFeatureIndex)
    Q_PROPERTY(ReducePolicy reducePolicy READ reducePolicy WRITE setReducePolicy)

    /* 过滤和偏移 */
    Q_PROPERTY(DirectionFilter directionFilter READ directionFilter WRITE setDirectionFilter)
    Q_PROPERTY(QPointF backlash READ backlash WRITE setBacklash)
    Q_PROPERTY(MotionAxisList channels READ channels WRITE setChannels)
    Q_PROPERTY(QVector<QVector<float> > channelOffset READ channelOffset WRITE setChannelOffset)

    /* 显示风格 */
    Q_PROPERTY(QSizeF scale READ scale WRITE setScale)

public:
    enum CoordinateType {
        CoordNormal,
        CoordThetaR,
        CoordRobotic,
    };
    Q_ENUM(CoordinateType)

    enum ReducePolicy {
        ReduceNone,
        ReduceByMax,                                                            // 最大值
        ReduceByMin                                                             // 最小值
//        ReduceByAverage                                                         // 平均值
    };
    Q_ENUM(ReducePolicy)

    enum DirectionFilter {
        DirectionAny,
        DirectionForward,
        DirectionReverse,
    };
    Q_ENUM(DirectionFilter)

public:
    ImageOptions();
    explicit ImageOptions(const QRectF &sourceRect, const QSize targetRect);
    explicit ImageOptions(const QRectF &sourceRect, const QSizeF gridSize);
    explicit ImageOptions(const QSize targetRect, const QSizeF gridSize, const QPointF &sourceOffset);

    const QRectF &sourceRect() const;
    const QSize &targetRect() const;
    const QSizeF &gridSize() const;
    float gridArea() const;
    const QTransform &ForwardTransform() const;
    const QTransform &InverseTransform() const;

    void setTransform(const QRectF &sourceRect, const QSize targetRect);
    void setTransform(const QRectF &sourceRect, const QSizeF gridSize);
    void setTransform(const QSize targetRect, const QSizeF gridSize, const QPointF &sourceOffset);

    bool IsValid() const;

    QPointF MapToSource(const QPointF &point) const;
    QPointF MapToSource(const UTDataFrame &frame, int channel) const;

    QPoint MapToGrid(float x, float y) const;
    QPoint MapToGrid(const QPointF &sourcePoint) const;
    QSize MapToGrid(const QSizeF &sourceSize) const;
    QRect MapToGrid(const QRectF &sourceRect) const;
    QPolygon MapToGrid(const QPolygonF &sourcePolygon) const;
    QPoint MapToGridBound(const QPoint &point) const;

    QPointF MapFromGrid(const QPointF &gridPoint) const;
    QSizeF MapFromGrid(const QSize &gridSize) const;
    QRectF MapFromGrid(const QRectF &gridRect) const;
    QPolygonF MapFromGrid(const QPolygonF &gridPolygon) const;
    QPointF MapFromGridBound(const QPoint &point) const;

    bool SourceContains(const QPointF &point) const;
    bool GridContains(const QPoint &point) const;

    QPointF BoundToSource(const QPointF &point) const;
    QPoint BoundToGrid(const QPoint &point) const;

    qreal AreaToGrid(qreal sourceArea) const;
    qreal AreaFromGrid(qreal gridArea) const;

    CoordinateType coordinateType() const {return d->coordinateType;}
    void setCoordinateType(CoordinateType newType);
    float cyclularCenter() const {return d->cyclularCenter;}
    void setCyclularCenter(float center);

    int axisX() const {return d->axisX;}
    int axisY() const {return d->axisY;}
    void setAxisX(int axis) {d->axisX = axis;}
    void setAxisY(int axis) {d->axisY = axis;}

    int featureIndex() const {return d->featureIndex;}
    void setFeatureIndex(int index);
    ReducePolicy reducePolicy() const {return d->reducePolicy;}
    void setReducePolicy(ImageOptions::ReducePolicy reduce);

    DirectionFilter directionFilter() const {return d->direction;}
    void setDirectionFilter(DirectionFilter filter) {d->direction = filter;}
    bool isDirectionEnabled(bool isReversed) const;

    void setScale(const QSizeF &size) {d->scale = size;}
    QSizeF scale() const {return d->scale;}

    void setBacklash(const QPointF &size) {d->backlash = size;}
    QPointF backlash() const {return d->backlash;}

    void setChannelOffset(const QVector<QVector<float> > channelOffset) {d->offset = channelOffset;}
    QVector<QVector<float> > channelOffset() {return d->offset;}

    const MotionAxisList &channels() const {return d->channels;}
    bool isChannelEnabled(int index) const {return d->channels.hasAxis(index);}
    void setChannels(const MotionAxisList &newChannels) {d->channels = newChannels;}

    QJsonValue toJson() const;
    static ImageOptions fromJson(const QJsonValue &json);

    bool operator ==(const ImageOptions &other) const;
    bool operator !=(const ImageOptions &other) const;

private:
    void updateTransforms();
    void updateCyclular();

    struct Data : public QSharedData
    {
        QRectF source;
        QSize target;
        QSizeF grid;
        QTransform forward;
        QTransform inverse;
        CoordinateType coordinateType = CoordNormal;
        float cyclularCenter = 0;
        QPointF sourceOffset;
        int axisX = -1;
        int axisY = -1;

#ifdef PIPE_DETECT
        int featureIndex = 1;
#else
        int featureIndex = 9;
#endif

        ReducePolicy reducePolicy = ReduceNone;
        DirectionFilter direction = DirectionAny;
        QSizeF scale = QSizeF(1, 1);
        QPointF backlash;
        MotionAxisList channels;
        QVector<QVector<float> > offset;
    };
    QSharedDataPointer<Data> d;

    static const QSharedDataPointer<Data> dummy;
};

#ifndef QT_NO_DEBUG_STREAM
QDebug operator <<(QDebug debug, const ImageOptions &transform);
#endif
