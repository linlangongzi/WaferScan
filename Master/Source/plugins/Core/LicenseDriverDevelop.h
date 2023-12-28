#pragma once

#include "LicenseDriver.h"

class LicenseDriverDevelop : public LicenseDriver
{
    Q_OBJECT

public slots:
    virtual void init() override;
    virtual LicenseDeviceList enumerate() override;

private:
    LicenseDeviceList licenseDev;
};
