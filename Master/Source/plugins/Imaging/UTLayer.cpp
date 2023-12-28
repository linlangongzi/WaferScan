#include "UTLayer.h"
#include <Ars500/ARS500.h>
#include "Ars1000/PipeInstrumentDriver.h"
#include <allrising/QtExtend.h>
#include <Imaging/DefectItemModel.h>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UTDataHeader.h>

UTLayer::UTLayer(QObject *parent) :
    QObject(parent),
    model( new DefectItemModel(this) ),
    id(1)
{
    startTimer(500);
}

/*!
 * \brief UTLayer::GetModel 获得layer的model
 * \return
 */
DefectItemModel *UTLayer::GetModel()
{
    return model;
}

/*! 由json反序列化 */
void UTLayer::FromJson(const QJsonObject &obj, const ImageOptions &fallbackOptions)
{
    id = obj["ID"].toString().toInt();
    model->loadJson(obj["UTDefectList"]);

    ImageOptions newOptions = fallbackOptions;
    if (obj.contains("ImageOptions")) {
        newOptions = ImageOptions::fromJson(obj["ImageOptions"]);
    }
    if (obj.contains("ChannelFilter")) {
        MotionAxisList channels;
        foreach (const QJsonValue &value, obj["ChannelFilter"].toArray()) {
            channels.append(value.toBool());
        }
        if (channels.isEmpty()) {
            channels.append(true);                                              // 默认第一个通道
        }
        newOptions.setChannels(channels);
    }
    if (imageOptions == newOptions) {
        Reset();                                                                // 加入心打开的文件与当前文件的options相同，需要手动清空缓冲区
    }
    SetImageOptions(newOptions);
}

/*!
 * \brief UTLayer::ToJson 序列化成json
 * \return
 */
QJsonObject UTLayer::ToJson() const
{
    QJsonObject obj;
    obj["ID"] = QString::number(id);
    obj["UTDefectList"] = model->toJson();
    obj["ImageOptions"] = imageOptions.toJson();
    return obj;
}

ImageOptions UTLayer::GetImageOptions() const
{
    return imageOptions;
}

bool UTLayer::GetDataInputEnabled() const
{
    return dataInputEnabled;
}

UTDataFrame UTLayer::dataFrameAt(const QPoint &point) const
{
    if (imageOptions.GridContains(point) && data.size()) {
        int offset = point.y() * imageOptions.targetRect().width() + point.x();
        return data.at(offset);
    } else {
        return UTDataFrame();
    }
}

UTDataSegment UTLayer::dataRectAt(const QRect &rect) const
{
    if (!data.size()) {
        return UTDataSegment();
    }
    QRect dataRect = QRect(QPoint(0, 0), imageOptions.targetRect());
    QRect validRect = dataRect.intersected(rect);
    UTDataSegment segment;
    segment.reserve(rect.width() * rect.height());
    for (int row = validRect.top(); row <= validRect.bottom(); ++row) {
        int rowOffset = row * dataRect.width();
        for (int col = validRect.left(); col <= validRect.right(); ++col) {
            segment << data.at(rowOffset + col);
        }
    }
    return segment;
}

UTDataSegment UTLayer::sliceAt(Qt::Orientation orientation, int index) const
{
    if (orientation == Qt::Horizontal) {
        QRect row(0, index, imageOptions.targetRect().width(), 1);
        return dataRectAt(row);
    } else {
        QRect col(index, 0, 1, imageOptions.targetRect().height());
        return dataRectAt(col);
    }
}

UTDataSegment UTLayer::dataAll() const
{
    if (!data.size() || data.size() >= INT_MAX) {
        return UTDataSegment();
    }
    UTDataSegment segment;
    segment.reserve(int(data.size()));
    foreach (const UTDataFrame &frame, data) {
        segment << frame;
    }
    return segment;
}

UTImageList UTLayer::imageList() const
{
    return images;
}

void UTLayer::SetImageOptions(const ImageOptions &options)
{
    if (imageOptions == options) {
        return;
    }
    imageOptions = options;
    Reset();
    emit ImageOptionsChanged(imageOptions);
}

void UTLayer::Reset()
{
    using Ars500::UTDriverARS500;
    using PipeDetect::PipeInstrumentDriver;

    QSize dataRect = imageOptions.targetRect();
    if (!dataRect.isValid()) {
        dataRect = QSize(0, 0);
    }
    qDebug() << "UTLayer::Reset to new size" << dataRect.height() << "rows, " << dataRect.width() << "cols";
    data.clear();
    outputBuffer.clear();
    images.clear();
    emit ImagesUpdated(images);

    try {
        data.resize(dataRect.height() * dataRect.width());
//        data.squeeze();
#ifdef PIPE_DETECT
        int numValues = PipeInstrumentDriver::InitDataHeader().GetNumFeatures();
#else
        int numValues = UTDriverARS500::InitDataHeader().GetNumFeatures();
#endif
        images.reserve(numValues);
        for (int index = 0; index < numValues; ++index) {
            images.append(UTImage(imageOptions.targetRect()));
        }
    } catch (std::bad_alloc err) {
        qCritical() << "UTLayer: failed allocating memory" << err.what();
        data.clear();
        images.clear();
    }
    emit ImagesUpdated(images);
}

void UTLayer::OnDataArrived(const UTDataSegment &segment)
{
    if (data.empty() || segment.isEmpty() || !dataInputEnabled) {
        return;
    }
    for (const auto &frame: segment) {
        int channel = frame.Context.GetChannelID();
        if (!imageOptions.isChannelEnabled(channel) || !imageOptions.isDirectionEnabled(frame.Context.IsReverse(imageOptions.axisX()))) {
            continue;
        }
        if ((imageOptions.coordinateType() == ImageOptions::CoordRobotic) && !frame.IsIGateTriggered()) {
            continue;
        }
        QPointF scenePoint = imageOptions.MapToSource(frame, channel);
        QPointF point = imageOptions.MapToSource(scenePoint);
        const QPoint p = point.toPoint();
        ReduceFrame(p, frame);
        if (imageOptions.coordinateType() == ImageOptions::CoordThetaR) {
            static const float margin = 0.5f;
            float diff = point.x() - p.x();
            if (qAbs(diff) <= margin) {
                ReduceFrame(p + QPoint(diff > 0 ? 1 : -1, 0), frame);
            }
            diff = point.y() - p.y();
            if (qAbs(diff) <= margin) {
                ReduceFrame(p + QPoint(0, diff > 0 ? 1 : -1), frame);
            }
        }
    }
    emit ProcessorFinished();
}

void UTLayer::ReduceFrame(const QPoint &point, const UTDataFrame &frame)
{
    if (imageOptions.GridContains(point)) {
        int featureIndex = imageOptions.featureIndex();
        int colLength = imageOptions.targetRect().width();
        int offset = point.y() * colLength + point.x();

        UTDataFrame origFrame = data[offset];
        bool merge = origFrame.isEmpty();

        if (!merge) {
            switch (imageOptions.reducePolicy()) {
            case ImageOptions::ReduceByMax:
                merge |= (frame.CData.value(featureIndex) > origFrame.CData.value(featureIndex));
                break;
            case ImageOptions::ReduceByMin:
                merge |= (frame.CData.value(featureIndex) < origFrame.CData.value(featureIndex));
                break;
            default:
                merge |= true;
                break;
            }
        }

        /* 合并到矩阵中 */
        if (merge) {
            data[offset] = frame;
            outputBuffer[point] = frame;                             /* 使用QPoint索引缓冲区，将其插入 */
        }
    }
}

void UTLayer::SetDataInputEnabled(bool enabled)
{
    dataInputEnabled = enabled;
}

void UTLayer::timerEvent(QTimerEvent *)
{
    if (outputBuffer.isEmpty()) {
        return;
    }
    for (auto it = outputBuffer.cbegin(); it != outputBuffer.cend(); ++it) {
        for (int index = 0; index < images.size(); ++index) {
            const QPoint &point = it.key();
            images[index].SetPoint(point.x(), point.y(), it->CData.value(index));
        }
    }
    QHash<QPoint, UTDataFrame> updated;
    outputBuffer.swap(updated);
    emit MatrixDataUpdated(updated);
    emit ImagesUpdated(images);
}

void UTLayer::RequestDataAll(void *reference)
{
    emit SegmentUpdate(dataAll(), reference);
}
