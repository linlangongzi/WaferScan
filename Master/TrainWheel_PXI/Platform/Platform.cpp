#include "Platform.h"
#include "plx_lib_x.h"
#include "CardWork/AECWork.h"
#include "ConfigObject/User.h"
#include "ConfigObject/UTSetup.h"
#include "ConfigObject/Material.h"
#include "TcpServer/UTSetupSetOne.h"
#include "ConfigObject/UTInstrument.h"
#include "ChannelManager/ChannelManager.h"
#include "CardWork/MFCWork.h"
#include "ThreadController/ThreadController.h"
#include "ThreadController/ThreadWork.h"
#include "CardWork/AECRuningWork.h"
#include <QDebug>
#include <QApplication>

/* 初始化静态常量 */
const QString Platform::DEFAULT_User_PATH = "User.json";
const QString Platform::DEFAULT_Setup_PATH = "Setup.json";
const QString Platform::DEFAULT_Material_PATH = "Material.json";
const QString Platform::DEFAULT_Instrument_PATH = "Instrument.json";

/* 初始化静态变量 */
Platform::PlatformPointer Platform::instance;

Platform::Platform(QObject *parent) :
    PlatformBase(parent)
{
}

Platform::~Platform()
{
    qDebug() << "Platform destruction starting, saving config...";
    try
    {
        SavePlatform();
        SaveUsers();
        SaveSetups();
        SaveInstruments();
        SaveMaterials();
    }
    catch (...)
    {
        qFatal("Platform::Destruction failed.");
    }
}

Platform *Platform::GetPlatform()
{
    if (instance.isNull())
    {
        QMutexLocker locker(&singletonMutex);                                   /* 只在首次构造Platform时才锁定，避免多线程new多次 */
        instance.reset(new Platform);
        locker.unlock();
        instance->Init();                                                       /* 初始化过程可能会调用platform()，此时如果mutex不解锁将导致死锁，故将Init与构造分离。 */
    }
    return instance.data();
}

void Platform::Init()
{
    GetVersion();
    //注册UTSharedDataSegment类型，需要跨线使用信号槽传递该类型，要先声明Q_DECLARE_METATYPE(UTSharedDataSegment)
    qRegisterMetaType<UTSharedDataParam>("UTSharedDataParam");      // 不需要Q_DECLARE_METATYPE， 不传参需要Q_DECLARE_METATYPE
    qRegisterMetaType<UTSharedDataSegment>("UTSharedDataSegment");
    qRegisterMetaType<UTSharedFeatureSegment>("UTSharedFeatureSegment");
    qRegisterMetaType<UTSharedMonitorFrame>("UTSharedMonitorFrame");
    qRegisterMetaType<UTSharedDataSimulation>("UTSharedDataSimulation");
    qRegisterMetaType<UTSharedFeatureCommu>("UTSharedFeatureCommu");
    qRegisterMetaType<UTSharedCommuSegment>("UTSharedCommuSegment");
    qRegisterMetaType<sharedVersionLower>("sharedVersionLower");
    qRegisterMetaType<TCGCommuData>("TCGCommuData");
    qRegisterMetaType<TCGData>("TCGData");

    probeNoManager = sharedProbeNoManager::create();
    channelManager = new ChannelManager(this);
    utSetupSetOne = new UTSetupSetOne(this);
    pPLX = SharedPLXCommunication::create();

    qDebug() << "Loading platform config...";
    InitPlatform(); /* Platform必须首先初始化，因为初始化其他组件需要读取Platform配置 */

    InitUsers();
    InitSetups();
    InitInstruments();
    InitMaterials();
    emit LoadCompleted();
    emit BindCompleted();

    CreateCardWork();
}

void Platform::InitUsers()
{
    User* userPrototype = new User;
    UserManager = new ConfigManager(userPrototype, this);
    LoadJsonConfig(Get("UserProfilePath"), DEFAULT_User_PATH, UserManager);
}

void Platform::SaveUsers()
{
    QJsonDocument jsonDoc(UserManager->ToJson());
    SaveJsonConfig(jsonDoc, Get("UserProfilePath"), DEFAULT_User_PATH);
}

void Platform::InitSetups()
{
    UTSetup* setupPrototype = new UTSetup;
    SetupManager = new ConfigManager(setupPrototype, this);
    LoadJsonConfig(Get("SetupProfilePath"), DEFAULT_Setup_PATH, SetupManager);
}

void Platform::SaveSetups()
{
    QJsonDocument jsonDoc(SetupManager->ToJson());
    SaveJsonConfig(jsonDoc, Get("SetupProfilePath"), DEFAULT_Setup_PATH);
}

void Platform::InitMaterials()
{
    Material *materialPrototype = new Material;
    MaterialManager = new ConfigManager(materialPrototype, this);
    LoadJsonConfig(Get("MaterialProfilePath"), DEFAULT_Material_PATH, MaterialManager);
}

void Platform::SaveMaterials()
{
    QJsonDocument jsonDoc(MaterialManager->ToJson());
    SaveJsonConfig(jsonDoc, Get("MaterialProfilePath"), DEFAULT_Material_PATH);
}

void Platform::InitInstruments()
{
    UTInstrument *instrumentPrototype = new UTInstrument;
    InstrumentManager = new ConfigManager(instrumentPrototype, this);
    LoadJsonConfig(Get("InstrumentProfilePath"), DEFAULT_Instrument_PATH, InstrumentManager);
}

void Platform::SaveInstruments()
{
    QJsonDocument jsonDoc(InstrumentManager->ToJson());
    SaveJsonConfig(jsonDoc, Get("InstrumentProfilePath"), DEFAULT_Instrument_PATH);
}

ChannelManager* Platform::GetChannelManager() const
{
    return channelManager;
}

UTSetupSetOne *Platform::GetUTSetupSetOne() const
{
    return utSetupSetOne;
}

void Platform::CreateCardWork()
{
    pCardParam = SharedCardParamVec::create();
    for(int i = 0; i < MAX_CARD_NO; i++)
    {
        SharedCardParam sharedCardParam = SharedCardParam::create();
        pCardParam->append(sharedCardParam);
    }

    int nAecCardInsertNum = GetAecCardNum();
    CPLXCommunication::E_InitResult result = pPLX->Init(pCardParam, nAecCardInsertNum);
    if(result != CPLXCommunication::SUCCESS)
    {
        return;
    }
//    pMfcWork = new MFCWork(this);
    pMfcWork = sharedMFCWork::create();
    pMfcWork->SetCardParam(pCardParam->at(0));
    ThreadController* threadControllerMfc = new ThreadController(this, pMfcWork.objectCast<ThreadWork>());
    Q_UNUSED(threadControllerMfc);

    pAecWork = pointerCardWorkVec::create();
    // 读取AEC卡的数量，一块卡一个AECWORK线程
    qint32 nAecCardNum = pPLX->GetAECCardNum();

//    nAecCardNum = 1;
    for(int i = 0; i < nAecCardNum; i++)
    {
        pCardParam->at(i + 1)->index = i;
        pCardParam->at(i + 1)->pPLX = pPLX;
        pCardParam->at(i + 1)->wSelChannel = probeNoManager->GetBasicNo();
        sharedAecWork aecWork = sharedAecWork::create();

        if(aecWork)
        {
           aecWork->SetCardParam(pCardParam->at(i + 1));
           pAecWork->append(aecWork);
           ThreadController* threadController = new ThreadController(this, aecWork.objectCast<ThreadWork>());
           Q_UNUSED(threadController);
        }
    }

//    sharedAECRuningWork aceRuningWork(new AECRuningWork(0, 30));
    sharedAECRuningWork aecRuningWork = sharedAECRuningWork::create();
    ThreadController* threadController = new ThreadController(this, aecRuningWork.objectCast<ThreadWork>());
    Q_UNUSED(threadController);
}

int Platform::GetDeleteFrameNum()
{
    return Get("DeleteFrameNum").toInt();
}

int Platform::GetCommunicationId()
{
    return Get("CommunicationId").toInt();
}

void Platform::GetVersion()
{
    sharedVersionLower version = sharedVersionLower::create();
    version->aecCardNum = GetAecCardNum();
    version->sensorCardNum = GetSensorCardNum();
    version->isFeatureSingleChannel = FEATURE_SINGLE_CHANNEL;
    version->isFlangeAnalysis = FLANGE_ANALYSIS;
    version->deleteFrames = platform()->GetDeleteFrameNum();
    version->svnVersion = qApp->property("Revision").toInt();
    version->version = qApp->applicationVersion().toFloat();
    emit VersionRead(version);
}

/*!
 * \brief Platform::GetSensorCardNum
 * \return 设置默认值3是因为正常是从第一个探伤卡获取传感器状态,前面的0,1,2是3个传感器卡序号
 */
int Platform::GetSensorCardNum()
{
    return Get("SensorCardIndex").toInt(3);
}

int Platform::GetStraightFlangeCardNum()
{
    return Get("StraightFlangeCardNum").toInt();
}

int Platform::Get21DegreeCardNum()
{
    return Get("21DegreeCardNum").toInt();
}

bool Platform::IsStraightFlangeTwoSensor()
{
    return Get("IsStraightFlangeTwoSensor").toBool();
}

bool Platform::Is21DegreeTwoSensor()
{
    return Get("Is21DegreeTwoSensor").toBool();
}
