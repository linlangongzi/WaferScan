#include "LicenseDevice.h"
#include <allrising/Util.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QMetaMethod>
#include <QDebug>

REGISTER_JSON_CONVERTER(LicenseLevel)
REGISTER_JSON_CONVERTER(LicenseDevice)

bool LicenseLevel::IsValid() const
{
    return isValid && level != Invalid;
}

QString LicenseLevel::GetName() const
{
    return EnumToKey(level);
}

void LicenseLevel::SetName(const QString &name)
{
    level = KeyToEnum(name, Invalid);
}

bool LicenseDevice::IsValid() const
{
    return isValid && !deviceId.isEmpty();
}

QDateTime LicenseDevice::GetTime() const
{
    qint64 diff = 0;
    if (deviceTime.isValid() && hostTime.isValid()) {
        diff = hostTime.msecsTo(deviceTime);
    }
    return QDateTime::currentDateTime().addMSecs(diff);
}

LicenseLevel LicenseDevice::GetLevel(LicenseLevel::Level level) const
{
    if (level == LicenseLevel::Invalid) {
        return LicenseLevel();
    }
    for (const LicenseLevel &l: licenseLevels) {
        if (l.level == level) {
            return l;
        }
    }
    return LicenseLevel();
}

bool LicenseDevice::HasValidLevel(LicenseLevel::Level level, bool allowExpired) const
{
    if (!IsValid()) {
        return false;
    }
    QDateTime now = GetTime();
    for (int index = 0; index < licenseLevels.count(); ++index) {
        const LicenseLevel &l = licenseLevels[index];
        if (l.level == level && IsLevelValid(index, now, allowExpired)) {
            return true;
        }
    }
    return false;
}

bool LicenseDevice::IsLevelValid(int index, const QDateTime &time, bool allowExpired) const
{
    QDateTime now = time.isValid() ? time : GetTime();
    const LicenseLevel &level = licenseLevels.value(index);
    return level.isValid && (allowExpired || !level.expire.isValid() || level.expire >= now);
}

QString LicenseDevice::GetLevelStatus(int index) const
{
    if (!IsValid()) {
        return tr("未激活");
    }
    const LicenseLevel &level = licenseLevels.value(index);
    if (!level.isValid) {
        return tr("未激活");
    }
    if (level.expire.isValid() && level.expire < GetTime()) {
        return tr("已过期");
    }
    return tr("有效");
}

bool LicenseDevice::operator ==(const LicenseDevice &other) const
{
    if (MetaType::isGadgetsDifferent(*this, other)) {
        return false;
    }
    return driver == other.driver;
}

bool LicenseDevice::operator !=(const LicenseDevice &other) const
{
    return MetaType::isGadgetsDifferent(*this, other);
}
