#include "ImageRecord.h"
#include <allrising/QtExtend.h>
#include <QElapsedTimer>
#include <QLoggingCategory>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UTDataHeader.h>
#include <QMessageBox>
#include <QSaveFile>

static const int invalidBlock = -1;
static Q_LOGGING_CATEGORY(debug, "image.record");
static const char HOLE_FILE[] = "waveform.hole";
static const char WAVEFORM_MAP[] = "waveform.map";
static const char WAVEFORM_CHUNK[] = "waveform-%1.bin";
static const bool converterRecordConfig = registerJsonConverter<RecordConfig>();
static const bool converterRecordLayerConfig = registerJsonConverter<RecordLayerConfig>();

static const bool registerImageRecord = IDocument::registerPartFactory(&ImageRecord::staticMetaObject,
                                                                       "ImageRecord",
                                                                       [](){return new ImageRecord;});

ImageRecord::ImageRecord(QObject *parent) :
    IDocumentPart(parent)
{
    startTimer(1000);
}

bool ImageRecord::open(QIODevice::OpenMode mode)
{
    bool ok = IDocumentPart::open(mode);
    if (!ok) {
        return false;
    }
    loadJson();
    return ok;
}

RecordConfig ImageRecord::recordConfig() const
{
    return m_recordConfig;
}

void ImageRecord::setRecordConfig(const RecordConfig &config)
{
    if (m_recordConfig == config) {
        return;
    }
    qDeleteAll(m_layers);
    m_layers.clear();

    m_recordConfig = config;
    for (auto &layerConfig: m_recordConfig.layers) {
        layerConfig.featureName = config.header.featureName(layerConfig.feature);
        createLayer(layerConfig);
    }

    loadHoleFile();
    notifyModified();

    emit configUpdated(m_recordConfig);
}

QDir ImageRecord::layerDir() const
{
    return QDir(dir().filePath(recordBaseName()));
}

void ImageRecord::setSetups(const QHash<int, UTSetup> &setups)
{
    m_setups = setups;
}

UTImage ImageRecord::image(int layerIndex)
{
    if (!isValidIndex(layerIndex) || !m_layers[layerIndex]->isImageValid()) {
        return UTImage();
    }
    return m_layers[layerIndex]->image;
}

UTSetup ImageRecord::setup(int channelId) const
{
    return m_setups.value(channelId);
}

QPointF ImageRecord::origin() const
{
    return QPointF();
}

CDataValue ImageRecord::featureAt(int layerIndex, const QPoint &point)
{
    if (!isValidIndex(layerIndex)) {
        return CDataValue();
    }

    ImageRecordLayer *layer = m_layers[layerIndex];
    if (!isLayerReadable(layer, point)) {
        return CDataValue();
    }
    return layer->image.asFloat(point);
}

CDataFrame ImageRecord::featuresAt(const QPoint &point)
{
    CDataFrame features;
    if (!isValidPoint(point)) {
        return features;
    }

    for (ImageRecordLayer *layer: m_layers) {
        if (layer && layer->isImageValid()) {
            features << layer->image.asFloat(point);
        } else {
            features << CDataValue();
        }
    }
    return features;
}

ADataFrame ImageRecord::waveformAt(const QPoint &point)
{
    if (m_recordConfig.hasWaveform && m_ascanLayer == Q_NULLPTR) {
        createAscanLayer();
    }
    if (!isLayerReadable(m_ascanLayer, point)) {
        return ADataFrame();
    }

    int block = m_ascanLayer->image.at<int>(point.x(), point.y());
    if (block == invalidBlock) {
        return ADataFrame();
    }
    return readBlock(block);
}

UTDataFrame ImageRecord::dataFrameAt(const QPoint &point)
{
    if (!isValidPoint(point)) {
        qCDebug(debug) << "Out of image size: " << point;
        return UTDataFrame();
    }
    return UTDataFrame(m_recordConfig.header, featuresAt(point), waveformAt(point));
}

UTImage ImageRecord::sliceImage(const QPoint &start, const QPoint &step, int total)
{
    QElapsedTimer timer;
    timer.start();
    UTImage image(QSize(m_recordConfig.blockSize / sizeof(ADataValue), total), 0, QMetaType::UShort);
    QPoint point = start;
    for (int line = 0; line < total; ++line) {
        ADataFrame frame = waveformAt(point);
        for (int index = 0; index < frame.count(); ++index) {
            image.at<ADataValue>(index, line) = frame[index];
        }
        point += step;
    }
    qCDebug(debug) << "Finishing slice image" << timer.elapsed() << "ms";
    return image;
}

void ImageRecord::setFeatureAt(int layerIndex, const QPoint &point, CDataValue value)
{
    if (!isValidIndex(layerIndex)) {
        return;
    }

    ImageRecordLayer *layer = m_layers[layerIndex];
    if (!isLayerReadable(layer, point) || !isWritable() || !layer->config.enable) {
        return;
    }
    layer->image.SetPointFast(point.x(), point.y(), value);
    layer->modified = true;
    m_modified.setBit(layerIndex, true);
    notifyModified();
}

void ImageRecord::setFeaturesAt(const QPoint &point, const CDataFrame &frame)
{
    if (!isValidPoint(point)) {
        return;
    }
    for (int index = 0; index < m_layers.count(); ++index) {
        ImageRecordLayer *layer = m_layers[index];
        int featureIndex = m_recordConfig.layers.value(index).feature;
        if (!layer->isImageValid()
            || !isWritable()
            || uint(featureIndex) >= uint(frame.size())
            || !layer->config.enable) {
            continue;
        }
        layer->image.SetPoint(point.x(), point.y(), frame[featureIndex]);
        layer->modified = true;
        m_modified.setBit(index, true);
    }
    notifyModified();
}

void ImageRecord::setWaveformAt(const QPoint &point, const ADataFrame &frame)
{
    if (!isLayerReadable(m_ascanLayer, point)) {
        return;
    }

    int block = m_ascanLayer->image.asInt(point.x(), point.y());
    if (block == invalidBlock) {
        block = allocateBlock();
    }
    m_ascanLayer->image.asInt(point.x(), point.y()) = block;
    m_ascanLayer->modified = true;
    writeBlock(block, frame);
}

void ImageRecord::removeWaveformAt(const QPoint &point)
{
    if (!isLayerReadable(m_ascanLayer, point) || !isWritable()) {
        return;
    }

    int block = invalidBlock;
    qSwap(block, m_ascanLayer->image.asInt(point.x(), point.y()));
    if (block != invalidBlock) {
        freeBlock(block);
    }
}

ImageOptions ImageRecord::imageOptions() const
{
    return m_recordConfig.imageOptions;
}

bool ImageRecord::addLayer(const RecordLayerConfig &config)
{
    if (!(openMode() & QIODevice::WriteOnly)) {
        qCDebug(debug) << "Can not add layer in an unable to write dir" << dir();
        return false;
    }

    m_recordConfig.layers << config;
    createLayer(config);
    notifyModified();

    return true;
}

bool ImageRecord::removeLayer(int index)
{
    if (uint(index) >= uint(m_layers.size()) || uint(index) >= uint(m_recordConfig.layers.size())) {
        qCDebug(debug, "Failed remove layer: index %d not exist", index);
        return false;
    }

    delete m_layers.takeAt(index);
    m_recordConfig.layers.removeAt(index);
    m_modified = QBitArray(m_layers.size(), true);
    notifyModified();

    return true;
}

RecordLayerConfig ImageRecord::layerConfig(int index) const
{
    return m_recordConfig.layers.value(index);
}

void ImageRecord::setLayerConfig(int index, const RecordLayerConfig &config)
{
    if (!isValidIndex(index)) {
        return;
    }
    m_recordConfig.layers[index] = config;

    ImageRecordLayer *layer = m_layers[index];
    layer->config = config;
    m_modified.setBit(index, true);

    notifyModified();
}

bool ImageRecord::hasAscanLayer()
{
    if (m_ascanLayer == Q_NULLPTR) {
        return false;
    }
    return true;
}

void ImageRecord::createAscanLayer()
{
    if (m_ascanLayer != Q_NULLPTR) {
        return;
    }

    m_ascanLayer = new ImageRecordLayer(this);
    m_ascanLayer->config.fileName = WAVEFORM_MAP;
    m_ascanLayer->image = UTImage(m_recordConfig.size, invalidBlock, QMetaType::Int);
    m_ascanLayer->ensureOpen();
    m_recordConfig.hasWaveform = true;
}

/*!
 * \brief ImageRecord::removeAscanLayer
 * 谨慎使用, 使用前要弹窗确认
 */
void ImageRecord::removeAscanLayer()
{
    if (!m_recordConfig.hasWaveform) {
        return;
    }
    m_blockFiles.clear();

    int fileIndexMax = m_nextBlock - 1 / m_recordConfig.fileSizeMax;
    layerDir().remove(HOLE_FILE);                                               // 空洞表文件
    layerDir().remove(WAVEFORM_MAP);                                            // block查询表文件
    for (int index = 0; index <= fileIndexMax; ++index) {                       // 数据文件
        layerDir().remove(QString(WAVEFORM_CHUNK).arg(index));
    }

    delete m_ascanLayer;
    m_ascanLayer = Q_NULLPTR;

    m_holes.clear();
    m_nextBlock = 0;
    m_recordConfig.hasWaveform = false;
}

void ImageRecord::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    emit imagesUpdated(m_modified);
    m_modified.fill(false, m_layers.size());
}

bool ImageRecord::loadJson()
{
    QJsonObject obj = readJson().toObject();
    if (!obj.contains("RecordConfig") || !obj.contains("Setups")) {
        return false;
    }

    setRecordConfig(convertFromJson<RecordConfig>(obj["RecordConfig"]));
    m_setups = convertFromJson<decltype (m_setups)>(obj["Setups"]);
    m_nextBlock = obj["NextBlock"].toInt();

    return true;
}

void ImageRecord::saveHoleFile()
{
    if (m_holes.isEmpty()) {                          // 无 空洞||A扫 删除空洞表文件
        layerDir().remove(HOLE_FILE);
        return;
    }
    writeFileAsDataStream(layerDir().filePath(HOLE_FILE), m_holes, Q_NULLPTR);
    qCInfo(debug) << "Saved holes file" << m_holes.size();
}

void ImageRecord::loadHoleFile()
{
    m_holes.clear();
    readFileAsDataStream(layerDir().filePath(HOLE_FILE), m_holes, Q_NULLPTR);
    qCInfo(debug) << "Loaded holes file" << m_holes.size();
}

void ImageRecord::saveJson()
{
    QJsonObject obj;
    obj["RecordConfig"] = convertToJson(m_recordConfig);
    obj["Setups"] = convertToJson(m_setups);
    obj["NextBlock"] = QJsonValue(m_nextBlock);

    writeJson(obj);
}

bool ImageRecord::isValidIndex(int index) const
{
    if (uint(index) >= uint(m_layers.count()) ||
        uint(index) >= uint(m_recordConfig.layers.count())) {
        return false;
    }
    return true;
}

bool ImageRecord::isValidPoint(const QPoint &point) const
{
    const QSize &size = m_recordConfig.size;
    return (uint(point.x()) < uint(size.width())) && (uint(point.y()) < uint(size.height()));
}

bool ImageRecord::isLayerReadable(ImageRecordLayer *layer, const QPoint &point)
{
    if (layer == Q_NULLPTR
        || !layer->isImageValid()
        || !isValidPoint(point)) {
        qCDebug(debug) << "layer is error";
        return false;
    }
    return true;
}

void ImageRecord::createLayer(const RecordLayerConfig &config)
{
    QString layerPath = dir().filePath(recordBaseName());
    QDir layerDir;
    if (!layerDir.exists(layerPath)) {
        layerDir.mkpath(layerPath);
    }
    ImageRecordLayer *layer = new ImageRecordLayer(this);
    layer->config = config;
    m_layers << layer;

    m_modified.resize(m_layers.size());
    m_modified.setBit(m_layers.size() - 1, true);
}

QString ImageRecord::layerFilePath(const QString &fileName)
{
    return QDir(dir().filePath(recordBaseName())).filePath(fileName);
}

QString ImageRecord::recordBaseName() const
{
    QFileInfo info(fileName());
    return info.baseName();
}

ADataFrame ImageRecord::readBlock(int block)
{
    ADataFrame frame;
    QFile *file = blockFile(block);
    if (file == Q_NULLPTR) {
        return frame;
    }

    const auto posPair = blockPosition(block);
    file->seek(posPair.second);
    QDataStream s(file);
    int size = m_recordConfig.blockSize / sizeof(ADataValue);
    frame.reserve(size);
    ADataValue data;
    for (; size; --size) {
        s >> data;
        frame << data;
    }

    return frame;
}

void ImageRecord::writeBlock(int block, const ADataFrame &frame)
{
    QFile *file = blockFile(block);
    if (file == Q_NULLPTR) {
        return;
    }

    const auto posPair = blockPosition(block);
    file->seek(posPair.second);
    QDataStream s(file);
    int size = m_recordConfig.blockSize / sizeof(ADataValue);
    for (int index = 0; index < size; ++index) {
        s << frame.value(index);
    }
}

int ImageRecord::allocateBlock()
{
    if (!m_holes.isEmpty()) {
        return m_holes.takeLast();
    }
    return m_nextBlock++;
}

void ImageRecord::freeBlock(int block)
{
    if (block == m_nextBlock - 1) {
        m_nextBlock--;

        int fileIndexBefore = (m_nextBlock - 1) / m_recordConfig.fileSizeMax;
        while (m_holes.contains(m_nextBlock - 1)) {                             // 以链表已经排好升序为前提
            if (m_holes.removeOne(m_nextBlock - 1)) {
                m_nextBlock--;
            }
        }

        int fileIndexAfter = (m_nextBlock - 1) / m_recordConfig.fileSizeMax;
        for (int index = fileIndexBefore; index > fileIndexAfter; --index) {
            if (m_blockFiles.contains(index)) {
                delete m_blockFiles.take(index);
            }
            layerDir().remove(QString(WAVEFORM_CHUNK).arg(index));
        }
    } else {
        m_holes.append(block);
    }
}

/*!
 * \brief ImageRecord::ascanFileIndex
 * \param block:   -1无效 0开始有效
 * \return fileId: -1无效 0开始有效
 */
QPair<int, qint64> ImageRecord::blockPosition(int block)
{
    if (m_recordConfig.fileSizeMax == 0) {
        return QPair<int, qint64>(-1, 0);
    }
    int fileIndex = block / m_recordConfig.fileSizeMax;
    qint64 pos = qint64(block % m_recordConfig.fileSizeMax) * m_recordConfig.blockSize;
    return qMakePair(fileIndex, pos);
}

QFile *ImageRecord::blockFile(int block)
{
    const auto posPair = blockPosition(block);
    int fileIndex = posPair.first;
    if (fileIndex < 0) {
        return Q_NULLPTR;
    }

    if (!m_blockFiles.contains(fileIndex)) {
        QString fileName = layerFilePath(QString(WAVEFORM_CHUNK).arg(fileIndex));
        QFile *file = new QFile(fileName);
        if (!file->open(openMode())) {
            qCInfo(debug) << "Failed open file" << fileName;
            delete file;
            return Q_NULLPTR;
        }
        m_blockFiles.insert(fileIndex, file);
    }
    return m_blockFiles[fileIndex];
}

bool ImageRecord::save()
{
    qCDebug(debug) << "Saving record" << fileName() << "with" << m_layers.count() << "layers";
    saveJson();

    for (ImageRecordLayer *layer: m_layers) {
        if (layer->modified) {
            layer->saveFile();
        }
    }

    if (m_ascanLayer && m_ascanLayer->modified) {
        m_ascanLayer->saveFile();
    }

    saveHoleFile();

    return true;
}
