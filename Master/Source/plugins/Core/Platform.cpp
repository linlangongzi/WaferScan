#include "Platform.h"

static Platform *m_instance = nullptr;

Platform::Platform(QObject *parent) :
    QObject(parent)
{
    if (m_instance) {
        qFatal("Platform may only be constructed once.");
        return;
    }
    m_instance = this;
}

Platform::~Platform()
{
    m_instance = nullptr;
}

QJsonValue Platform::Get(const QString &key) const
{
    return data.value(key);
}

QJsonObject Platform::ToJson() const
{
    return data;
}

void Platform::Set(const QString &key, const QJsonValue &value)
{
    if (key.isEmpty()) {
        return;
    }

    /* 获取旧的数值。获取旧数值与写入新数值，应当为一个原子操作。
     * 此处不可使用GetRaw，因为GetRaw使用权限更低的readLock，并不能保证读与写之间不发生其他写入。 */
    QJsonValue preValue = data.value(key);
    data.insert(key, value);

    if (preValue != value) {                                               /* 发出信号，必须在已解锁的情况下进行 */
        emit ValueChanged(key, value);
    }
}

void Platform::FromJson(const QJsonObject &source)
{
    data = source;
    foreach (QString key, data.keys()) {
        emit ValueChanged(key, Get(key));
    }
}

Platform *platform()
{
    return m_instance;
}
