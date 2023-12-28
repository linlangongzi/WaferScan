#pragma once

#include <QObject>
#include <QCoreApplication>
#include <allrising/Util.h>
#include "UltrasonicGlobal.h"

class ULTRASONIC_EXPORT InstrumentConfig
{
    Q_GADGET

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString address MEMBER address)
    Q_PROPERTY(QString deviceType MEMBER deviceType)
    Q_PROPERTY(int channelCount MEMBER channelCount)

public:
    DECLARE_COMPARE_OPERATORS(InstrumentConfig)

    QString name;
    QString address;
    QString deviceType;
    int channelCount = 0;
};

class ULTRASONIC_EXPORT EncoderSetup
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(EncoderSetup)

    Q_PROPERTY(Type type MEMBER type)
    Q_PROPERTY(int divider MEMBER divider)
    Q_PROPERTY(bool reverse MEMBER reverse)
    Q_PROPERTY(qreal offset MEMBER offset)
    Q_PROPERTY(qreal repeatCycle MEMBER repeatCycle)
    Q_PROPERTY(RepeatType repeatType MEMBER repeatType)

public:
    enum Type {
        None,
        Quadrature,                                                             /*!< 相差 */
        StepDirection,                                                          /*!< 脉冲+方向 */
    };
    Q_ENUM(Type)

    enum RepeatType {
        NoRepeat,
        RepeatZero,
        Repeat
    };
    Q_ENUM(RepeatType)

    DECLARE_COMPARE_OPERATORS(EncoderSetup)

    static bool registerType();

public:
    Type type = None;
    int divider = 1;
    bool reverse = false;
    qreal offset = 0;
    qreal repeatCycle = 0;
    RepeatType repeatType = NoRepeat;
    int axis = -1;
};

class ULTRASONIC_EXPORT InstrumentSetup
{
    Q_GADGET

    Q_PROPERTY(int prf MEMBER prf)
    Q_PROPERTY(int transmitVoltage MEMBER transmitVoltage)
    Q_PROPERTY(PulseMode pulseMode MEMBER pulseMode)
    Q_PROPERTY(QList<EncoderSetup> encoders MEMBER encoders)

public:
    enum AcquireMode {
        AcquireNone,
        AcquireFeature,
        AcquireWave,
    };
    Q_ENUM(AcquireMode)

    enum PulseMode {
        FreeRunning,
        ExternalTrigger,
    };
    Q_ENUM(PulseMode)

    DECLARE_COMPARE_OPERATORS(InstrumentSetup)

public:
    int prf = 100;
    int transmitVoltage = 0;
    PulseMode pulseMode = FreeRunning;
    QList<EncoderSetup> encoders;
    QVariantMap variables;
};

class InstrumentInfo
{
    Q_GADGET

    Q_PROPERTY(QString model MEMBER model)
    Q_PROPERTY(QString hardwareVersion MEMBER hardwareVersion)
    Q_PROPERTY(QString softwareVersion MEMBER softwareVersion)
    Q_PROPERTY(QString fpgaVersion MEMBER fpgaVersion)
    Q_PROPERTY(QString serialNumber MEMBER serialNumber)
    Q_PROPERTY(QString manufacturer MEMBER manufacturer)
    Q_PROPERTY(int channelCount MEMBER channelCount)
    Q_PROPERTY(int gateCount MEMBER gateCount)
    Q_PROPERTY(int encoderCount MEMBER encoderCount)

    Q_PROPERTY(double maxGain MEMBER maxGain)
    Q_PROPERTY(int maxPrf MEMBER maxPrf)
    Q_PROPERTY(bool supportAlterSource MEMBER supportAlterSource)
    Q_PROPERTY(bool supportAdjustTransmitMode MEMBER supportAdjustTransmitMode)
    Q_PROPERTY(bool supportImpedance MEMBER supportImpedance)
    Q_PROPERTY(bool supportPulseType MEMBER supportPulseType)
    Q_PROPERTY(bool supportPulseWidth MEMBER supportPulseWidth)
    Q_PROPERTY(bool supportSurfaceTracking MEMBER supportSurfaceTracking)

public:
    DECLARE_COMPARE_OPERATORS(InstrumentInfo)

    QString model;
    QString hardwareVersion;
    QString softwareVersion;
    QString fpgaVersion;
    QString serialNumber;
    QString manufacturer;
    QList<int> voltage;
    int channelCount = 0;
    int gateCount = 0;
    int encoderCount = 0;

    double maxGain = 110.0;                                                     // 最大增益  单位dB
    int maxPrf = 8000;                                                          // 最大重复频率  单位Hz
    bool supportAlterSource =  false;                                           // 支持关联检测
    bool supportAdjustTransmitMode = true;                                      // 支持收发模式调节
    bool supportImpedance = true;                                               // 支持输入阻抗调节
    bool supportEnergy = true;                                                  // 支持能量切换
    bool supportFilter = true;                                                  // 支持滤波
    bool supportPulseType = false;                                              // 支持尖脉冲/方波激励切换
    bool supportPulseWidth = false;                                             // 支持脉宽调节
    bool supportSurfaceTracking = false;                                        // 支持表面跟踪
};

class InstrumentStatus
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(InstrumentStatus)

    Q_PROPERTY(State state MEMBER state)
    Q_PROPERTY(QString errorString MEMBER errorString)

public:
    enum State {
        Connected,
        Disconnected,
        Initializing
    };
    Q_ENUM(State)

    DECLARE_COMPARE_OPERATORS(InstrumentStatus)

    QString text() const;

    State state = Disconnected;
    QString errorString;
};
