#pragma once

#include <QMutex>
#include <QPointer>
#include <QCryptographicHash>
#include "ConfigObject.h"
#include "ConfigManager.h"

/* User是用户对象，提供用户信息查询，用户切换，用户权限查询，用户密码验证等服务。
 * User继承ConfigObject，所有方法均为线程安全。
 */
class User : public ConfigObject
{
    Q_OBJECT
    CONFIG_OBJECT

public:
    typedef ConfigObject::ConfigID ConfigID;
    /* 获取当前用户指针 */
    static User *CurrentUser();
    /* 获取当前用户ID */
    static ConfigID CurrentUserID();
    static void SwitchUser(User *user);

    /* 重载Get和Set，具有权限控制 */
    virtual const QJsonValue Get(const QString &key);
    virtual void Set(const QString &key, const QJsonValue &value);

    /* 设置密码。密码将用密文存储，密文无法恢复为原文 */
    void SetPassword(const QString &password);

    /* 检测一个密码是否与该用户密码相同 */
    bool ChallengePassword(const QString &password);

    /* 检查该用户是否具有特定的权限，参数为权限名 */
    bool HasPrivilege(const QString &key);

    /* 对密码进行加密，传入明文，返回密文 */
    static const QString Encryption(const QString &password, ConfigID id);

protected:
    User(QObject * parent = NULL);

    /* 工厂方法，构造一个新的User，返回ConfigObject指针 */
    virtual ConfigObject *NewObject(QObject* parent) const;

    virtual void ManagerLoaded(ConfigManager *manager);

private:
    /* 全局唯一的当前用户，受mutex锁定控制 */
    static QPointer<User> currentUser;
    static QMutex mutex;

    /* 内部对密码加密使用的默认算法 */
    static const QCryptographicHash::Algorithm ENCRYPT_ALGORITHM = QCryptographicHash::Sha1;
    static const QString DEFAULT_ADMIN_PASSWORD;
};
