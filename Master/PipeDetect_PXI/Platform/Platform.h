#ifndef PLATFORM_H
#define PLATFORM_H

#include "ThreadController/ThreadWork.h"
#include "Platform/PlatformBase.h"
#include <QVector>
/* 前置声明 */

class AECWork;
class UTSetupSetOne;
class ChannelManager;

/*!
 * \class Platform
 * \inmodule PXI_trainWheelDetect
 * Platform是全局的单例对象，表达一个完整的超声检测平台，负责收集、管理、载入、保存各种配置对象，初始化核心业务。
 * Platform无法直接被外界直接构造，需要用Platform::GetPlatform()来获取一个全局唯一的指针。也可以使用缩写platform()。
 *
 * Platform本身也是一个配置对象，其中包含程序的全局设置，可以使用platform()->Get("key")和platform()->Set("key", value)来访问。
 *
 * Platform加载顺序：
 * 1. Platform在初次构造时，首先会载入Platform.json
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


typedef QSharedPointer<AECWork> sharedAecWork;
typedef QVector<sharedAecWork> AecWorkVector;
typedef QSharedPointer<AecWorkVector> pointerAecWorkVec;

class Platform : public PlatformBase
{
    Q_OBJECT

public:
    /* 声明各组指向ConfigManager的指针，及其Get方法。
     * 其他类只能使用该指针，无法析构它们，因为ConfigManager的析构体只有友元类才能调用 */
    /* ConfigManager在构造之后，parent应当设为Platform。 */
    PLATFORM_NEW_CONFIG_TYPE(User)
    PLATFORM_NEW_CONFIG_TYPE(Setup)
    PLATFORM_NEW_CONFIG_TYPE(Marker)
    PLATFORM_NEW_CONFIG_TYPE(Material)
    PLATFORM_NEW_CONFIG_TYPE(Instrument)

public:
    /* 向外界导出的单例获取方法，返回一个全局唯一的Platform指针 */
    static Platform *GetPlatform();
    UTSetupSetOne *GetUTSetupSetOne() const;

private:
    virtual void Init();
    virtual void CreateCardWork();

protected:
    /* 单例管理和包装 */
    explicit Platform(QObject *parent = 0);                                     /* 受保护的构造体，只允许被自身的static GetInstance()构造 */
    ~Platform();                                                                /* 受保护的析构体，只允许由友元PlatformDeleter析构 */

private:
    struct PlatformDeleter                                                      /* 受保护的删除器类，用来析构Platform。由于Platform的析构体受保护，这个删除器类应当是Platform的友元类。 */
    {
        static void cleanup(Platform *pointer)                                  /* QScopedPointer要求的清理函数，析构传入的对象 */
        {
            delete pointer;
        }
    };

    friend struct PlatformDeleter;                                              /* PlatformDeleter在定义时是struct，此处使用friend class会导致警告 */
    typedef QScopedPointer<Platform, PlatformDeleter> PlatformPointer;          /* 使用自定义删除器的自动指针。 */
    static PlatformPointer instance;                                            /* 该指针对象被析构时，将调用受保护的Deleter类中的cleanup析构Platform。其他方法无法析构Platform。 */
    UTSetupSetOne *utSetupSetOne;
};

/* 全局获取platform方法，为GetInstance缩写 */
static inline Platform *platform()
{
    return Platform::GetPlatform();
}

#endif // PLATFORM_H
