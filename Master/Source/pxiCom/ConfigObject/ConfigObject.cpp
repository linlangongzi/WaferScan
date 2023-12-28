#include "ConfigObject.h"
#include <QDebug>

/*!
 * \brief ConfigObject::ConfigObject
 * \param parent QObject 父子关系中的父对象
 */
ConfigObject::ConfigObject(QObject *parent) :
    QObject(parent)
{
    data = QJsonObject();
}

/*!
 * \brief 将一个ConfigID转换为QJsonValue。这个函数与 \c ToObjectID 使用一致的格式。
 * \param id 需要转换的 \c ConfigID
 * \return 转换之后的 \c QJsonValue
 */
const QJsonValue ConfigObject::FromObjectID(ConfigID id)
{
    return QJsonValue(QString::number(id));
}

/*!
 * \brief 将一个 \c QJsonValue 转换为 \c ConfigID 。这个函数与 \c FromObjectID 使用一致的格式。
 * \param value 要转换的 \c QJsonValue 值。
 * \return 转换之后的 \c ConfigID
 */
ConfigObject::ConfigID ConfigObject::ToObjectID(const QJsonValue &value)
{
    return value.toString().toULongLong();  /* 如果value的值无法转为toString()，将返回空字符串，从而转为0。 */
}

const QJsonValue ConfigObject::FromDateTime(const QDateTime &time)
{
    return QJsonValue(time.toString(DATE_FORMAT));
}

const QDateTime ConfigObject::ToDateTime(const QJsonValue &value)
{
    return QDateTime::fromString(value.toString(), DATE_FORMAT);
}

void ConfigObject::DelObject(ConfigObject *obj)
{
    obj->SetRaw("Deleted", QJsonValue(true));
}

const QJsonValue ConfigObject::Get(const QString &key)
{
    return GetRaw(key);
}

ConfigObject::ConfigID ConfigObject::GetAsID(const QString &key)
{
    return ToObjectID(Get(key));
}

QDateTime ConfigObject::GetAsDateTime(const QString &key)
{
    return ToDateTime(Get(key));
}

ConfigObject::ConfigID ConfigObject::GetID()
{
    ConfigID id = ToObjectID(Get("ID"));
    if ( Q_UNLIKELY(id == 0 && (data.count() > 1)) )    /* 这个表达式不太可能成立 */
    {
        qCritical() << "Failed getting ID from Json";
    }
    return id;
}

void ConfigObject::Set(const QString &key, const QJsonValue &value)
{
    if (key == "Type")
    {
        qCritical() << "ConfigObject::Set: Access denied for set 'Type'" << value;
        return;
    }
    SetRaw(key, value);
}

void ConfigObject::Set(const QString &key, const QString &value)
{
    Set(key, QJsonValue(value));
}

void ConfigObject::Set(const QString &key, double value)
{
    Set(key, QJsonValue(value));
}

void ConfigObject::Set(const QString &key, bool value)
{
    Set(key, QJsonValue(value));
}

void ConfigObject::Set(const QString &key, const QJsonObject &value)
{
    Set(key, QJsonValue(value));
}

void ConfigObject::Set(const QString &key, const QJsonArray &value)
{
    Set(key, QJsonValue(value));
}

void ConfigObject::Set(const QString &key, const QVariant &value)
{
    Set(key, QJsonValue().fromVariant(value));
}

void ConfigObject::Set(const QString &key, ConfigID id)
{
    Set(key, FromObjectID(id));
}

void ConfigObject::Set(const QString &key, const QDateTime &value)
{
    Set(key, FromDateTime(value));
}

void ConfigObject::FromJson(const QJsonObject &source)
{
    QWriteLocker writeLock(&lock);
    Q_UNUSED(writeLock);
    data = source;
}

const QJsonObject ConfigObject::ToJson()
{
    QReadLocker readLock(&lock);
    Q_UNUSED(readLock);
    return data;
}

void ConfigObject::RequestUpdate(const QString &key)
{
    qDebug() << "ConfigObject::RequestUpdate" << key;
    emit ValueChanged(key, Get(key));
}

void ConfigObject::SetID(ConfigID id)
{
    SetRaw("ID", FromObjectID(id));
}

QJsonValue ConfigObject::GetRaw(const QString &key)
{
    QReadLocker readLock(&lock);
    QJsonValue value = data.value(key);
    readLock.unlock();
    return value;
}

void ConfigObject::SetRaw(const QString &key, const QJsonValue &value)
{
    QWriteLocker writeLock(&lock);
    /* 获取旧的数值。获取旧数值与写入新数值，应当为一个原子操作。
     * 此处不可使用GetRaw，因为GetRaw使用权限更低的readLock，并不能保证读与写之间不发生其他写入。 */
    QJsonValue preValue = data.value(key);
    data.insert(key, value);
    writeLock.unlock();
    if (preValue != value)  /* 发出信号，必须在已解锁的情况下进行 */
    {
//        qDebug() << "ConfigObject::SetRaw: value changed - key" << key << preValue << "->" << value;、
        if(key == "LastModifiedBy" || key == "LastModifiedTime")
        {
            ;//
        }
        else
        {
            emit ValueChanged(key, value);
        }
    }
}
