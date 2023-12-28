#pragma once

#include <QCoreApplication>
#include <QDateTime>
#include <QJsonValue>
#include <QObject>
#include <allrising/Util.h>

class QJsonValue;
class LicenseDriver;

class LicenseLevel
{
    Q_GADGET
    Q_PROPERTY(Level level MEMBER level)
    Q_PROPERTY(QDateTime expire MEMBER expire)
    Q_PROPERTY(bool isValid MEMBER isValid)

public:
    enum {
        MaxLevel = 128,
    };
    enum Level {
        Invalid = -1,
        WaferScan,
        Pipe,
        Train,
        WaferScanDev = MaxLevel - 1,
    };
    Q_ENUM(Level)

    Level level;
    QDateTime expire;
    bool isValid = false;

    bool IsValid() const;
    QString GetName() const;
    void SetName(const QString &name);

    DECLARE_COMPARE_OPERATORS(LicenseLevel)
};

class LicenseDevice
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(LicenseDevice)

    Q_PROPERTY(bool isValid MEMBER isValid)
    Q_PROPERTY(QString deviceId MEMBER deviceId)
    Q_PROPERTY(QString deviceType MEMBER deviceType)
    Q_PROPERTY(QString deviceDesc MEMBER deviceDesc)
    Q_PROPERTY(QDateTime deviceTime MEMBER deviceTime)
    Q_PROPERTY(QDateTime hostTime MEMBER hostTime)
    Q_PROPERTY(QString licenseCustomer MEMBER licenseCustomer)
    Q_PROPERTY(QString licenseModel MEMBER licenseModel)
    Q_PROPERTY(QString licenseSerial MEMBER licenseSerial)
    Q_PROPERTY(QString licenseDate MEMBER licenseDate)
    Q_PROPERTY(QList<LicenseLevel> licenseLevels MEMBER licenseLevels)
    Q_PROPERTY(QJsonValue config MEMBER config)

public:
    enum Flag {
        DeviceRecharge = 1 << 0,
        DeviceUpdate = 1 << 1,
        DevicegenerateRecharge = 1 << 2,
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    bool isValid = false;
    QString deviceId;
    QString deviceType;
    QString deviceDesc;
    QDateTime deviceTime;
    QDateTime hostTime;

    QString licenseCustomer;
    QString licenseModel;
    QString licenseSerial;
    QString licenseDate;

    QList<LicenseLevel> licenseLevels;

    QJsonValue config;
    Flags flags;

    bool IsValid() const;
    QDateTime GetTime() const;
    LicenseLevel GetLevel(LicenseLevel::Level level) const;
    bool HasValidLevel(LicenseLevel::Level level, bool allowExpired = false) const;
    bool IsLevelValid(int index, const QDateTime &time = QDateTime(), bool allowExpired = false) const;
    QString GetLevelStatus(int index) const;

    bool CanUpdate() const {return flags & DeviceUpdate;}
    bool CanRecharge() const {return flags & DeviceRecharge;}
    bool CanGenerageRecharge() const {return flags & DevicegenerateRecharge;}

    bool operator ==(const LicenseDevice &other) const;
    bool operator !=(const LicenseDevice &other) const;

private:
    LicenseDriver *driver = nullptr;
    friend class LicenseManager;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(LicenseDevice::Flags)

using LicenseDeviceList = QList<LicenseDevice>;
Q_DECLARE_METATYPE(LicenseDeviceList);
