#pragma once

#include "LicenseDriver.h"

class LicenseDriverLdap : public LicenseDriver
{
    Q_OBJECT

public slots:
    LicenseDeviceList enumerate() override;
    void setCredential(const QString &user, const QString &pass) override;

private:
    QString userName;
    QString password;
};
