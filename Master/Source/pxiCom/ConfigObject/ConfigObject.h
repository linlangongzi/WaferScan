#pragma once

#include <QObject>
#include <QReadWriteLock>
#include <QJsonObject>
#include <QDateTime>
#include <QPointer>

class ConfigManager;
class Platform;

#define CONFIG_OBJECT   friend class ConfigManager;   \
                        friend class Platform;

/*!
    \class ConfigObject
    \threadsafe

    \brief \c ConfigObject 是可配置对象的基类，抽象类，不可直接实例化。

    \c ConfigObject 只应当被 \c Config 和 \c User 两个类继承，其他可配置对象应当使用 \c Config 作为基类。

    \c ConfigObject 提供了一个受控的 \c QJsonObject 注册表，类似于\c {QMap<QString, QVariant>}，但值的类型为\c QJsonValue。
    在外部访问该配置对象应当使用 \c Get 和 \c Set 方法，可配置对象在继承 \c Config 时可以覆盖 \c Get 和 \c Set，对自定义的若干个key进行权限控制。
    例如，一个探头对象具有属性"Frequency"，并且不希望外部更改它，则重载 \c Get 时可进行判断，如果key为"Frequency"则拒绝访问。
    \c ConfigObject 中的所有方法均保证线程安全。

    对派生类， \c ConfigObject 还提供了受保护的 \c GetRaw 和 \c SetRaw 来绕过 \c Get 和 \c Set 的限制进行读写。
    \c GetRaw 和 \c SetRaw 将负责保证线程安全，并且 \c SetRaw 会发出值被更改的信号。

    作为一个 \c QObject ， \c ConfigObject 还提供了值被更改的信号，可以供外部了解注册表被更改的情况。

    \c ConfigObject 应当受 \c ConfigManager 的管理， \c ConfigManager 是一个 \c QJsonArray 注册表，负责管理和查询同类型的若干 \c ConfigObject 。
    每个 \c ConfigObject 都有一个唯一的ID，供 \c ConfigManager 索引使用。

    继承 \c ConfigObject 的类，需要在 \c Q_OBJECT 后加入宏 \c CONFIG_OBJECT ，该宏会为派生类设置友元类。

    作为核心配置组件,这个类的代码为执行效率进行过深度优化.
*/

class ConfigObject : public QObject
{
    Q_OBJECT
    CONFIG_OBJECT

public:
    /*!
     * \brief 内部索引使用的ID类型，转为\c QJsonObject 时存为字符串.
     */
    typedef qulonglong ConfigID;

    /*!
     * \brief ConfigID的最大值, 为ulonglong上限的一半. 这个上限不应该达到.
     */
    static const ConfigID ConfigIDMax = 0x8000000000000000;

    /* 静态类型转换函数，用于特定类型的值与QJsonValue相互转换 */
    static const QJsonValue FromObjectID(ConfigID id);
    static ConfigID ToObjectID(const QJsonValue &value);
    static const QJsonValue FromDateTime(const QDateTime &time);
    static const QDateTime ToDateTime(const QJsonValue &value);

    static void DelObject(ConfigObject* obj);

    /*!
     * \brief 指向 \c ConfigObject 的受保护指针
     */
    typedef QPointer<ConfigObject> pointer;

public:
    /* 获取JsonObject表中的一个值，可由派生类重载，默认等价于GetRaw */
    virtual const QJsonValue Get(const QString &key);

    /* 获取JsonObject表中的一个值，并将其转为ConfigID。等价于ToObjectID(Get(key))。 */
    ConfigID GetAsID(const QString &key);

    /* 获取JsonObject表中的一个值，并将其转为QDateTime。值实际保存为字符串，格式由DATE_FORMAT决定。 */
    QDateTime GetAsDateTime(const QString &key);

    /* 获取自身的ID，等价于ToObjectID(Get("ID"))，当ID不存在时返回0。 */
    ConfigID GetID();

    /* 设置JsonObject表中的一个值 */
    virtual void Set(const QString &key, const QJsonValue &value);
    void Set(const QString &key, const QString &value);    /* 以下所有Set均依赖虚函数Set */
    void Set(const QString &key, double value);
    void Set(const QString &key, bool value);
    void Set(const QString &key, const QJsonObject &value);
    void Set(const QString &key, const QJsonArray &value);
    void Set(const QString &key, const QVariant &value);
    void Set(const QString &key, ConfigID id);
    void Set(const QString &key, const QDateTime &value);

    /* 导入和导出 */
    virtual const QJsonObject ToJson();

public slots:
    /*!
     * \brief 请求一个最新的值，此槽函数将触发 \c ValueChanged 信号。
     * \param key 请求的键
     */
    void RequestUpdate(const QString &key);

signals:
    /*!
     * \brief 配置对象的值被改变时发出的信号. 当且仅当新值与旧值不同时才会发出该信号.
     * \param key 被改变的键
     * \param value 新的值
     */
    void ValueChanged(const QString &key, const QJsonValue &value);

protected:
    /* 从Json导入 */
    virtual void FromJson(const QJsonObject &source);

    /* 设置配置对象的内部ID，该操作仅供ConfigManager使用 */
    void SetID(ConfigID id);

    /* 对JsonObject注册表的原始写入 */
    QJsonValue GetRaw(const QString &key);
    void SetRaw(const QString &key, const QJsonValue &value);

    /*!
     * \brief 工厂方法，创建一个自身的副本，必须由派生类重写，构造其自身。
     * 此方法仅供友元类ConfigManager调用。
     * \param parent QObject的父对象
     * \return ConfigObject的指针
     */
    virtual ConfigObject *NewObject(QObject* parent) const = 0;

    /* 接口方法，由ConfigManager在初始化完成后调用调用一次。在这里可以进行默认Object创建等工作。如有需要则重载。
     * 请注意，调用时this指针是原型对象，所以其中Set的值不会被保存。
     * 详情参见Platform.h中加载顺序。 */
    virtual void ManagerLoaded(ConfigManager *manager) {Q_UNUSED(manager);}
    /* 接口方法，由ConfigManager在ManagerLoaded之后，对受控的所有ConfigObject分别调用一次。 */
    virtual void LoadCompleted() {}
    /* 接口方法，由ConfigManager在所有LoadCompleted之后，对受控的所有ConfigObject分别调用一次 */
    virtual void BindCompleted() {}

    /* 构造函数。ConfigObject不得被其他类构造，只应该由ConfigManager调用NewObject来构造 */
    explicit ConfigObject(QObject* parent = 0);
//    virtual ~ConfigObject();

private:
    static const Qt::DateFormat DATE_FORMAT = Qt::ISODate;
    QJsonObject data;
    QReadWriteLock lock;
};
