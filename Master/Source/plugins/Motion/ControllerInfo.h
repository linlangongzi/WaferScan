#pragma once

#include "MotionGlobal.h"
#include <QCoreApplication>
#include <allrising/MetaType/MetaType.h>
#include <QVersionNumber>

namespace Motion {

static const QVersionNumber CONTROLLER_VERSION_REQUIRED(2, 0256);               // 2.0256 后允许获取 featureEnabled
static const QVersionNumber CONTROLLER_VERSION_RECOMMENDED(2, 0256);

enum ControllerFeature
{
    IecRuntime = 1 << 21,
    KinematicRuntime = 1 << 22,
};
Q_DECLARE_FLAGS(ControllerFeatures, ControllerFeature);

class MOTION_EXPORT ControllerInfo
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(ControllerInfo)

    Q_PROPERTY(QString model MEMBER model)
    Q_PROPERTY(QString serial MEMBER serial)
    Q_PROPERTY(QString version MEMBER version)
    Q_PROPERTY(QString fpgaVersion MEMBER fpgaVersion)
    Q_PROPERTY(int axisCount MEMBER axisCount)
    Q_PROPERTY(int servoPeriod MEMBER servoPeriod)
    Q_PROPERTY(int tableSize MEMBER tableSize)
    Q_PROPERTY(int digitalInputCount MEMBER digitalInputCount)
    Q_PROPERTY(int digitalOutputCount MEMBER digitalOutputCount)
    Q_PROPERTY(int digitalIoCount MEMBER digitalIoCount)
    Q_PROPERTY(int analogInputCount MEMBER analogInputCount)
    Q_PROPERTY(int analogOutputCount MEMBER analogOutputCount)
    Q_PROPERTY(bool hasMemoryCard MEMBER hasMemoryCard)
    Q_PROPERTY(int featureEnabled MEMBER featureEnabled)

    Q_PROPERTY(int memoryFree MEMBER memoryFree)
    Q_PROPERTY(int systemError MEMBER systemError)
    Q_PROPERTY(float cpuLoad MEMBER cpuLoad)

public:
    QString featureEnabledText() const;

    QString model;
    QString serial;
    QString version;
    QString fpgaVersion;
    int axisCount = 0;
    int servoPeriod = 0;
    int tableSize = 0;
    int digitalInputCount = 0;
    int digitalOutputCount = 0;
    int digitalIoCount = 0;
    int analogInputCount = 0;
    int analogOutputCount = 0;
    bool hasMemoryCard = false;
    int featureEnabled = 0;

    int memoryFree = 0;
    int systemError = 0;
    float cpuLoad = 0;
};

enum ControllerState
{
    Disconnected,
    Connecting,
    SwitchChannel,
    RequestInfo,
    ConfigureAxis,
    SwitchingMode,
    Normal,
};

class MOTION_EXPORT ControllerStatus
{
    Q_DECLARE_TR_FUNCTIONS(ControllerStatus)

public:
    QString text() const;

    ControllerState state = Disconnected;
    int progress = 0;
    int total = 0;
    int baseCommandId;
};

class MOTION_EXPORT ControllerIoConfig
{
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(bool inverted MEMBER inverted)

public:
    DECLARE_COMPARE_OPERATORS(ControllerIoConfig)

    QString name;
    bool inverted = true;
};

class MOTION_EXPORT ControllerConfig
{
    Q_GADGET

    Q_PROPERTY(QString host MEMBER host)
    Q_PROPERTY(QList<ControllerIoConfig> ioConfig MEMBER ioConfig)
    Q_PROPERTY(bool fastProgram MEMBER fastProgram)

public:
    QString host;
    QList<ControllerIoConfig> ioConfig;
    bool fastProgram = false;
};

}   // namespace Motion
