#include "UTImage.h"
#include "UTImagePrivate.h"
#include "utils/Util.h"
#include <QImage>
#include <QPixmap>
#include <vector>
#include <QPolygonF>

using std::vector;

//typedef Scalar_<CDataValue> UTScalar;
typedef double UTScalar;

const int typeId = qRegisterMetaType<UTImage>("UTImage");
const int listTypeId = qRegisterMetaType<UTImageList>("UTImageList");

UTImage::UTImage(UTImagePrivate *image) :
    UTSharedImagePrivate(image)
{}

/*!

 * \brief 默认构造体，为图像指定一个变换，并且填充。
 * \param trans 图像变换
 * \param fill 填充值
 */
UTImage::UTImage(int width, int height, CDataValue fill) :
    UTSharedImagePrivate( new UTImagePrivate(QSize(width, height), fill) )
{}

UTImage::UTImage(const QSize &size, CDataValue fill) :
    UTSharedImagePrivate( new UTImagePrivate(size, fill) )
{}

bool UTImage::IsValid() const
{
    return data() && !GetSize().isEmpty();
}

/*!
 * \brief 复制当前图像，这个操作会导致图像被完全复制
 * \return 指向复制后图像的共享指针
 */
UTImage UTImage::Copy() const
{
    return UTImage( new UTImagePrivate( data() ) );
}

/*!
 * \brief 复制当前图像的一部分，这个操作会导致图像被复制
 * \param rect 被复制的区域
 * \return 复制后的图像
 */
UTImage UTImage::Copy(const QRect &rect) const
{
    Mat clip( data()->data, ToRect(rect) );                                     /* 创建一个临时的矩阵头，指向自身的数据 */
    UTImage copy( new UTImagePrivate( QSize() ) );
    clip.copyTo(copy->data);                                                    /* 从临时的矩阵头中复制到新图像 */
    return copy;
}

void UTImage::CopyTo(UTImage target, const UTImage *mask) const
{
    data()->data.copyTo( target.data()->data, mask ? mask->data()->ToMask() : noArray() );
}

UTImage UTImage::CreateSameSize(CDataValue fill) const
{
    return UTImage(GetSize(), fill);
}

/*! \brief 获取图像的尺寸，单位为像素 */
QSize UTImage::GetSize() const
{
    return QSize(data()->data.cols, data()->data.rows);
}

QRect UTImage::GetRect() const
{
    return QRect( QPoint(0, 0), GetSize() );
}

int UTImage::GetWidth() const
{
    return data()->data.cols;
}

int UTImage::GetHeight() const
{
    return data()->data.rows;
}

int UTImage::GetPixelCount(const QRect &ROI) const
{
    if ( ROI.isNull() ) {
        return int( data()->data.total() );
    }
    QRect roi = BoundRect(ROI);
    return roi.width() * roi.height();
}

QRect UTImage::BoundRect(const QRect &rect) const
{
    ARS_CHECK_RETURN( rect.isValid(), GetRect() );
    int left = qBound(0, rect.left(), data()->data.cols - 1);
    int top = qBound(0, rect.top(), data()->data.rows - 1);
    int width = qBound(0, rect.width(), data()->data.cols - left);
    int height = qBound(0, rect.height(), data()->data.rows - top);
    return QRect(left, top, width, height);
}

/*! \brief 测试目标点是否在图像内 */
bool UTImage::Contains(int x, int y) const
{
    return data()->data.cols > x && data()->data.rows > y;
}

void UTImage::SetPoint(int x, int y, CDataValue v)
{
    ARS_CHECK( Contains(x, y) );
    SetPointFast(x, y, v);
}

#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
/*!
 * \brief 将数据帧绘制在图像中。绘制时会进行空指针和范围检查。
 * \sa SetPointFast
 */
void UTImage::SetPoint(UTSharedDataFrame frame, const ImageTransform &trans, int index, int xIndex, int yIndex)
{
    QPoint pixel = trans.MapToGrid(frame->GetPose(xIndex), frame->GetPose(yIndex));
    SetPoint( pixel.x(), pixel.y(), frame->CData.value(index) );
}

/*!
 * \brief 将数据片绘制在图像中，每个数据帧都会进行空指针和范围检查。
 * \sa SetPointsFast
 */
void UTImage::SetPoints(const UTSharedDataSegment &segment, const ImageTransform &trans, int index, int xIndex, int yIndex)
{
    qDebug() << "UTImage::SetPoints writing" << segment.count() << "frames";
    int count = 0;
    for (auto it = segment.cbegin(); it != segment.cend(); ++it) {
        SetPoint(*it, trans, index, xIndex, yIndex);
        ++count;
    }
    qDebug() << "UTImage:" << count << "points written";
}
#endif

void UTImage::SetPointFast(int x, int y, CDataValue v)
{
    data()->data(y, x) = v;
}

#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
/*!
 * \brief 将已知有效的数据帧绘制在图像中，假定frame不是空指针，而且point范围在图像内。
 * \sa SetPoint
 */
void UTImage::SetPointFast(UTSharedDataFrame frame, const ImageTransform &trans, int index, int xIndex, int yIndex)
{
    UTPose &pose = frame->Pose;
    QPoint pixel = trans.MapToGrid(pose[xIndex], pose[yIndex]);
    SetPointFast(pixel.x(), pixel.y(), frame->CData[index]);
}

/*!
 * \brief 将已知有效的数据片绘制在图像中，假定数据片中的每个帧都不是空指针，并且范围都在图像内。
 * \sa SetPoints
 */
void UTImage::SetPointsFast(const UTSharedDataSegment &segment, const ImageTransform &trans, int index, int xIndex, int yIndex)
{
    for (auto it = segment.cbegin(); it != segment.cend(); ++it) {
        SetPointFast(*it, trans, index, xIndex, yIndex);
    }
}
#endif

/*!
 * \brief 使用给定的调色板，将图像数据渲染为 QImage 。这个操作中图像会被复制 3 - 5 次，并可
 * 能涉及几十MB的内存交换。
 * \param palette 使用的调色板
 * \param mask 掩模图像的指针，掩模与图像尺寸必须相同，掩模中非 0 部分将被渲染为颜色表中
 *          1 - 255 ，为 0 的部分将会被渲染为颜色表中的 0 ，可以将颜色表中 0 设置为透明色
 *          来使无数据的部分为透明。
 * \return 渲染后的图像，格式为 QImage::Format_ARGB32_Premultiplied 。
 */
QImage UTImage::ToImage(const UTImagePalette &palette, const UTImage *mask) const
{
    UTImagePrivate::Image32 rendered = data()->Render(palette, mask ? mask->data() : NULL);
    QImage image(rendered.data, rendered.cols, rendered.rows, int(rendered.step), QImage::Format_ARGB32_Premultiplied);
    /* 由于构造时不会复制数据，而转换缓冲区即将被析构，执行深度复制 */
    return image.copy();
}

/*!
 * \brief 将图像数据缩放转换为8位图像。
 * \param min 数值转换的下限，低于或等于此值的像素将被映射为1
 * \param max 数值转换的上限，高于或等于此值的像素将被映射为255
 * \param mask 掩模图像的指针，掩模与图像尺寸必须相同。当指定掩模时，掩模中0像素将被映射为0，
 *          非零像素映射为非零值。
 * \return 渲染后的图像，格式为 QImage::Format_Indexed8
 */
QImage UTImage::ToIndexImage(CDataValue min, CDataValue max, const UTImage *mask) const
{
    UTImagePrivate::Image8 bytes = data()->Render8(min, max, mask ? mask->data() : NULL);
    QImage image(bytes.data, bytes.cols, bytes.rows, int(bytes.step), QImage::Format_Indexed8);
    return image.copy();
}

/*!
 * \brief 将图像渲染为 QPixmap ，与 ToImage 类似
 * \return 渲染后的图像
 */
QPixmap UTImage::ToPixmap(const UTImagePalette &palette, const UTImage *mask) const
{
    UTImagePrivate::Image32 rendered = data()->Render(palette, mask ? mask->data() : NULL);
    QImage image(rendered.data, rendered.cols, rendered.rows, int(rendered.step), QImage::Format_ARGB32_Premultiplied);
    return QPixmap::fromImage( image.copy() );
}

/*!
 * \brief 获取图像中的最大值和最小值
 * \param min 最小值
 * \param max 最大值
 * \param minLoc 最小值的坐标
 * \param maxLoc 最大值的坐标
 * \param mask 搜索范围的掩模
 * \param ROI 掩模的区域， ROI 的尺寸必须与 mask 的尺寸相同
 */
void UTImage::GetMinMax(CDataValue *min, CDataValue *max, QPoint *minLoc, QPoint *maxLoc, const UTImage *mask, const QRect &ROI) const
{
    if (!(min || max || minLoc || maxLoc)) {
        return;
    }
    double minBuffer = 0;
    double maxBuffer = 0;
    Point minLocBuffer;
    Point maxLocBuffer;
    Mat image;
    Mat maskBytes = data()->PrepareMask(mask ? mask->data() : NULL, ROI, &image, false);
    cv::minMaxLoc(image,
                  min ? &minBuffer : NULL, max ? &maxBuffer : NULL,
                  minLoc ? &minLocBuffer : NULL, maxLoc ? &maxLocBuffer : NULL,
                  mask ? maskBytes : noArray() );
    if (min) {
        *min = minBuffer;
    }
    if (max) {
        *max = maxBuffer;
    }
    if (minLoc) {
        *minLoc = QPoint(minLocBuffer.x, minLocBuffer.y);
    }
    if (maxLoc) {
        *maxLoc = QPoint(maxLocBuffer.x, maxLocBuffer.y);
    }
    if (!ROI.isNull()) {
        if (minLoc) *minLoc += ROI.topLeft();
        if (maxLoc) *maxLoc += ROI.topLeft();
    }
}

/*! \brief 统计图像中非0点的数量 */
int UTImage::CountNonZero(const UTImage *mask, const QRect &ROI) const
{
    if (mask) {                                                                 /* 如果指定了掩模，则需要构造临时缓冲区，将掩模之外的像素过滤掉 */
        Q_ASSERT( mask->GetRect() == BoundRect(ROI) );
        Rect roi = data()->ToBoundRect(ROI);
        Mat1b bytes(roi.size(), 0);
        Mat1b copy;
        data()->SubImage(ROI).convertTo(copy, CV_8U);
        Q_ASSERT(copy.size == bytes.size);
        copy.copyTo( bytes, mask->data()->ToMask() );
        return cv::countNonZero(bytes);
    }
    return cv::countNonZero( data()->SubImage(ROI) );                           /* 未指定掩模，只需取子图像进行统计即可 */
}

/*!
 * \brief 计算像素占积密度
 * \param mask 掩模
 * \param ROI 掩模的区域
 * \return 占积密度(非零像素数 / 有效像素数)
 */
double UTImage::CalcDensity(const UTImage *mask, const QRect &ROI)
{
    int area = mask ? mask->CountNonZero() : GetPixelCount();
    Q_ASSERT( !mask || mask->GetRect() == BoundRect(ROI) );

    int validArea = CountNonZero(mask, ROI);
    return double(validArea) / area;
}

/*! \brief 计算图像中所有像素值之和 */
CDataValue UTImage::GetSum() const
{
    return cv::sum(data()->data)[0];
}

/*!
 * \brief 计算图像中像素的平均值
 * \param mask 掩模的指针，为 NULL 时计算所有像素的均值
 * \param ROI 掩模的区域， ROI 的尺寸必须与 mask 的尺寸相同
 * \return 平均值
 */
CDataValue UTImage::GetMean(const UTImage *mask, const QRect &ROI) const
{
    if (!IsValid()) {
        return 0;
    }
    Q_ASSERT_X(!mask || ROI.isNull() || ROI.size() == mask->GetSize(), "UTImage::GetMean", "Size of ROI and mask mismatch");
    return cv::mean( data()->SubImage(ROI), mask ? mask->data()->ToMask() : noArray() )[0];
}

/*!
 * \brief 使用指定大小的子图像大小，对图像进行占积率检查，搜索占积率超过特定值的子图像
 * \param threshold 阈值，占积率超过阈值的图像会被返回
 * \param block 子图像尺寸，单位为像素
 * \param step 步进间距，单位为像素
 * \return 占积率高于阈值的子图像列表
 */
UTImage::FillInAreas UTImage::CheckBlockDensity(CDataValue threshold, const QSize &block, const QSize &step) const
{
    Mat1b bytes;
    data()->data.convertTo(bytes, CV_8U);

    QSize boundedBlock = block.boundedTo( QSize(bytes.cols, bytes.rows) );      /* 由于图像可能比子图像还小，将子图像矩形限制在目标图像的尺寸之内 */
    int blockArea = boundedBlock.width() * boundedBlock.height();
    FillInAreas areas;
    for ( int left = 0; left + boundedBlock.width() < bytes.cols; left += step.width() ) {
        for ( int top = 0; top + boundedBlock.height() < bytes.rows; top += step.height() ) {
            Mat1b subImage( bytes, Rect( left, top, boundedBlock.width(), boundedBlock.height() ) );
            double pixels = cv::countNonZero(subImage);
            double ratio = pixels / blockArea;
            if (ratio >= threshold) {
                QRect rect(QPoint(left, top), boundedBlock);
                areas.append( FillInArea(rect, ratio) );
            }
        }
    }
    return areas;
}

/*!
 * \brief 使用指定大小的区块尺寸遍历整个图像，寻找密度最高的区块
 * \param block 区块尺寸
 * \param step 遍历图像使用的步进间距
 * \param ROI (Region of Interest) 搜索区域，如果区块尺寸大于搜索区域，则区块尺寸会被限制在
 *        搜索区域的尺寸范围内。 ROI为空时，搜索整张图像。
 * \return 找到的区块密度和位置
 */
QPair<double, QRect> UTImage::FindMaxDensity(const QSize &block, const QSize &step, const QRect &ROI) const
{
    QRect roi = ROI.isNull() ? GetRect() : BoundRect(ROI);                      /* 确保 ROI 在图像内 */
    Mat1b bytes;
    data()->data.convertTo(bytes, CV_8U);

    double maxRatio = 0;
    QSize boundedBlock = block.boundedTo( GetSize() );                          /* 由于图像可能比子图像还小，将子图像矩形限制在目标图像的尺寸之内 */
    int blockArea = boundedBlock.width() * boundedBlock.height();
    QRect maxRect(QPoint(0, 0), boundedBlock);                                  /*!< 最大占积率所在的位置，尺寸设为区块尺寸，搜索中只需其更新位置即可 */

    for ( int left = roi.left(); left + boundedBlock.width() < roi.right(); left += step.width() ) {
        for ( int top = roi.top(); top + boundedBlock.height() < roi.bottom(); top += step.height() ) {
            Mat1b subImage( bytes, Rect( left, top, boundedBlock.width(), boundedBlock.height() ) );
            double pixels = cv::countNonZero(subImage);                         /* 将像素数转为浮点，避免整除 */
            double ratio = pixels / blockArea;
            if (ratio >= maxRatio) {
                maxRect.moveTopLeft( QPoint(left, top) );
                maxRatio = ratio;
            }
        }
    }
    return qMakePair(maxRatio, maxRect);
}

/*!
 * \brief 搜索图像中的边缘，图像会先被二值化，之后搜索结构性边缘。这个算法会将图像复制一次。
 * \param threshold 边缘搜索的阈值
 * \param mode 阈值的模式，
 *        THRESHOLD_ABOVE 表示高于阈值的为图形；
 *        THRESHOLD_BELOW 表示低于阈值的为图形
 * \return 边缘列表，每个边缘包含一个外边缘和若干内边缘
 */
UTImage::Boundaries UTImage::FindBoundaries(CDataValue threshold, UTImage::ThresholdMode mode, const QRect &ROI, const UTImage *mask) const
{
    if (!IsValid()) {
        return Boundaries();
    }
    QRect roi = BoundRect(ROI);
    Mat1b bytes = data()->SubImage8(mode, -threshold * mode, roi, mask ? mask->data() : NULL);
    qDebug() << "UTImage::FindBoundaries" << bytes << threshold << mode;

    vector<vector<Point> > contours;                                            /* 边缘点列表 */
    vector<Vec4i> hierarchy;                                                    /* 边缘结构描述表 */
    /* 执行边缘搜索 */
    cv::findContours(bytes, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, ToPoint(roi.topLeft()));

    ARS_CHECK_WARNING_RETURN( hierarchy.size() > 0, "Contour not found", Boundaries() );/* 未找到边缘，返回空列表 */
    ARS_CHECK_WARNING_RETURN( hierarchy.size() == contours.size(), "Contour size mismatch", Boundaries() );
    Boundaries boundaries;
    int outerIndex = 0;
    while (outerIndex >= 0) {
        Boundary boundary;

        /* 遍历属于当前外边缘的内边缘 */
        int innerIndex = hierarchy[outerIndex][2];
        while (innerIndex >= 0) {
            /* 当 mode 为 CV_RETR_CCOMP 时，OpenCV返回的树形结构应当只有2层 */
            ARS_CHECK_WARNING_RETURN(hierarchy[innerIndex][2] < 0, "Wrong hierarchy", boundaries);
            boundary.second.append( ToQPolygon(contours[innerIndex]) );         /* 记录内边缘 */

            innerIndex = hierarchy[innerIndex][0];                              /* 迭代到下一个内边缘 */
        }

        /* 处理当前外边缘 */
        boundary.first = ToQPolygon(contours[outerIndex]);
        boundaries.append(boundary);
        outerIndex = hierarchy[outerIndex][0];                                  /* 迭代到下一个外边缘 */
    }
    return boundaries;
}

QVector<int> UTImage::CalcHistogram(int resolution, CDataValue min, CDataValue max, const QRect &ROI, const UTImage *mask) const
{
    Mat subImage;
    Mat maskBytes = data()->PrepareMask(mask ? mask->data() : NULL, ROI, &subImage, false);
//    qDebug() << "subImage" << UTImagePrivate(subImage);
//    qDebug() << "maskBytes" << UTImagePrivate(maskBytes);
//    cv::imshow("MaskBytes", maskBytes);
//    qDebug() << "CalcHistogram" << ROI << subImage.dims << subImage.size[0] << subImage.size[1];

    Mat hist;
    if (min > max) {
        qSwap(min, max);
    } else if (min == max) {
        max = min + 1e-3;
    }
    const int channels = 0;
    const float range[2] = {min, float(max + 1e-3)};
//    qDebug() << "Using min-max=" << min << max;
    const float *ranges = range;
    cv::calcHist(&subImage, 1, &channels, mask ? maskBytes : noArray(), hist, 1, &resolution, &ranges, true, false);
    return QVector<int>::fromStdVector(hist);
}

/*! \brief 对2个图片逐像素进行加，结果存储在当前图像中
 *  \sa Subtract Multiply Divide */
UTImage &UTImage::Add(const UTImage &other)
{
    cv::add(data()->data, other->data, data()->data);
    return *this;
}

UTImage &UTImage::Add(CDataValue value)
{
    cv::add(data()->data, UTScalar(value), data()->data);
    return *this;
}

/*! \brief 对2个图片进行逐像素减，结果存储在当前图像中
 *  \sa Add Multiply Divide */
UTImage &UTImage::Subtract(const UTImage &other)
{
    cv::subtract(data()->data, other->data, data()->data);
    return *this;
}

UTImage &UTImage::Subtract(CDataValue value)
{
    cv::subtract(data()->data, UTScalar(value), data()->data);
    return *this;
}

/*! \brief 对2个图片进行逐像素乘，结果存储在当前图像中
 *  \sa Add Subtract Divide */
UTImage &UTImage::Multiply(const UTImage &other)
{
    cv::multiply(data()->data, other->data, data()->data);
    return *this;
}

UTImage &UTImage::Multiply(CDataValue value)
{
    cv::multiply(data()->data, UTScalar(value), data()->data);
    return *this;
}

/*! \brief 对2个图片进行逐像素除，结果存储在当前图像中
 *  \sa Add Subtract Multiply DividedBy */
UTImage &UTImage::Divide(const UTImage &other)
{
    cv::divide(data()->data, other->data, data()->data);
    return *this;
}

/*! \brief 用一个常量逐像素除，即 (value / image) ，结果存储在当前图像中
 *  注意，这个接口与 Divide 不相同
 *  \sa Divide */
UTImage &UTImage::DividedBy(CDataValue value)
{
    cv::divide(value, data()->data, data()->data);
    return *this;
}

/*! 对图像进行逐像素绝对值运算。这个操作会对图像执行一次复制 */
UTImage &UTImage::Abs()
{
    data()->data = cv::abs(data()->data);
    return *this;
}

UTImage UTImage::Compare(const UTImage &other, UTImage::CompareMode mode) const
{
    UTImage ret = CreateSameSize();
    cv::compare(data()->data, other.data()->data, ret.data()->data, mode);
    return ret;
}

UTImage UTImage::Compare(CDataValue value, UTImage::CompareMode mode) const
{
    Mat buffer;
    cv::compare(data()->data, value, buffer, mode);                             /* 如果 dst 使用预分配的 Mat1f ，则OpenCV 2.4.9 会在 _OutputArray::create 中断言失败 */
    UTImage ret;
    ret.data()->data = buffer;
    return ret;
}

UTImage UTImage::CompareRange(CDataValue min, CDataValue max, bool minAllowEq, bool maxAllowEq) const
{
    UTImage low = Compare(min, minAllowEq ? CMP_GE : CMP_GT);
    UTImage high = Compare(max, maxAllowEq ? CMP_LE : CMP_LT);
    return low.And(high);
}

UTImage UTImage::And(const UTImage &other) const
{
    UTImage ret = CreateSameSize();
    CopyTo(ret, &other);
    return ret;
}

UTImage UTImage::Or(const UTImage &other) const
{
    UTImage ret = Copy();
    other.CopyTo(ret, &other);
    return ret;
}

/*! 弹出窗口显示图像，供Debug使用。这个函数只在Debug时起效 */
void UTImage::DebugPrint(const QString &name) const
{
#ifdef QT_DEBUG
    qDebug() << "Image:" << name << GetSize();
    CDataValue min = 0;
    CDataValue max = 0;
    QPoint minLocation;
    QPoint maxLocation;
    GetMinMax(&min, &max, &minLocation, &maxLocation);
    qDebug() << "  Min:" << min << "at" << minLocation;
    qDebug() << "  Max:" << max << "at" << maxLocation;
    qDebug() << "  Total non-zero points:" << CountNonZero();
    imshow(name.toStdString(), data()->data);
#else
    Q_UNUSED(name);
#endif
}

/*!
 * \brief 对图像执行膨胀操作。膨胀是基本的形态学操作，与侵蚀互为对偶操作
 * \param pixels 膨胀的像素数
 * \return 自身的引用
 * \sa Erode
 */
UTImage &UTImage::Dilate(int pixels)
{
    if (IsValid()) {
        cv::dilate(data()->data, data()->data, Mat(), Point(-1, -1), pixels);
    }
    return *this;
}

/*!
 * \brief 对图像执行侵蚀操作。侵蚀是基本的形态学操作，与膨胀互为对偶操作
 * \param pixels 侵蚀的像素数
 * \return 自身的引用
 * \sa Dilate
 */
UTImage &UTImage::Erode(int pixels)
{
    cv::erode(data()->data, data()->data, Mat(), Point(-1, -1), pixels);
    return *this;
}

UTImage &UTImage::DilateDiscrete()
{
    Matx<uchar, 3, 3> kernel(
        0, 0, 0,
        0, 1, 1,
        0, 1, 0
    );
    cv::dilate( data()->data, data()->data, kernel, Point(-1, -1) );
    return *this;
}

/*!
 * \brief 在图像上绘制椭圆
 * \param center 椭圆的中心
 * \param size 椭圆的尺寸
 * \param angle 椭圆的倾角
 * \param startAngle 绘制椭圆时的起始角，当起始角和结束角为 0 - 360 度时绘制的是完整椭圆
 * \param endAngle 绘制椭圆时的结束角
 * \param fill 绘制椭圆使用的值
 * \return 自身的引用
 * \sa DrawPolygon http://docs.opencv.org/_images/ellipse.png
 */
UTImage &UTImage::DrawEllipse(const QPoint &center, const QSize &size, double angle, double startAngle, double endAngle, CDataValue fill)
{
    cv::ellipse(data()->data, ToPoint(center), ToSize(size), angle, startAngle, endAngle, UTScalar(fill), -1);
    return *this;
}

/*!
 * \brief 在图像上绘制多边形
 * \param polygon 多边形
 * \param fill 绘制多边形使用的值
 * \return 自身的引用
 */
UTImage &UTImage::DrawPolygon(const QPolygon &polygon, CDataValue fill)
{
    if (IsValid()) {
        const Point *points = reinterpret_cast<const Point *>( polygon.data() );
        const int numPoints = polygon.count();
        cv::fillPoly( data()->data, &points, &numPoints, 1, UTScalar(fill) );
    }
    return *this;
}

/*! \brief 操作图像的像素，可以对指定坐标的像素进行读写 */
CDataValue &UTImage::operator ()(int x, int y)
{
    return data()->data(y, x);
}

/*! \brief 读取只读图像的像素，无法进行写入 */
const CDataValue &UTImage::operator ()(int x, int y) const
{
    return data()->data(y, x);
}

/*! \brief 操作图像的像素，可以对指定坐标的像素进行读写 */
CDataValue &UTImage::operator ()(const QPoint &point)
{
    return data()->data( point.y(), point.x() );
}

/*! \brief 读取只读图像的像素，无法进行写入 */
const CDataValue &UTImage::operator ()(const QPoint &point) const
{
    return data()->data( point.y(), point.x() );
}

#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
UTImage UTImage::FromAData(const UTSharedDataSegment &segment, Qt::Orientation direction)
{
    if (segment.isEmpty()) {
        return UTImage();
    }
    /* 计算A扫描样本长度 */
    int waveSize = 0;
    for (auto it = segment.cbegin(); it != segment.cend(); ++it) {
        const UTSharedDataFrame &frame = (*it);
        if (frame.isNull()) {
            continue;
        }
        waveSize = frame->AData.size();
        if (waveSize) {
            break;
        }
    }
    if (waveSize == 0) {
        return UTImage();
    }

    int imageSize = segment.size();
    if (direction == Qt::Horizontal) {
        UTImage image(imageSize, waveSize);
        for (int index = 0; index < imageSize; ++index) {
            const UTSharedDataFrame &frame = segment[index];
            if (frame.isNull()) {
                continue;
            }
            const ADataFrame &waveform = frame->AData;
            for (int waveIndex = 0; waveIndex < waveform.count(); ++waveIndex) {
                image(index, waveIndex) = waveform[waveIndex];
            }
        }
        return image;
    } else {
        UTImage image(waveSize, imageSize);
        for (int index = 0; index < imageSize; ++index) {
            const UTSharedDataFrame &frame = segment[index];
            if (frame.isNull()) {
                continue;
            }
            const ADataFrame &waveform = frame->AData;
            for (int waveIndex = 0; waveIndex < waveform.count(); ++waveIndex) {
                image(waveIndex, index) = waveform[waveIndex];
            }
        }
        return image;
    }
}
#endif

QDebug operator <<(QDebug debug, const UTImage &image)
{
    return debug << *image.data();
}

float FindEnclosingCircle(const QPolygonF &points, QPointF *center)
{
    float radius = 0;
    Point2f cvCenter;
    cv::minEnclosingCircle(PolygonFToVec(points), cvCenter, radius);
    if (center) {
        *center = QPointF(cvCenter.x, cvCenter.y);
    }
    return radius;
}

QPolygonF FindEnclosingRect(const QPolygonF &polygon)
{
    RotatedRect rect = cv::minAreaRect( PolygonFToVec(polygon) );
    Point2f rectPoints[4];
    rect.points(rectPoints);
    QPolygonF result;
    for (int index = 0; index < 4; ++index) {
        result << QPointF(rectPoints[index].x, rectPoints[index].y);
    }
    return result;
}

double PolygonArea(const QPolygonF &polygon)
{
    return cv::contourArea( PolygonFToVec(polygon) );
}

QPolygon ApproximateCircle(const QPoint &center, int radius)
{
    std::vector<Point> points;
    cv::ellipse2Poly(ToPoint(center), Size(radius, radius), 0, 0, 360, 10, points);
    return ToQPolygon(points);
}

double PolygonDistance(QPolygonF p1, QPolygonF p2)
{
    if ( p1.count() < p2.count() ) {                                            /* 将较长的多边形转为 OpenCV 的结构，减少调用 cv 的次数 */
        p1.swap(p2);
    }
    double minDist = -DBL_MAX;                                                  /* OpenCV 返回的距离为负值，值越大则越近 */
    std::vector<Point2f> p1vec = PolygonFToVec(p1);
#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
    for (auto it = p2.constBegin(); it != p2.constEnd(); ++it) {
        double dist = cv::pointPolygonTest(p1vec, Point2f( it->x(), it->y() ), true);
        minDist = qMax(minDist, dist);
    }
#else
	for (QPolygonF::ConstIterator it = p2.constBegin(); it != p2.constEnd(); ++it) {
		double dist = cv::pointPolygonTest(p1vec, Point2f( it->x(), it->y() ), true);
		minDist = qMax(minDist, dist);
	}
#endif
    return -minDist;
}

/*! 用于 GroupPolygon 中 cv::partition 的协助类，评判两个多边形是否可以合并 */
struct PolygonComparator
{
    PolygonComparator(double dist, double eps = 0, bool epsMax = true) : minDist(dist), minEPS(eps), epsWithMax(epsMax) {}
    inline bool operator ()(const QPolygonF &polygon1, const QPolygonF &polygon2)
    {
        return PolygonDistance(polygon1, polygon2) <= minDist;
    }
    inline bool operator ()(const QPair<QPolygonF, double> &polygon1, const QPair<QPolygonF, double> &polygon2)
    {
        double dist = PolygonDistance(polygon1.first, polygon2.first);
        double length = epsWithMax ? qMax(polygon1.second, polygon2.second) : qMin(polygon1.second, polygon2.second);
        return (dist < minDist) || ( dist < (minEPS * length) );                /* 间距小于绝对间距阈值，或小于绝对间距 * eps 则判为同一组 */
    }
    double minDist;                                                             /*!< 绝对距离阈值 */
    double minEPS;                                                              /*!< 相对距离阈值，为0时代表忽略相对阈值检查 */
    bool epsWithMax;
};

/*!
 * \brief 按距离对多个多边形进行分组，将间距小于指定值的一项或多项合并为一组。
 * \param polygon 输入多边形列表
 * \param minDistance 距离阈值，间距小于此阈值的将被标记为同一组
 * \param labelCount 输出分组结果中，标签的总数
 * \return 组标签，同一组的多边形标签相同。这个 QVector 的长度与输入 polygon 相同
 */
QVector<int> GroupPolygons(const QVector<QPolygonF> &polygon, double minDistance, int &labelCount)
{
    ARS_CHECK_RETURN( polygon.toStdVector().size() > 0, QVector<int>() );
    std::vector<int> labels;
    labelCount = cv::partition( polygon.toStdVector(), labels, PolygonComparator(minDistance) );
    return QVector<int>::fromStdVector(labels);
}

/*!
 * \brief 按绝对距离和相对距离对多个多边形进行分组，将绝对距离或相对距离小于指定值的一项或多项合并为一组
 * \param polygon 多边形及其指示长度的列表
 * \param minDistance 绝对距离的阈值，间距小于此阈值的将被标记为同一组
 * \param minEPS 相对距离的阈值，即绝对距离除以任一指示长度小于此阈值的将被标记为同一组
 * \param labelCount 输出结果中，标签的总数
 * \return 组标签，同一组的多边形标签相同。这个 QVector 的长度与输入 polygon 相同
 */
QVector<int> GroupPolygons(const QVector<QPair<QPolygonF, double> > &polygon, double minDistance, double minEPS, int &labelCount, bool epsWithMax)
{
    ARS_CHECK_RETURN( polygon.toStdVector().size() > 0, QVector<int>() );
    std::vector<int> labels;
    labelCount = cv::partition( polygon.toStdVector(), labels, PolygonComparator(minDistance, minEPS, epsWithMax) );
    return QVector<int>::fromStdVector(labels);
}

typedef const QRect * Edge;
typedef QMultiMap<int, Edge> EdgeMap;                                           /*!< 排序的边缘描述表，使用迭代器遍历此描述表即可按顺序遍历所有边缘 */
typedef QSet<Edge> EdgeSet;
template<typename Iterator>
EdgeSet MakeEdgeSet(Iterator begin, Iterator end)
{
    EdgeSet edges;
    for (Iterator it = begin; it != end; ++it) {
        edges.insert(it.value());
    }
    return edges;
}

/*!
 * \brief 使用指定大小的区块尺寸，寻找在该区块内物体密度最高的位置
 * \param objects 物体列表
 * \param block 区块尺寸
 * \return 物体数量，以及找到的区块位置
 */
QPair<int, QRect> FindMaxObjectDensity(const QVector<QRect> &objects, const QSize &block, const QRect &ROI)
{
    int maxCount = 0;
    QRect maxRect(QPoint(0, 0), block);                                         /*!< 最大密度所在的位置 */

    EdgeMap EdgesV;                                                             /*!< 纵向边缘表 */
#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
    for (auto it = objects.constBegin(); it != objects.constEnd(); ++it) {
        EdgesV.insertMulti(it->left(), it);
        EdgesV.insertMulti(it->right(), it);
    }
#else 
	int count = objects.count();
	for (int i = 0; i < count; i++) {
		EdgesV.insertMulti(objects.value(i).left(), &objects.value(i));
		EdgesV.insertMulti(objects.value(i).right(), &objects.value(i));
	}
#endif

    typedef EdgeMap::ConstIterator Iterator;
    Iterator itBeginV = EdgesV.upperBound(ROI.left());                          /*!< ROI左边界的下一个边缘 */ /* O(log n) */
    Iterator itEndV = EdgesV.upperBound(ROI.right());                           /*!< ROI右边界的下一个边缘 */ /* O(log n) */
    for (Iterator itV = itBeginV; itV != itEndV; ++itV) {                       /* 遍历ROI.left~ROI.right之间的有效边缘 */ /* O(n2 log n) */
        Iterator itEndBlockRight = EdgesV.upperBound(itV.key() + block.width()); /* O(log n) */
        EdgeSet edgeSetV = MakeEdgeSet(itV, itEndBlockRight);
        if (edgeSetV.count() <= maxCount) {
            continue;
        }

        EdgeMap EdgesH;                                                         /*!< 横向边缘表 */
        foreach (Edge edge, edgeSetV) {                                         /* O(n) */
            EdgesH.insertMulti(edge->top(), edge);
            EdgesH.insertMulti(edge->bottom(), edge);
        }

        Iterator itBeginH = EdgesH.upperBound(ROI.top());
        Iterator itEndH = EdgesH.upperBound(ROI.bottom());
        for (Iterator itH = itBeginH; itH != itEndH; ++itH) {                   /* O(n log n) */
            Iterator itEndBlockBottom = EdgesH.upperBound(itH.key() + block.height());
            EdgeSet edgeSetH = MakeEdgeSet(itH, itEndBlockBottom);              /* O(log n) */
            int count = edgeSetH.count();
            if (count > maxCount) {
                maxCount = count;
                maxRect.moveTopLeft(QPoint(itV.key(), itH.key()));
            }
        }
    }

    return qMakePair(maxCount, maxRect);
}
