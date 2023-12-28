#pragma once

#include "ImageRecordLayer.h"
#include "UTImage.h"
#include <Core/IDocument.h>
#include <Core/IDocumentPart.h>
#include <QCache>
#include <QLinkedList>
#include <QObject>
#include <Ultrasonic/UTData.h>
#include <Ultrasonic/UTSetup.h>
#include <allrising/Util.h>

class IMAGING_EXPORT RecordConfig
{
    Q_GADGET

    Q_PROPERTY(UTDataHeader header MEMBER header)
    Q_PROPERTY(QString layerDir MEMBER layerDir)
    Q_PROPERTY(QSize size MEMBER size)
    Q_PROPERTY(QList<RecordLayerConfig> layers MEMBER layers)
    Q_PROPERTY(ImageOptions imageOptions MEMBER imageOptions)
    Q_PROPERTY(int blockSize MEMBER blockSize)
    Q_PROPERTY(int fileSizeMax MEMBER fileSizeMax)
    Q_PROPERTY(bool hasWaveform MEMBER hasWaveform)

public:
    DECLARE_COMPARE_OPERATORS(RecordConfig)

    UTDataHeader header;
    QString layerDir;
    QSize size;
    QList<RecordLayerConfig> layers;
    ImageOptions imageOptions;
    int blockSize = 1024;                                                       // 单位 字节   1KB
    int fileSizeMax = 1024 * 1024;                                              // 单位 block 1GB
    bool hasWaveform = false;
};
Q_DECLARE_METATYPE(RecordConfig)

class IMAGING_EXPORT ImageRecord : public IDocumentPart
{
    Q_OBJECT

public:
    explicit ImageRecord(QObject *parent = nullptr);

    inline void notifyModified()
    {
        qobject_cast<IDocument *>(parent())->setModified(true);
    }

    bool open(OpenMode mode) override;

    RecordConfig recordConfig() const;
    void setRecordConfig(const RecordConfig &config);
    QDir layerDir() const;

    void setSetups(const QHash<int, UTSetup> &setups);

    Q_REQUIRED_RESULT UTImage image(int layerIndex);
    UTSetup setup(int channelId) const;
    QPointF origin() const;

    Q_REQUIRED_RESULT CDataValue featureAt(int layerIndex, const QPoint &point);
    Q_REQUIRED_RESULT CDataFrame featuresAt(const QPoint &point);
    Q_REQUIRED_RESULT ADataFrame waveformAt(const QPoint &point);
    Q_REQUIRED_RESULT UTDataFrame dataFrameAt(const QPoint &point);             // 获取C扫图像上指定点对应的A扫数据
    UTImage sliceImage(const QPoint &start, const QPoint &step, int total);

    void setFeatureAt(int layerIndex, const QPoint &point, CDataValue value);
    void setFeaturesAt(const QPoint &point, const CDataFrame &frame);
    void setWaveformAt(const QPoint &point, const ADataFrame &frame);
    void removeWaveformAt(const QPoint &point);

    ImageOptions imageOptions() const;

    bool addLayer(const RecordLayerConfig &config);                             // 供外部调用
    bool removeLayer(int index);
    RecordLayerConfig layerConfig(int index) const;
    void setLayerConfig(int index, const RecordLayerConfig &config);

    bool hasAscanLayer();
    void createAscanLayer();
    void removeAscanLayer();

signals:
    void imagesUpdated(QBitArray layers);
    void configUpdated(const RecordConfig &config);

private:
    void timerEvent(QTimerEvent *event) override;
    void saveJson();
    bool loadJson();

    void saveHoleFile();
    void loadHoleFile();

    bool isValidIndex(int index) const;
    bool isValidPoint(const QPoint &point) const;
    bool isLayerReadable(ImageRecordLayer *layer, const QPoint &point);
    void createLayer(const RecordLayerConfig &config);                          // 不包含写入权限检查，供自身使用
    QString layerFilePath(const QString &fileName);
    QString recordBaseName() const;

    ADataFrame readBlock(int block);
    void writeBlock(int block, const ADataFrame &frame);
    int allocateBlock();
    void freeBlock(int block);
    QPair<int, qint64> blockPosition(int block);                                // <fileId, pos>
    QFile *blockFile(int block);

private:
    bool save() override;

    QBitArray m_modified;
    QHash<int, UTSetup> m_setups;

    int m_nextBlock = 0;
    QLinkedList<int> m_holes;                                                   // QSet<block>
    RecordConfig m_recordConfig;
    ImageRecordLayer *m_ascanLayer = Q_NULLPTR;                                 // 记录block偏移位置
    QCache<int, QFile> m_blockFiles;
    QList<ImageRecordLayer *> m_layers;
};
