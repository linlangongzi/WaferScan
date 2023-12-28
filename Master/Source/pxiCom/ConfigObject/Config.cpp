#include "Config.h"

Config::Config(QObject *parent) :
    ConfigObject(parent)
{
    SetRaw("CreatedBy", FromObjectID(User::CurrentUserID()));
    SetRaw("CreatedTime", QDateTime::currentDateTime().toString(Qt::ISODate));
}

void Config::Set(const QString &key, const QJsonValue &value)
{
    if (IsLocked())
    {
        qWarning("Access denied.");
        return;
    }
    ConfigID userID = User::CurrentUserID();
    SetRaw("LastModifiedBy", FromObjectID(userID));
    SetRaw("LastModifiedTime", QDateTime::currentDateTime().toString(Qt::ISODate));
    SetRaw(key, value);
}

bool Config::IsLocked()
{
    if (Get("Locked").toBool())
    {
        return true;
    }
    return false;
}

bool Config::SetLock(bool locked)
{
    if (IsLocked())
    {
        if (locked)
        {

        }
        else
        {

        }
    }
    else
    {
        if (locked)
        {

        }
        else
        {

        }
    }
    return true;
}

ConfigObject *Config::NewObject(QObject *parent) const
{
    ConfigObject *obj = New(parent);
    if (obj->Get("Type").isNull())
    {
        qFatal("Config::New Type not exist.");
        return NULL;
    }
    return obj;
}
