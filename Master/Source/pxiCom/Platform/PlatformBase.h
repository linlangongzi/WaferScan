#pragma once

#include "ConfigObject/ConfigObject.h"
#include "ConfigObject/ConfigManager.h"
#include <QFile>
#include <QMutex>
#include <QString>
#include <QPointer>
#include <QJsonDocument>
#include "ProbeConnectManager/ProbeConnectManager.h"
#include "CardWork/CardWorkBase.h"
#include "CardWork/PLXCommunication.h"
#include <QVector>
/* 前置声明 */

/* 定义Config类型的宏，展开后将为指定Config类型创建私有指针、初始化和保存方法、静态默认路径，用于缩短头文件长度 */
#define PLATFORM_NEW_CONFIG_TYPE(_NAME) PLATFORM_NEW_CONFIG_TYPE_PL(_NAME, _NAME ## s)
/* 自定义复数形式的版本 */
#define PLATFORM_NEW_CONFIG_TYPE_PL(_NAME, _NAME_PLURAL) \
    public:     ManagerPointer _NAME_PLURAL() const {Q_ASSERT(_NAME ## Manager != NULL); return _NAME ## Manager;}    /* 定义全局Getter，例如public: ManagerPointer Users(){return UserManager;} */    \
    private:    ManagerPointer _NAME ## Manager;                                /* 定义私有指针变量，例如private: ManagerPointer UserManager; */                 \
    private:    void Init ## _NAME_PLURAL();                                    /* 定义私有初始化方法，例如private: InitUsers(); */                              \
    private:    void Save ## _NAME_PLURAL();                                    /* 定义私有保存方法，例如private: SaveUsers(); */                                \
    private:    const static QString DEFAULT_ ## _NAME ## _PATH;                /* 定义静态默认保存路径，例如private: const static QString DEFAULT_User_PATH; */ \

/*!
 * \class PlatformBase
 * \inmodule PXICOM
 * PlatformBase是全局的单例对象，表达一个完整的超声检测平台，负责收集、管理、载入、保存各种配置对象，初始化核心业务。
 * PlatformBase无法直接被外界直接构造，需要用PlatformBase::GetPlatformBase()来获取一个全局唯一的指针。也可以使用缩写platformBase()。
 *
 * PlatformBase本身也是一个配置对象，其中包含程序的全局设置，可以使用PlatformBase()->Get("key")和PlatformBase()->Set("key", value)来访问。
 *
 * PlatformBase加载顺序：
 * 1. PlatformBase在初次构造时，首先会载入Platform.json
 *
 * 2. 根据Platform.json中记录的路径，按顺序载入各ConfigManager。
 *    例如，加载用户列表时，首先读取Get("ProbeProfilePath")，如果值存在，则使用该值作为路径；如果不存在，则使用DEFAULT_User_PATH作为路径。
 *    这个过程中会调用manager->FromJson()对manager及其中的配置对象进行赋值。
 *
 * 3. 所有ConfigManager都完成后，会发出LoadCompleted()信号，此信号由ConfigManager自动绑定到自身的槽，每个Manager都会进行初始化操作：
 *    a. 调用prototype->ManagerLoaded(manager*)，对特型的manager初始化，这个过程中可以进行预置项检查，预置项属性检查等操作。
 *       例如，User列表中需要保证永远有root用户，则可以重载User::ManagerLoaded()，在其中进行manager->FindObject("UserName", "root")，并作出相应处理。
 *
 *    b. 遍历manager注册表，分别执行item->LoadCompleted()，这个过程中应当完成跨种类绑定等操作。
 *       例如，UTInstrument需要与Probe进行绑定，绑定依赖于Instrument中记录的ProbeID，则可以重载UTInstrument::LoadCompleted()，在其中读取自身的属性，查询全局Probes表，进行绑定。
 *
 * 4. 所有ConfigManager都完成后，会发出BindCompleted()信号，此信号由ConfigManager自动绑定到自身的槽，每个Manager都会进行初始化操作
 *    a. 遍历manager注册表，分别执行item->BindCompleted()，此时所有配置对象都已经完成绑定，应当将进行数值同步等操作。
 */

typedef QSharedPointer<CardWorkBase> sharedCardWork;
typedef QVector<sharedCardWork> CardWorkVector;
typedef QSharedPointer<CardWorkVector> pointerCardWorkVec;

class PlatformBase : public ConfigObject
{
    Q_OBJECT

public:
    typedef QPointer<ConfigManager> ManagerPointer; /* 指向ConfigManager的指针。 */
    /* 声明各组指向ConfigManager的指针，及其Get方法。
     * 其他类只能使用该指针，无法析构它们，因为ConfigManager的析构体只有友元类才能调用 */
    /* ConfigManager在构造之后，parent应当设为PlatformBase。 */

public:
    /* 向外界导出的单例获取方法，返回一个全局唯一的PlatformBase指针 */
    static PlatformBase *GetPlatformBase();
    sharedProbeNoManager GetProbeNoManager();
    pointerCardWorkVec GetAecWork() const;
    sharedCardWork GetMfcWork() const;

    template <typename T1, typename T2, typename T3> T1 GetAecWorkByType() const
    {
        T1 pAecWorkVec = T1::create();
        for (CardWorkVector::iterator it = pAecWork->begin(); it != pAecWork->end(); ++it)
        {
            sharedCardWork cardWork = *it;
            T2 aecWork = T2::create();
            aecWork = cardWork.objectCast<T3>();
            pAecWorkVec->append(aecWork);

        }
        return pAecWorkVec;
    }

    /*template <typename T> T* GetMfcWork() const
    {
        return dynamic_cast<T*>(pMfcWork);
    }*/
    template <typename T> T* GetMfcWork() const
    {
        return dynamic_cast<T*>(pMfcWork.data());
    }

    SharedPLXCommunication GetPLX();
    bool IsFlaw() const;
    void AecWorkInit();
    void AecWorkStart();
    void AecWorkStop();
    /* 辅助函数，将JsonDocument保存到指定路径 */
    void SaveJsonConfig(const QJsonDocument &jsonDoc, const QJsonValue &path, const QString &defaultPath);

protected:
    virtual void Init();    /* 初始化所有配置对象，由于构造体中初始化会导致死锁，Init需要在构造并解锁之后进行。 */
    virtual void CreateCardWork();
    void InitPlatform();
    void SavePlatform();
    const static QString DEFAULT_Platform_PATH;

    /* 辅助函数，从目标路径载入Json文件，用该JsonObject初始化manager。 */
    QJsonDocument LoadJsonConfig(const QJsonValue &path, const QString &defaultPath, ConfigManager *manager);

    /* 辅助函数，将JsonDocument保存到指定路径 */
//    void SaveJsonConfig(const QJsonDocument &jsonDoc, const QJsonValue &path, const QString &defaultPath);

    const static QJsonDocument::JsonFormat JSON_FORMAT;  /* 默认Json导出格式 */

    virtual ConfigObject *NewObject(QObject *parent) const { Q_UNUSED(parent); return NULL; }
    int GetAecCardNum();

signals:
    void LoadCompleted();                                                       /* 所有Manager加载完成信号 */
    void BindCompleted();                                                       /* 所有Manager绑定完成信号 */
signals:
    void PlxInitResult(int);                                                    /* 暂时不放在CPLXCommunication里，否则需改他造该类*/
public slots:
    CPLXCommunication::E_InitResult GetPlxInitResult();                         /* 暂时不放在CPLXCommunication里，否则需改他造该类*/

protected:
    /* 单例管理和包装 */
    explicit PlatformBase(QObject *parent = 0);                                     /* 受保护的构造体，只允许被自身的static GetInstance()构造 */
    ~PlatformBase();                                                                /* 受保护的析构体，只允许由友元PlatformDeleter析构 */

protected:
    static QMutex singletonMutex;                                               /* 单例指针写入专用的互斥锁 */
    sharedProbeNoManager probeNoManager;
    pointerCardWorkVec pAecWork;
    SharedCardParamVec pCardParam;
    SharedPLXCommunication pPLX;
    //CardWorkBase* pMfcWork;
    sharedCardWork pMfcWork;
    bool isFlaw;
    struct PlatformBaseDeleter                                                      /* 受保护的删除器类，用来析构PlatformBase。由于PlatformBase的析构体受保护，这个删除器类应当是PlatformBase的友元类。 */
    {
        static void cleanup(PlatformBase *pointer)                                  /* QScopedPointer要求的清理函数，析构传入的对象 */
        {
            delete pointer;
        }
    };

private:
    friend struct PlatformBaseDeleter;                                              /* PlatformDeleter在定义时是struct，此处使用friend class会导致警告 */
    typedef QScopedPointer<PlatformBase, PlatformBaseDeleter> PlatformBasePointer;          /* 使用自定义删除器的自动指针。 */
    static PlatformBasePointer instance;                                            /* 该指针对象被析构时，将调用受保护的Deleter类中的cleanup析构Platform。其他方法无法析构PlatformBase。 */
};

/* 全局获取PlatformBase方法，为GetInstance缩写 */
static inline PlatformBase *platformBase()
{
    return PlatformBase::GetPlatformBase();
}
