#ifndef UTIMAGEPRIVATE_H
#define UTIMAGEPRIVATE_H

#include "arsCore/ImageTransform.h"
#include "UTImagePalette.h"
#include <opencv.h>
#include <QColor>

#define RENDER_OPENCV_LUT 1

using namespace cv;

/*!
 * \class UTImagePrivate
 * UTImage 的包装类，包装来自 OpenCV 的图像数据结构，避免命名空间 cv 扩散到整个系统。
 *
 * 警告: include这个头文件将导致 cv 命名空间被导入环境，这个文件永远不应该在头文件中include
 */

class UTImagePrivate
{
public:
    typedef Mat1b Image8;                                                       /*!< 8位图像缓冲区 */
    typedef Mat1i Image32;                                                      /*!< 渲染后的32位深图像缓冲区 */
    typedef Mat_<CDataValue> DataType;                                          /*!< 原始数据矩阵的数据类型 */

    /*! 默认构造体，构造空白图像 */
    UTImagePrivate(const QSize &size, CDataValue fill = 0) :
        data(size.isValid() ? Size( size.width(), size.height() ) : Size(0, 0), fill)
    {}
    /*! 复制构造体 */
    UTImagePrivate(const UTImagePrivate *other) :
        data( other->data.clone() )
    {}
    UTImagePrivate(const Mat &mat) :
        data(mat)
    {}

    Size GetSize() const;
    Rect ToBoundRect(const QRect &rect) const;
    Image8 Render8(CDataValue min, CDataValue max, const UTImagePrivate *mask = NULL);
    Image32 Render32(Image8 bytes, const QVector<QRgb> table);
    Image32 Render(const UTImagePalette &palette, const UTImagePrivate *mask);
    static const int colorTableLength = UCHAR_MAX + 1;

    Image8 PrepareMask(const UTImagePrivate *mask, const QRect &ROI, Mat *subImage, bool convertSubImage) const;
    Image8 ToMask() const;
    DataType SubImage(const QRect &ROI, const UTImagePrivate *mask = NULL) const;
    Image8 SubImage8(CDataValue alpha, CDataValue beta, const QRect &ROI, const UTImagePrivate *mask = NULL) const;

public:
    DataType data;                                                              /*!< 图像数据，格式为原始的C扫描特征值，没经过任何重映射 */
};

#ifndef QT_NO_DEBUG_STREAM
/*Q_CORE_EXPORT*/ QDebug operator <<(QDebug debug, const UTImagePrivate &image);
#endif

Point ToPoint(const QPoint &point);
Size ToSize(const QSize &size);
std::vector<Point> ToPointVector(const QPolygon &polygon);
Mat PolygonFToCV(const QPolygonF &polygon);
std::vector<Point2f> PolygonFToVec(const QPolygonF &polygon);
QPolygon ToQPolygon(const std::vector<Point> &contour);
Rect ToRect(const QRect &rect);
Rect ToBoundRect(const QRect &rect, const Mat &mat);

#endif                                                                          // UTIMAGEPRIVATE_H
