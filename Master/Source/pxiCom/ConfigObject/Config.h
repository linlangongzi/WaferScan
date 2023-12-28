#pragma once

#include "ConfigObject.h"
#include "User.h"
#include <QDateTime>


/* Config 配置对象基类
 * 所有方法均为线程安全。
 *
 * 提供了写入和读取受监控的配置对象，除了User之外的所有配置对象都应当继承这个类。
 *
 * 在ConfigObject的基础上，Config提供了锁定写入，以及追踪修改时间和用户的功能。
 *
 * 当Set时，当前用户ID和时间会被写入LastModifiedBy和LastModifiedTime
 */
class Config : public ConfigObject
{
    Q_OBJECT
    CONFIG_OBJECT

public:
    typedef ConfigObject::ConfigID ConfigID;

    explicit Config(QObject *parent = 0);
//    virtual ~Config();

    virtual void Set(const QString &key, const QJsonValue &value);
//    void Set(char* key, QJsonValue &value);
    bool IsLocked();
    bool SetLock(bool locked);

protected:
    virtual ConfigObject *NewObject(QObject* parent) const;
    virtual ConfigObject *New(QObject* parent) const = 0;
};
