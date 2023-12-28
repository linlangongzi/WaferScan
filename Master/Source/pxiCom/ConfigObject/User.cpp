#include "User.h"
#include <QDebug>

// 静态成员变量需要在类外初始化
QPointer<User> User::currentUser = 0;
const QString User::DEFAULT_ADMIN_PASSWORD("AllrisingAdmin");
QMutex User::mutex;

User::User(QObject *parent) :
    ConfigObject(parent)
{
    SetRaw("Type", "User");
}

ConfigObject *User::NewObject(QObject *parent) const
{
    return dynamic_cast<ConfigObject *>(new User(parent));
}

void User::ManagerLoaded(ConfigManager *manager)
{
    User *admin = manager->FindObject<User>("UserName", "Admin");
    if (admin == NULL)          /* 必须存在管理员用户 */
    {
        qDebug() << "User::ManagerLoaded creating Admin user.";
//        admin = manager->NewItem<User>();
//        admin->Set("UserName", "Admin");
//        admin->SetPassword(DEFAULT_ADMIN_PASSWORD);
    }
}

User *User::CurrentUser()
{
    QMutexLocker lock(&mutex);
    Q_UNUSED(lock);       /* 避免编译器报警告 */
    return currentUser;
}

User::ConfigID User::CurrentUserID()
{
    User * currentUser = CurrentUser();
    if (currentUser == NULL)
    {
        return 0;
    }
    return currentUser->GetID();
}

void User::SwitchUser(User *user)
{
    QMutexLocker lock(&mutex);
    Q_UNUSED(lock);
    currentUser = user;
}

const QJsonValue User::Get(const QString &key)
{
    if (key == "Password")
    {
        qWarning() << "Attempt to read password.";
        return QJsonValue();        /* 拒绝访问，返回Null值 */
    }
    return ConfigObject::Get(key);
}

void User::Set(const QString &key, const QJsonValue &value)
{
    if (key == "Password")
    {
        qWarning() << "Attempt to write password.";
        return;
    }
    ConfigObject::Set(key, value);
}

void User::SetPassword(const QString &password)
{
    QJsonValue value(Encryption(password, GetID()));
    ConfigObject::Set("Password", value);
}

bool User::ChallengePassword(const QString &password)
{
    QJsonValue value = GetRaw("Password");
    QString str = Encryption(password, GetID());
    if (value.toString() == Encryption(password, GetID()))
    {
        return true;
    }
    return false;
}

const QString User::Encryption(const QString &password, ConfigID id)
{
    QByteArray byte;
    byte.append(password);
    byte.append(QString::number(id));
    QString ret = QCryptographicHash::hash(byte, ENCRYPT_ALGORITHM).toHex();
    return ret;
}
