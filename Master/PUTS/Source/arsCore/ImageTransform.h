#ifndef IMAGETRANSFORM_H
#define IMAGETRANSFORM_H

#include <QRectF>
#include <QSizeF>
#include <QTransform>
#include <QJsonObject>
#include <QMetaType>
#include <QDebug>

/*!
 * \brief ImageTransform 是一个源区域到目标区域的坐标变换。
 * 源区域为一个QRectF，原点不定，坐标为浮点；目标区域以(0, 0)为原点，坐标为整数。
 *
 * 除了默认构造体和复制构造体， ImageTransform 有3种构造方式：
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
class ImageTransform
{
public:
    ImageTransform();
    ImageTransform(const ImageTransform &other);
    explicit ImageTransform(const QRectF &sourceRect, const QSize targetRect);
    explicit ImageTransform(const QRectF &sourceRect, const QSizeF gridSize);
    explicit ImageTransform(const QSize targetRect, const QSizeF gridSize, const QPointF &sourceOffset);

    const QRectF &SourceRect() const;
    const QSize &TargetRect() const;
    const QSizeF &GridSize() const;
    float GridArea() const;
    const QTransform &ForwardTransform() const;
    const QTransform &InverseTransform() const;
    bool IsValid() const;

    QPointF MapToSource(const QPointF &point) const;

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

    bool operator ==(const ImageTransform &other) const;
    bool operator !=(const ImageTransform &other) const;

    QJsonObject ToJson() const;
    static ImageTransform FromJson(const QJsonObject &obj);

    enum CyclularType {
        CYCLULAR_NONE,
        CYCLULAR_THETA_R
    };
    CyclularType GetCyclularType() const {return cyclularType;}
    float GetCuclularCenter() const {return cyclularCenter;}
    void SetCyclularType(CyclularType newType, float center);

    void SetScale(const QSizeF &size) {scale = size;}
    QSizeF GetScale() const {return scale;}

private:
    void UpdateTransforms();

    QRectF source;
    QSize target;
    QSizeF grid;
    QTransform forward;
    QTransform inverse;
    CyclularType cyclularType;
    float cyclularCenter;
    QPointF sourceOffset;

    QSizeF scale;
};

Q_DECLARE_METATYPE(ImageTransform)
Q_DECLARE_TYPEINFO(ImageTransform, Q_MOVABLE_TYPE);

#ifndef QT_NO_DEBUG_STREAM
/*Q_CORE_EXPORT*/ QDebug operator <<(QDebug debug, const ImageTransform &transform);
#endif

#endif                                                                          // IMAGETRANSFORM_H
