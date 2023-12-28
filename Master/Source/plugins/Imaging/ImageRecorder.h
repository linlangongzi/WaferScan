#pragma once

#include <QObject>
#include "ImageRecord.h"
#include <Core/IDocument.h>

class ImageRecorder : public QObject
{
    Q_OBJECT

public:
    explicit ImageRecorder(QObject *parent = Q_NULLPTR);

    void acquireFrame(const UTDataFrame &frame);
    void acquireSegment(const UTDataSegment &segment);
    void addImageRecord(const QString &fileName, const RecordConfig &config, const QHash<int, UTSetup> &setups);
    ImageRecord *imageRecord(const QString &fileName);

    int recordCount() const;
    void clear();

private:
    void reduceFrame(ImageRecord *record, const ImageOptions &options, const QPoint &point, const UTDataFrame &frame);

private:
    QList<ImageRecord *> m_imageRecords;
};
