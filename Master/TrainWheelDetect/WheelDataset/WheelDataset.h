#ifndef WHEELDATASET_H
#define WHEELDATASET_H

#include <QHash>
#include <QObject>
#include <QDateTime>

#include <Ultrasonic/UTStreamFile.h>
#include <Core/IDocumentPart.h>
#include <allrising/MetaType/MetaType.h>
#include <allrising/MetaType/JsonConverter.h>
#include <UTInstrumentDriver/PXI100/DataDef.h>

#define MAX_TRAIN 100
#define BSCAN_LINES_ZERO 5.0

class MovingAverage
{
public:
    int size() const;
    void add(qreal value);
    qreal next(qreal value);
    qreal append(qreal value);

private:
    QList<qreal> window;
    qreal sum = 0;
    int count = 0;
};

class WheelDataset : public IDocumentPart
{
    Q_OBJECT

    struct ChannelData
    {
        int location = -1;
        UTSharedDataSegment data;
        UTSharedDataFrame extreme;
    };

public:
    explicit WheelDataset(QObject *parent = 0);
    int wheelSetCount() const {return wheelSetMax;}
    bool clearData();
    void add(UTSharedDataFrame frame);
    UTSharedDataSegment calculateDefects();
    UTSharedDataSegment wheelFrames(int wheelId);
    UTSharedDataFrame extremeFrame(qint64 channelId);
    UTSharedDataSegment channelFrames(qint64 channelId);
    QHash<qint64, ChannelData> allData() const {return data;}
    void modifyData(qint64 channelId);

signals:
    void judgeReady();
    void wheelSetMaxChanged(int countMax);
    void channelDataUpdated(QSet<qint64> channels);

private:
    void calculateExtremeFrame();
    void setWheelSetMax(int countMax);

private:
    int wheelSetMax = 0;                                                        // 最大轮组
    QSet<qint64> dirtyChannels;                                                 // <ChannelId>
    QHash<qint64, ChannelData> data;                                            // <ChannelId, ChannelData>
    UTSharedDataSegment unusedData;

    friend class ControlPanel;
};

#endif // WHEELDATASET_H
