#pragma once

#include <Plot/PlotConfig.h>
#include <QApplication>
#include <Ars1000/PipeDataDef.h>
#include <Imaging/ImageOptions.h>

class PipeChannelConfig
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(PipeChannelConfig)

    Q_PROPERTY(bool enable MEMBER enable)
    Q_PROPERTY(qreal baseOffset MEMBER baseOffset)
    Q_PROPERTY(QVector<qreal> gateOffset MEMBER gateOffset)

    Q_PROPERTY(int index MEMBER index)
    Q_PROPERTY(int probeBoxIndex MEMBER probeBoxIndex)
    Q_PROPERTY(int probeNo MEMBER probeNo)
    Q_PROPERTY(int channelNo MEMBER channelNo)
    Q_PROPERTY(int hardChannel MEMBER hardChannel)
    Q_PROPERTY(int softChannel MEMBER softChannel)
    Q_PROPERTY(int muxCardId MEMBER muxCardId)
    Q_PROPERTY(int muxInput MEMBER muxInput)
    Q_PROPERTY(int muxOutput MEMBER muxOutput)
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString probeName MEMBER probeName)
    Q_PROPERTY(bool isFlaw MEMBER isFlaw)
    Q_PROPERTY(qreal angleCorrect MEMBER angleCorrect)

public:
    QString channelName() const;
    QString channelDescription() const;

    DECLARE_COMPARE_OPERATORS(PipeChannelConfig)

    bool enable = true;
    qreal baseOffset = 0;
    QVector<qreal> gateOffset;

    int index = 0;
    int probeBoxIndex = 0;
    int probeNo = 0;
    int channelNo = 0;
    int hardChannel = 0;
    int softChannel = 0;
    int muxCardId = 0;
    int muxInput = 0;
    int muxOutput = 0;
    QString name;
    QString probeName;
    bool isFlaw = false;
    qreal angleCorrect = 0; // 单位:度
};

class PipeModeConfig
{
    Q_GADGET

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QList<PlotView> plots MEMBER plots)
//    Q_PROPERTY(QList<MotionAxisList> imageChannels MEMBER imageChannels)
    Q_PROPERTY(QList<ImageOptions> images MEMBER images)

public:
    QString name() const
    {
        return m_name;
    }
    void setName(QString name)
    {
        m_name = name;
    }

    DECLARE_COMPARE_OPERATORS(PipeModeConfig)
    static PipeModeConfig fromJson(const QJsonValue &json);

    QString m_name;
    QList<PlotView> plots;
//    QList<MotionAxisList> imageChannels;
    QList<ImageOptions> images;
};

class PipeGlobalConfig
{
    Q_GADGET

    Q_PROPERTY(qreal intervalX MEMBER intervalX)
    Q_PROPERTY(qreal intervalR MEMBER intervalR)
    Q_PROPERTY(qreal intervalPlot MEMBER intervalPlot)
    Q_PROPERTY(bool autoChannelOffset MEMBER autoChannelOffset)
    Q_PROPERTY(bool autoAngleCorrect MEMBER autoAngleCorrect)
    Q_PROPERTY(bool autoDeadZone MEMBER autoDeadZone)
    Q_PROPERTY(qreal deadZoneLeft MEMBER deadZoneLeft)
    Q_PROPERTY(qreal deadZoneRight MEMBER deadZoneRight)
    Q_PROPERTY(QList<PipeChannelConfig> channels MEMBER channels)
    Q_PROPERTY(QList<PipeModeConfig> modes MEMBER modes)

public:
    QList<qreal> plotOffset(int featureIndex) const;
    QList<PlotView> plots(int mode, int groupMask, const PipeDetect::OrderInfo &info) const;
    QVector<QVector<float> > imageOffset() const;
    QList<ImageOptions> imageOptions(int modeId, int groupMask, PipeDetect::OrderInfo orderInfo) const;
    MotionAxisList enabledChannels(int groupMask) const;
    QList<qreal> angleCorrect() const;

    DECLARE_COMPARE_OPERATORS(PipeGlobalConfig)

    qreal intervalX = 10;
    qreal intervalR = 10;
    qreal intervalPlot = 10;
    bool autoChannelOffset = true;
    bool autoAngleCorrect = true;
    bool autoDeadZone = true;
    qreal deadZoneLeft = 0;
    qreal deadZoneRight = 0;
    QList<PipeChannelConfig> channels;
    QList<PipeModeConfig> modes;
};
