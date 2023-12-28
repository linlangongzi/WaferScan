#include "UTImagePrivate.h"
#include "utils/Util.h"

Size UTImagePrivate::GetSize() const
{
    if (data.dims <= 2)
    {
        return Size(data.cols, data.rows);
    }
    return Size(data.size.p[1], data.size.p[0]);
}

Rect UTImagePrivate::ToBoundRect(const QRect &rect) const
{
    Size imageSize = GetSize();
    ARS_CHECK_RETURN(rect.isValid(), Rect(Point(0, 0), imageSize));
    int left = qBound(0, rect.left(), imageSize.width - 1);
    int top = qBound(0, rect.top(), imageSize.height - 1);
    int width = qBound(0, rect.width(), imageSize.width - left);
    int height = qBound(0, rect.height(), imageSize.height - top);
    return Rect(left, top, width, height);
}

UTImagePrivate::Image8 UTImagePrivate::Render8(CDataValue min, CDataValue max, const UTImagePrivate *mask)
{
    /* 如果掩模图像大小与当前图像不符，忽略掩模 */
    if ( mask && GetSize() != mask->GetSize() )
    {
        qWarning() << "UTImage::ToImage: mask size mismatch";
        mask = NULL;
    }

    /* 将 float 原始数据映射为 (1 - 255) ，掩模外的部分映射为 0 ，因为 LUT 只能接受 CV_8U
     * 或 CV_8S 的输入，在映射时，超过调色板上下限的部分将被 OpenCV 做饱和处理 */
    /* 首先将 float 映射到 (0 - 254) */
//    qDebug() << "Converting to CV_8U with range" << min << "-" << max;
    Image8 bytes;
    double alpha = double(UCHAR_MAX - 1) / (max - min);
    double beta = -min * alpha;
    data.convertTo(bytes, CV_8U, alpha, beta);

    /* 准备掩模后的 CV_8U 图像，如果有掩模，分配新的全为 0 的缓冲区，引用旧的 bytes 缓冲区即可 */
    if (mask)
    {
        Image8 bytesMasked(GetSize(), 0);
        cv::add( bytes, 1, bytesMasked, mask->ToMask() );
        return bytesMasked;
    }
    cv::add(bytes, 1, bytes);
    return bytes;
}

UTImagePrivate::Image32 UTImagePrivate::Render32(UTImagePrivate::Image8 bytes, const QVector<QRgb> table)
{
    /* 构造 OpenCV 能接受的颜色表，数据区指向栈中的 palette ，这个颜色表析构时不会销毁数据 */
    Q_ASSERT(table.size() == colorTableLength);                                 /* 调色板长度检查 */
    ARS_CHECK_WARNING_RETURN( table.size() >= colorTableLength, "Invalid colorTable length", Image32() );
    int *paletteData = const_cast<int *>( reinterpret_cast<const int *>( table.data() ) );
    Image32 colorTable(1, colorTableLength, paletteData);

    /* 使用 LUT 渲染到 RGB 缓冲区 */
    Image32 rendered;
    /* 调色板中颜色表的字长应当与渲染后图像的字长一致 */
    Q_STATIC_ASSERT( sizeof(UTImagePalette::ColorTable::value_type) == sizeof(Image32::value_type) );

#ifdef QT_DEBUG
//    qDebug() << "lutcn:" << colorTable.channels();
//    qDebug() << "cn:" << bytes.channels();
//    qDebug() << "lut.total:" << colorTable.total();
//    qDebug() << "lut.isContinuous:" << colorTable.isContinuous();
//    qDebug() << "src.depth:" << bytes.depth() << " CV_8U:" << CV_8U;
#endif

    /* 执行渲染 */
    cv::LUT(bytes, colorTable, rendered);
    return rendered;
}

UTImagePrivate::Image32 UTImagePrivate::Render(const UTImagePalette &palette, const UTImagePrivate *mask)
{
//    qDebug() << "UTImage:Rendering at" << *this;

    Image8 bytes = Render8(palette.min, palette.max, mask);
#if defined(QT_DEBUG) && QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    double maxima = -qInf();
    double minima = qInf();
    cv::minMaxIdx(data, &minima, &maxima);
//    qDebug() << "MinMax of original image:" << data << minima << maxima << ", total zero" << cv::countNonZero(data);
    if (mask)
    {
        Image8 maskBytes = mask->ToMask();
        cv::minMaxIdx(maskBytes, &minima, &maxima);
//        qDebug() << "MinMax of mask:" << maskBytes << minima << maxima << ", total zero" << cv::countNonZero( mask->ToMask() );
    }
    cv::minMaxIdx(bytes, &minima, &maxima);
//    qDebug() << "MinMax of 8U buffer:" << bytes << minima << maxima << ", total zero" << cv::countNonZero(bytes);
#endif

    return Render32(bytes, palette.colorTable);
}

UTImagePrivate::Image8 UTImagePrivate::PrepareMask(const UTImagePrivate *mask, const QRect &ROI, Mat *subImage, bool convertSubImage) const
{
    Rect roi = ToBoundRect(ROI);
    Mat dataRoi(data, roi);
    if (subImage) {
        *subImage = Mat(data, roi);
    }
    if (!mask) {
        if (subImage && convertSubImage) {
            dataRoi.convertTo(*subImage, CV_8U);
        }
        return Image8();
    }

    Mat maskRoi;
    if (mask->GetSize() == GetSize()) {
        maskRoi = Mat(mask->data, roi);
    } else if (mask->GetSize() == roi.size()) {
        maskRoi = mask->data;
    } else {
        qCritical() << "Mask size mismatch";
        return Mat();
    }

    Mat maskBytes;
    maskRoi.convertTo(maskBytes, CV_8U);
    if (subImage && convertSubImage) {
        if (mask == this) {                                                     /* 如果mask为自身，则复用已转换的数据 */
            *subImage = maskBytes;
        } else {
            dataRoi.convertTo(*subImage, CV_8U);
        }
    }
    return maskBytes;
}

/*! \brief 将图像转为 Mat1b ，供掩模使用 */
UTImagePrivate::Image8 UTImagePrivate::ToMask() const
{
    Mat1b mask;
    data.convertTo(mask, CV_8U, 255);
    return mask;
}

bool CheckRect(const UTImagePrivate *image, const QRect &rect)
{
    if (!image)
    {
        return true;
    }
    ARS_CHECK_RETURN(rect.isNull(), true);
    return rect.width() == image->data.cols && rect.height() == image->data.rows;
}

UTImagePrivate::DataType UTImagePrivate::SubImage(const QRect &ROI, const UTImagePrivate *mask) const
{
    if (!CheckRect(mask, ROI))
    {
        qWarning() << "Mask and ROI size mismatch, ignoring mask";
        mask = NULL;
    }
    Rect roi = ToBoundRect(ROI);
    DataType subImage(data, roi);
    ARS_CHECK_RETURN(mask, subImage);
    DataType masked = DataType(roi.size(), 0);
    return masked.setTo(subImage, mask->ToMask());
}

UTImagePrivate::Image8 UTImagePrivate::SubImage8(CDataValue alpha, CDataValue beta, const QRect &ROI, const UTImagePrivate *mask) const
{
    if (!CheckRect(mask, ROI))
    {
        qWarning() << "Mask and ROI size mismatch, ignoring mask";
        mask = NULL;
    }
    Rect roi = ToBoundRect(ROI);
    Image8 bytes;
    data(roi).convertTo(bytes, CV_8U, alpha, beta);
    if (!mask)
    {
        return bytes;
    }
    Image8 bytesMasked(roi.size(), 0);
    bytes.copyTo(bytesMasked, mask->ToMask());
    return bytesMasked;
}

QDebug operator <<(QDebug debug, const UTImagePrivate &image)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    const bool oldSetting = debug.autoInsertSpaces();
#endif
#ifdef QT_DEBUG
    double min, max;
    cv::minMaxIdx(image.data, &min, &max);
#endif
    debug.nospace() << "UTImage(" << image.data.cols << "x" << image.data.rows
#ifdef QT_DEBUG
    << ", MinMax(" << min << "-" << max << ")"
#endif
    << ")";
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    debug.setAutoInsertSpaces(oldSetting);
#endif
    return debug.maybeSpace();
}

Point ToPoint(const QPoint &point)
{
    return Point( point.x(), point.y() );
}

Size ToSize(const QSize &size)
{
    return Size( size.width(), size.height() );
}

QPolygon ToQPolygon(const std::vector<Point> &contour)
{
    QPolygon polygon;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    Q_ASSERT( sizeof(Point) == sizeof(QPoint) );
    Q_ASSERT( (&contour.data()->y - &contour.data()->x) == 1 );                 /* 从 vector<Point> 到QPolygon的转换使用底层指针转换，其实现基于 Point 中 x/y
                                                                                 * 均为int，且内存布局 p.x 在前 p.y 在后，没有额外内存对齐的假设 */
    polygon.setPoints(int( contour.size() ), &contour.data()->x);
#else
    for (std::vector<Point>::const_iterator it = contour.begin(); it != contour.end(); ++it)
    {
        polygon.append( QPoint(it->x, it->y) );
    }
#endif
    return polygon;
}

Rect ToRect(const QRect &rect)
{
    return Rect( rect.x(), rect.y(), rect.width(), rect.height() );
}

Mat PolygonFToCV(const QPolygonF &polygon)
{
    return Mat( 1, polygon.size(), CV_32FC2, const_cast<void *>( reinterpret_cast<const void *>( polygon.data() ) ) );
}

std::vector<Point2f> PolygonFToVec(const QPolygonF &polygon)
{
    std::vector<Point2f> vec( polygon.count() );
    for (int index = 0; index < polygon.count(); ++index)
    {
        const QPointF &point = polygon[index];
        vec[index] = Point2f( point.x(), point.y() );
    }
    return vec;
}

