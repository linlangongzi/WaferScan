#include "ImageRecordLayer.h"
#include "ImageRecord.h"
#include <QDataStream>
#include <QElapsedTimer>
#include <QLoggingCategory>
#include <QSaveFile>

static Q_LOGGING_CATEGORY(debug, "image.record.layer");

ImageRecordLayer::ImageRecordLayer(QObject *parent) :
    QObject(parent)
{

}

void ImageRecordLayer::ensureOpen()
{
    if (triedOpen) {
        return;
    }
    triedOpen = true;

    ImageRecord *record = qobject_cast<ImageRecord *>(parent());
    if (!record) {
        qCritical(debug) << "Cannot find image record, file" << config.fileName;
        return;
    }

    QDir dir = record->layerDir();

    QString errorString;
    QElapsedTimer timer;
    timer.start();
    if (dir.exists(config.fileName)) {
        if (!readFileAsDataStream(dir.filePath(config.fileName), image, &errorString)) {
            qCWarning(debug) << "Failed opening file:" << errorString;
        } else {
            qCDebug(debug) << "Finished reading file, time" << timer.elapsed() << "ms";
        }
    }

    const QSize &size = record->recordConfig().size;
    if (image.GetSize() != size) {
        image = UTImage(size);
        qCDebug(debug) << "Allocated image, time" << timer.restart() << "ms";
    }
}

void ImageRecordLayer::saveFile()
{
    if (!modified) {
        return;
    }

    ImageRecord *record = qobject_cast<ImageRecord *>(parent());
    if (!record) {
        qCritical(debug) << "Cannot find image record, file" << config.fileName;
        return;
    }

    QDir dir = record->layerDir();
    if (!dir.exists()) {
        if (!record->dir().mkpath(dir.absolutePath())) {
            qCWarning(debug) << "Failed creating layer dir";
            return;
        }
    }

    QElapsedTimer timer;
    timer.start();
    QString errorString;
    if (!writeFileAsDataStream(dir.filePath(config.fileName), image, &errorString)) {
        qCWarning(debug) << "Failed writing layer:" << errorString;
        return;
    }
    qCDebug(debug) << "Finished writing file, time" << timer.elapsed() << "ms";
    modified = false;
}

bool ImageRecordLayer::isImageValid()
{
    ensureOpen();
    return image.IsValid();
}

void RecordLayerConfig::setFeature(int index)
{
    feature = index;
    fileName = QString("%1.layer").arg(index);
}
