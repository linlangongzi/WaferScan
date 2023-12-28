#pragma once

#include "ConfigObject.h"
#include <QObject>
#include <QThread>
#include <QReadWriteLock>
#include <QMap>
#include <QJsonArray>

/* ConfigManager 配置对象注册表
 * 所有Public方法均为线程安全。
 * 友元类使用Protected迭代器时，需要对lock添加读取锁。其他方法均为线程安全。
 *
 * ConfigManager是一个类型的所有配置对象的注册表，负责管理和查询ConfigObject配置对象。
 * ConfigObject具有一个ID，其格式详见定义ConfigObject::ConfigID。Manager的注册表使用该ID进行索引，该ID由Manager自动分配。
 *
 * 作为一个注册表和管理类，在构造时，需要指定一个容纳的子项的原型，ConfigManager会调用原型的New方法来构造新的子项，而不是用构造体。
 * 该原型应当在构造前创建，不应含有数据，其中的任何数据都无法保存，也不能被查询到。
 * 这个设计是由于ConfigManager为QObject，而QObject不能作为模板类被实例化。
 *
 * ConfigManager提供了一系列方法查询配置对象，效率最高的方法是GetByID。
 * 当需要新建配置对象时，应当使用ConfigManager的NewItem。
 *
 * 还可以使用FindObject，给定一个子项的key和value，查询key和value都符合的第一个项。
 */
class ConfigManager : public QObject {
    Q_OBJECT

    friend class Platform;
public:
    /* 为方便使用，引用来自ConfigObject的类型和常量 */
    typedef ConfigObject::ConfigID ConfigID;
    static const ConfigID ConfigIDMax = ConfigObject::ConfigIDMax;
    typedef ConfigObject::pointer pointer;      /* 指向ConfigObject的安全指针，当ConfigObject析构时会自动变为NULL */

    typedef QMap<ConfigID, pointer> Registry; /* 对象注册表，使用安全指针 */


    /* 使用唯一的ID获取一个ConfigObject */
    ConfigObject *GetByID(ConfigID id);

    template <typename T>
    T *GetByID(ConfigID id)
    {
        return dynamic_cast<T *>(GetByID(id));      /* 根据模板类型，将ConfigObject指针转换为派生类指针。 */
    }

    /* 移除并销毁一个ConfigObject */
    void Remove(ConfigObject *item);
    void Remove(ConfigID id);

    /* 获取已注册的对象数 */
    int GetNumItems() const;

    /* 获取一份注册表的copy，用于遍历注册表。修改此表不会对ConfigManager造成任何影响。 */
    Registry GetRegistry();

    ConfigObject *GetByIndex(int index);

    /* 将配置对象序列化为JsonArray表，或从JsonArray表恢复配置对象表，及其中的各项 */
    void FromJson(const QJsonArray &json);
    QJsonArray ToJson() const;

    /* 给定一个子项的key和value，遍历所有已注册的对象，返回key符合value的第一项的指针。
     * 如果找不到符合条件的对象，返回NULL。 */
    ConfigObject *FindObject(const QString& key, const QJsonValue &value);
    template <typename T>
    T *FindObject(const QString& key, const QJsonValue &value)
    {
        return dynamic_cast<T *>(FindObject(key, value));      /* 根据模板类型，将ConfigObject指针转换为派生类指针。 */
    }

    /* 在ConfigManager中新建一项。默认返回ConfigObject指针 */
    ConfigObject *NewItem(const QJsonObject &jsonObj = QJsonObject());

    /* 模板方法，当指定类型时，将ConfigObject指针转换为对应配置对象 */
    /* 用法：User * newUser = manager.NewItem<User>();
     *      Probe * newProbe = manager.NewItem<Probe>();
     */
    template <typename T>
    T *NewItem(const QJsonObject &jsonObj = QJsonObject())
    {
        return dynamic_cast<T *>(NewItem(jsonObj));                            /* 根据模板类型，将ConfigObject指针转换为派生类指针。 */
    }

protected slots:
    void LoadCompleted();                       /* 连接所有Manager加载完成信号，参见Platform.h中加载顺序。 */
    void BindCompleted();                       /* 连接所有Manager绑定完成信号 */

protected:
    /* 构造体。构造时，需要指定一个子项原型，子项原型必须是ConfigObject的派生类。
     * 此外，还需要指定Platform，ConfigManager将自动连接部分信号 */
    explicit ConfigManager(ConfigObject *prototype, QObject *parent = NULL);

//    void AddItem(ConfigObject *item);
    ConfigID NewID();
    /* 将一个配置对象注册到对象注册表中。受保护，仅供内部使用。 */
    void RegisterItem(ConfigObject *item);

    /* const迭代器，用于遍历整个注册表。为了避免遍历时读写冲突，使用前需要对ConfigManager进行读取锁定。
     * 由于涉及读写锁，迭代器访问受保护，只允许友元类访问，请谨慎使用 */
    typedef Registry::const_iterator iterator;  /* 重定义迭代器类型 */
    iterator begin();
    iterator end();
    QReadWriteLock lock;

public:
    /* ConfigManager只能被友元类析构 */
    ~ConfigManager();

signals:
    /* 当列表更新时发出信号 */
    void ItemRemoved();
    void ItemAdded(ConfigObject *newItem);

private:
    /* 内部函数，移除一个ConfigObject，需要提供对应的ID。
     * 使用时须保证item有效，且item与ID对应无误
     */
    void Remove(ConfigObject *item, ConfigID id);

    /* 配置对象原型，由于只有构造时才写入，不受读写锁保护 */
    pointer configPrototype;

    /* registry和maxID均受读写锁lock保护 */
    Registry registry;
    ConfigID maxID;
};
