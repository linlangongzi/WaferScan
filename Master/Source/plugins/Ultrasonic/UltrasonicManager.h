#pragma once

#include "ChannelConfig.h"
#include "Material.h"
#include "Probe.h"
#include "UTChannel.h"
#include "UTInstrument.h"
#include "UltrasonicGlobal.h"
#include <QHash>
#include <QMap>
#include <QObject>
#include <functional>

class MathExpression
{
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString expression MEMBER expression)

public:
    QString name;
    QString expression;
};

class ULTRASONIC_EXPORT UltrasonicManager : public QObject
{
    Q_OBJECT

public:
    explicit UltrasonicManager(QObject *parent = nullptr);
    ~UltrasonicManager();

    /*! 返回UltrasonicManager单例, 这个接口仅用于连接相关信号 */
    static UltrasonicManager *instance();
    static QMap<int, UTSetup> setups();

    static IInstrumentDriver *createDriver(const QString &name);
    static QStringList availableDriverNames();

    static QList<UTInstrument *> instruments();
    static UTInstrument *instrument(int index);
    static UTInstrument *instrument(const QString &name);
    static UTInstrument *createInstrument();
    static void removeInstrument(UTInstrument *instrument);

    static int channelCount();
    static QList<UTChannel *> channels();
    static UTChannel *channel(int channelId);
    static UTChannel *channel(const ChannelIdentifier &id);
    static UTSetup channelSetup(int channelId);
    static QString channelName(int channelId);
    static MaterialModel *materialModel();
    static ProbeModel *probeModel();
    static QList<MathExpression> mathExpressions();
    static qreal ampToDecibel(qreal amp, qreal base = 0.8);
    static qreal decibelToAmp(qreal decibel, qreal base = 0.8);
    static QString ampToDecibelText(qreal amp, qreal base = 0.8);
    static QString ampText(qreal amp, bool useDecibel, qreal base = 0.8);
    static bool isValidId(int channelId);
    static bool isValidChannel(const QString &instrumentName, int channel, int cycle);

    bool removeChannel(int channelId);
    UTChannel *createChannel(const ChannelConfig &config);

signals:
    void instrumentsChanged();
    void channelsChanged();
    void mathExpressionsChanged();

public slots:
    void loadConfig();
    void saveConfig();
    void loadLegacyConfig();
    void createLegacyChannels();
    void refreshInstruments();
    void refreshChannels();
    void dispatchFrame(const UTDataFrame &frame);
    void dispatchSegment(const UTDataSegment &segment);
    void reloadChannelProbe();

private:
    QList<UTInstrument *> m_instruments;
    QMap<int, UTChannel *> m_channels;
    MaterialModel *m_materials = nullptr;
    ProbeModel *m_probes = nullptr;
    QList<MathExpression> m_mathExpressions;
    int m_ampPrecision = 2;
};
