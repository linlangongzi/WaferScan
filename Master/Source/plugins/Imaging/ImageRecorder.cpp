#include "ImageRecord.h"
#include "ImageRecorder.h"
#include <QLoggingCategory>
#include <Core/DocumentManager.h>

static Q_LOGGING_CATEGORY(debug, "image.recorder");

ImageRecorder::ImageRecorder(QObject *parent) :
    QObject(parent)
{
}

void ImageRecorder::acquireFrame(const UTDataFrame &frame)
{
    int channel = frame.channelId();
    for (ImageRecord *record: m_imageRecords) {
        ImageOptions options = record->imageOptions();
        if (!options.isChannelEnabled(channel)) {                               // 通道过滤
            continue;
        }
        if (!options.isDirectionEnabled(frame.isReversed(options.axisX()))) {   // 编码器方向过滤
            continue;
        }
        if (options.coordinateType() == ImageOptions::CoordRobotic
                && !frame.IsIGateTriggered()) {                                 // 机器人运动时的闸门过滤
            continue;
        }

        QPointF sourcePoint = options.MapToSource(frame, channel);              // 获取原始坐标

        const QPointF point = options.MapToSource(sourcePoint);                // 转换成像素坐标
        const QPoint p = point.toPoint();
        reduceFrame(record, options, p, frame);
        if (options.coordinateType() == ImageOptions::CoordThetaR) {
            static const float margin = 0.5f;
            float diff = point.x() - p.x();
            if (qAbs(diff) <= margin) {
                reduceFrame(record, options, p + QPoint(diff > 0 ? 1 : -1, 0), frame);
            }
            diff = point.y() - p.y();
            if (qAbs(diff) <= margin) {
                reduceFrame(record, options, p + QPoint(0, diff > 0 ? 1 : -1), frame);
            }
        }
    }
}

void ImageRecorder::acquireSegment(const UTDataSegment &segment)
{
    if (segment.isEmpty() || !m_imageRecords.isEmpty()) {
        return;
    }

    for (const UTDataFrame &frame: segment) {
        acquireFrame(frame);
    }
}

void ImageRecorder::addImageRecord(const QString &fileName, const RecordConfig &config, const QHash<int, UTSetup> &setups)
{
    IDocument *doc = DocumentManager::currentDocument();
    ImageRecord *record = doc->addPart<ImageRecord>(fileName);
    if (record) {
        record->setSetups(setups);
        record->setRecordConfig(config);
        m_imageRecords << record;
        qCInfo(debug) << "Adding image record " << fileName;
    }
}

ImageRecord *ImageRecorder::imageRecord(const QString &fileName)
{
    IDocument *doc = DocumentManager::currentDocument();
    return doc->part<ImageRecord>(fileName);
}

int ImageRecorder::recordCount() const
{
    return m_imageRecords.size();
}

void ImageRecorder::clear()
{
    qDeleteAll(m_imageRecords);
}

void ImageRecorder::reduceFrame(ImageRecord *record, const ImageOptions &options, const QPoint &point, const UTDataFrame &frame)
{
    const QSize size = record->recordConfig().size;
    if (uint(point.x()) >= uint(size.width()) || uint(point.y()) >= uint(size.height())) { // 范围验证
        return;
    }

    const qreal newValue = frame.feature(options.featureIndex());
    const qreal oldValue = record->featureAt(options.featureIndex(), point); // 取出原有CDataValue
    if ((options.reducePolicy() == ImageOptions::ReduceByMax && newValue <= oldValue)
        || (options.reducePolicy() == ImageOptions::ReduceByMin && newValue >= oldValue)) {
        return;
    }

    record->setFeaturesAt(point, frame.features());
    const auto waveform = frame.waveform();
    if (!record->hasAscanLayer() && waveform.size() > 0 && record->recordConfig().hasWaveform) {
        record->createAscanLayer();
    }
    if (record->hasAscanLayer()) {
        record->setWaveformAt(point, waveform);
    }
}
