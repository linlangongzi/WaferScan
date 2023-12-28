#include "SettingsManager.h"

SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{

}

void SettingsManager::beginGroup(const QString &prefix)
{
    m_settings->beginGroup(prefix);
}

void SettingsManager::endGroup()
{
    m_settings->endGroup();
}

void SettingsManager::beginReadArray(const QString &prefix)
{
    m_settings->beginReadArray(prefix);
}

void SettingsManager::beginWriteArray(const QString &prefix, int size)
{
    m_settings->beginWriteArray(prefix, size);
}

void SettingsManager::endArray()
{
    m_settings->endArray();
}

bool SettingsManager::contains(const QString &key) const
{
    return m_settings->contains(key);
}

void SettingsManager::setValue(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);

    const QString group = m_settings->group();
    const QString fullKey = group.isEmpty() ? key : (group + "/" + key);
    emit valueChanged(fullKey);
}

QVariant SettingsManager::value(const QString &key, const QVariant &defaultValue) const
{
    return m_settings->value(key, defaultValue);
}

void SettingsManager::subscribeValueChange(QObject *obj, const QString &key, SettingsManager::Callback func)
{
    if (!obj || !func) {
        return;
    }
    Subscription sub;
    sub.owner = obj;
    sub.key = key;
    sub.func = func;
    m_keySubscription << sub;
    func();
}

void SettingsManager::subscribeGroupChange(QObject *obj, const QString &group, SettingsManager::Callback func)
{
    if (!obj || !func || group.isEmpty()) {
        return;
    }
    Subscription sub;
    sub.owner = obj;
    sub.key = group;
    sub.func = func;
    m_groupSubscription << sub;
    func();
}

void SettingsManager::setSettings(QSettings *settings)
{
    m_settings = settings;
}

void SettingsManager::objectDestroyed(QObject *obj)
{
    std::remove_if(m_keySubscription.begin(), m_keySubscription.end(), [=] (const Subscription &sub) {
        return sub.owner == obj;
    });
    std::remove_if(m_groupSubscription.begin(), m_groupSubscription.end(), [=] (const Subscription &sub) {
        return sub.owner == obj;
    });
}
