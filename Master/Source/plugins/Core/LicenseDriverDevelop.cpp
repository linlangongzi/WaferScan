#include "LicenseDriverDevelop.h"
#include <allrising/Util.h>
#include "LicenseManager.h"
#include <QApplication>

const int licenseDriverDev = LicenseManager::registerDriver(new LicenseDriverDevelop);

void LicenseDriverDevelop::init()
{
    LicenseDevice dev;
    dev.isValid = true;
    dev.deviceId = "Develop License";
    dev.deviceType = tr("开发环境授权");
    dev.deviceDesc = qApp->property("Revision").toString();
    LicenseLevel level;
    level.isValid = true;

    for (auto l: EnumValueList<LicenseLevel::Level>()) {
        if (l != LicenseLevel::Invalid) {
            level.level = l;
            dev.licenseLevels << level;
        }
    }

    licenseDev = LicenseDeviceList() << dev;
}

LicenseDeviceList LicenseDriverDevelop::enumerate()
{
    return licenseDev;
}
