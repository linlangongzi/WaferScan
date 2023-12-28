#pragma once

#include <QObject>
#include "LicenseDevice.h"

class LicenseDriver : public QObject
{
    Q_OBJECT

public slots:
    virtual void init() {}
    virtual LicenseDeviceList enumerate() = 0;
    virtual void setCredential(const QString &user, const QString &password);
    virtual QString update(const LicenseDevice &newDevice);
    virtual QString recharge(const QString &device, const QString &code);
    virtual QString generateRecharge(const QString &deviceId, const LicenseLevel &level);
};

inline void LicenseDriver::setCredential(const QString &user, const QString &password)
{
    Q_UNUSED(user);
    Q_UNUSED(password);
}

inline QString LicenseDriver::update(const LicenseDevice &newDevice)
{
    Q_UNUSED(newDevice);
    return tr("不支持");
}

inline QString LicenseDriver::recharge(const QString &device, const QString &code)
{
    Q_UNUSED(device);
    Q_UNUSED(code);
    return tr("不支持");
}

inline QString LicenseDriver::generateRecharge(const QString &deviceId, const LicenseLevel &level)
{
    Q_UNUSED(deviceId);
    Q_UNUSED(level);
    return tr("不支持");
}
