#include "ConfigManager.h"
#include <QDebug>

ConfigManager::ConfigManager(ConfigObject *prototype, QObject *parent) :
    QObject(parent),
    configPrototype(prototype),
    registry(),
    maxID(0)
{
    prototype->setParent(this);         /* 原型对象的parent设为自身，所以析构时不需要delete */
    if (parent == NULL)
    {
        qFatal("ConfigManager::Constructo:r must have a platform parent.");
    }
    setParent(parent);
    connect(parent, SIGNAL(LoadCompleted()), this, SLOT(LoadCompleted()));
    connect(parent, SIGNAL(BindCompleted()), this, SLOT(BindCompleted()));
}

ConfigManager::~ConfigManager()
{

}

ConfigObject *ConfigManager::GetByID(ConfigID id)
{
    QReadLocker readLock(&lock);
    ConfigObject *obj = registry.value(id, NULL);
    readLock.unlock();
    return obj;
}

void ConfigManager::Remove(ConfigObject *item)
{
    if (Q_UNLIKELY(item == NULL))
    {
        qWarning("ConfigManager::Remove: attempt to remove object with null pointer");
        return;
    }
    ConfigID id = item->GetID();
    QReadLocker readLock(&lock);
    pointer registryItem = registry.value(id, NULL);
    readLock.unlock();
    Q_ASSERT_X(registryItem == item, "ConfigManager::Remove", "item pointer and ID mismatch");
    Remove(item, id);
}

void ConfigManager::Remove(ConfigID id)
{
    QReadLocker readLock(&lock);
    Q_ASSERT_X(registry.contains(id), "ConfigManager::Remove", "item ID not found");
    pointer item = registry.value(id);
    if (Q_UNLIKELY(item.isNull()))
    {
        qCritical() << "ConfigManager:Remove: Attempt to remove null object.";
        return;
    }
    readLock.unlock();
    Remove(item.data(), id);
}

/* 内部函数，移除一个ConfigObject，需要提供对应的ID。
 * 使用时须保证item有效，且item与ID对应无误
 */
void ConfigManager::Remove(ConfigObject *item, ConfigID id)
{
    QWriteLocker writeLock(&lock);
    registry.remove(id);
    writeLock.unlock();
    delete item;    /* ConfigObject的销毁必须在Registry更新之后 */
    emit ItemRemoved();
}

int ConfigManager::GetNumItems() const
{
    return registry.size();
}

ConfigManager::Registry ConfigManager::GetRegistry()
{
    QReadLocker readLock(&lock);
    Q_UNUSED(readLock);                   /* 压制编译器警告 */
    return Registry(registry);  /* 使用QMap的复制构造体构造一份copy */
}

ConfigObject *ConfigManager::GetByIndex(int index)
{
    int count = GetNumItems();
    if (Q_UNLIKELY(index >= count))
    {
        qWarning() << "ConfigManager:GetByIndex: ConfigObject index out of bound.";
        return NULL;
    }
    QReadLocker readLock(&lock);
    ConfigObject *obj = static_cast<ConfigObject *>(registry.values().value(index));
    readLock.unlock();
    return obj;
}

void ConfigManager::FromJson(const QJsonArray &json)
{
    qDebug() << "ConfigManager::FromJson: Deserializing" << json.size() << "config objects with type" << configPrototype->GetRaw("Type");
    foreach (const QJsonValue itemValue, json)
    {
        if (Q_UNLIKELY(!itemValue.isObject()))
        {
            qWarning() << "ConfigManager::FromJson: Deserializing invalid config object: not a json object" << itemValue;
        }
        else
        {
            QJsonObject itemObj = itemValue.toObject();
            ConfigObject * item = NewItem(itemObj);
            if (Q_UNLIKELY(item == NULL))
            {
                qWarning() << "ConfigManager::FromJson: Failed adding new config object.";
            }
        }
    }
}

QJsonArray ConfigManager::ToJson() const
{
    qDebug() << "Serializing " << registry.size() << "config objects ...";
    QJsonArray array;
    foreach (ConfigObject *const item, registry.values())
    {
        const QJsonObject itemObj = item->ToJson();
        if (itemObj.isEmpty())
        {
            qWarning() << "ConfigManager::ToJson: Empty json object from item " << item;
        }
        else
        {
            array.append(QJsonValue(itemObj));
        }
    }
    return array;
}

ConfigManager::iterator ConfigManager::begin()
{
    return registry.constBegin();
}

ConfigManager::iterator ConfigManager::end()
{
    return registry.constEnd();
}

ConfigManager::ConfigID ConfigManager::NewID()
{
    ConfigID prevMax = maxID;
    if (Q_UNLIKELY(prevMax < ConfigIDMax))  /* 可用的ID超出预设的分配范围 */
    {
        return prevMax + 1;
    }
    qFatal("ConfigManager::NewID: Failed allocating new object ID, please contact retailer.");    /* 虽然ConfigID一直在递增，但在产品寿命周期内应当永远达不到上限 */
    return 0;
}

void ConfigManager::RegisterItem(ConfigObject *item)
{
    if (Q_UNLIKELY(item == NULL))
    {
        qCritical() << "ConfigManager::RegisterItem: item is null";
        return;
    }
    if (Q_UNLIKELY(configPrototype.isNull()))
    {
        qFatal("ConfigManager::RegisterItem: ConfigPrototype is null");
    }
    if (item->GetRaw("Type") != configPrototype->GetRaw("Type"))
    {
        qCritical() << "ConfigManager::RegisterItem: type mismatch, expecting " << configPrototype->GetRaw("Type") << ", got " << item->GetRaw("Type");
    }
    ConfigID id = item->GetID();
    QWriteLocker writeLock(&lock);
    if (0 == id)
    {
        id = NewID();
        qDebug() << "ConfigManager::RegisterItem: Allocating new ID" << id;
        item->SetID(id);
    }
    Q_ASSERT(id > 0);
    maxID = qMax<ConfigID>(maxID, id);
    registry.insert(id, pointer(item));
    writeLock.unlock();
    emit ItemAdded(item);
}

ConfigObject *ConfigManager::FindObject(const QString &key, const QJsonValue &value)
{
    QReadLocker readLock(&lock); /* 类似于QMutexLocker，施加读取锁，函数返回时，读取锁析构，自动解锁。 */
    Q_UNUSED(readLock);                   /* 避免编译器发出警告 */
    foreach (pointer obj, registry)
    {
        if (!obj.isNull() && obj->Get(key) == value)    /* C++使用短路逻辑，当obj为null时obj->Get不会被执行 */
        {
            return obj.data();
        }
    }
    return NULL;
}

ConfigObject *ConfigManager::NewItem(const QJsonObject &jsonObj)
{
    if (Q_UNLIKELY(configPrototype.isNull()))                                   /* 检查配置对象的原型 */
    {
        qFatal("ConfigManager:NewItem: ConfigPrototype is null");
        return NULL;
    }
    Q_ASSERT(!configPrototype->GetRaw("Type").isNull());                        /* 配置对象的原型必须含有Type信息 */
    if (Q_UNLIKELY(
        !jsonObj.value("Type").isNull()
        && jsonObj.value("Type") != configPrototype->GetRaw("Type")))           /* 插入的新对象，如果指定了Type，必须与原型的Type相同 */
    {
        qWarning() << "ConfigManager::NewItem: invalid config object: type mismatch:" << jsonObj.value("Type") << ", expecting" << configPrototype->GetRaw("Type");
        return NULL;                                                            /* 类型错误 */
    }
    ConfigObject *item = configPrototype->NewObject(this);                      /* 调用原型的New，构造新对象，并将其parent设为自身 */
    if (Q_UNLIKELY(item == NULL))
    {
        qCritical("ConfigManager::NewItem: Failed creating item.");
        return NULL;
    }
    if (!jsonObj.isEmpty())                                                     /* 由于空的JsonObject会覆盖构造体中设置的Type，所以只有非空的JsonObject才被使用 */
    {
        item->FromJson(jsonObj);                                                /* 用传入的JsonObject初始化该对象 */
    }
    RegisterItem(item);                                                         /* 在注册表中注册 */
    return item;                                                                /* 根据模板类型，将ConfigObject指针转换为派生类指针。 */
}

void ConfigManager::LoadCompleted()
{
//    ARS_CHECK_MAIN_THREAD;                                                      /* 必须在主线程中执行 */
    if (Q_UNLIKELY(configPrototype.isNull()))
    {
        return qFatal("ConfigManager::ConfigLoaded: configPrototype is null");
    }
//    qDebug() << "Config::Manager LoadCompleted" << this;
    configPrototype->ManagerLoaded(this);
    foreach (pointer item, registry)        /* 注意，此处没有对Manager加锁，因为这个调用应当永远存活在主线程中。 */
    {
        item->LoadCompleted();
    }
}

void ConfigManager::BindCompleted()
{
    foreach (pointer item, registry)        /* 注意，此处没有对Manager加锁，因为这个调用应当永远存活在主线程中。 */
    {
        item->BindCompleted();
    }
}
