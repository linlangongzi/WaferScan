#include "PlatformBase.h"
#include <QDebug>

/* 初始化静态常量 */
const QString PlatformBase::DEFAULT_Platform_PATH = "./Platform.json";
const QJsonDocument::JsonFormat PlatformBase::JSON_FORMAT = QJsonDocument::Indented;

/* 初始化静态变量 */
PlatformBase::PlatformBasePointer PlatformBase::instance;
QMutex PlatformBase::singletonMutex;

PlatformBase::PlatformBase(QObject *parent) :
    ConfigObject(parent)
{
}

PlatformBase::~PlatformBase()
{
    qDebug() << "PlatformBase destruction starting, saving config...";
}

PlatformBase *PlatformBase::GetPlatformBase()
{
    if (instance.isNull())
    {
        QMutexLocker locker(&singletonMutex);                                   /* 只在首次构造Platform时才锁定，避免多线程new多次 */
        instance.reset(new PlatformBase);
        locker.unlock();
        instance->Init();                                                       /* 初始化过程可能会调用PlatformBase()，此时如果mutex不解锁将导致死锁，故将Init与构造分离。 */
    }
    return instance.data();
}

void PlatformBase::Init()
{
    probeNoManager = sharedProbeNoManager::create();
    pPLX = SharedPLXCommunication::create();

    qDebug() << "Loading PlatformBase config...";
    InitPlatform(); /* PlatformBase必须首先初始化，因为初始化其他组件需要读取PlatformBase配置 */

    emit LoadCompleted();
    emit BindCompleted();

    CreateCardWork();
}

void PlatformBase::InitPlatform()
{
    QJsonDocument jsonDoc = LoadJsonConfig(QJsonValue(), DEFAULT_Platform_PATH, NULL);
    if (!jsonDoc.isObject())
    {
        return qCritical("Failed loading PlatformBase config: config is not object");
    }
    FromJson(jsonDoc.object());
    /* 在后面填写Platform的默认值 */
}

void PlatformBase::SavePlatform()
{
    QJsonDocument jsonDoc(ToJson());
    SaveJsonConfig(jsonDoc, QJsonValue(), DEFAULT_Platform_PATH);
}

QJsonDocument PlatformBase::LoadJsonConfig(const QJsonValue &path, const QString &defaultPath, ConfigManager *manager)
{
    QString configPath = path.toString();   /* 默认使用传入的path */
    if (configPath.isEmpty())
    {
        configPath = defaultPath;           /* 如果传入的path不是字符串，使用默认path */
    }
    QFile file(configPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &err);
        if (err.error == QJsonParseError::NoError)
        {
            if (manager == NULL)            /* manager指针为NULL，说明不需要为manager设置json值 */
            {
                return jsonDoc;
            }
            if (jsonDoc.isArray())
            {
                QJsonArray jsonArray = jsonDoc.array();
                manager->FromJson(jsonArray);
                return QJsonDocument();
            }
            else
            {
                qWarning() << QString("Error parsing json object from file %1: JsonDocument is not an array").arg(configPath);
            }
        }
        else
        {
            qWarning() << QString("Error parsing json object from file %1: JsonDocument error '%2' at offset %3").arg(configPath).arg(err.errorString()).arg(err.offset);
        }
    }
    else
    {
        qWarning() << QString("Error opening json config file %1: %2").arg(configPath, file.errorString());
    }
    return QJsonDocument();
}

void PlatformBase::SaveJsonConfig(const QJsonDocument &jsonDoc, const QJsonValue &path, const QString &defaultPath)
{
    QString configPath = path.toString();
    if (configPath.isEmpty())
    {
        configPath = defaultPath;
    }
    QFile file(configPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QByteArray jsonStr = jsonDoc.toJson(JSON_FORMAT);   /* 根据Qt文档，将JsonDocument转为字符串的过程不会失败 */
        qDebug() << QString("Writing json to config file %1").arg(configPath);
        file.write(jsonStr);
    }
    else
    {
        qCritical() << QString("Failed open config file %1: %2").arg(configPath, file.errorString());
    }
}

sharedProbeNoManager PlatformBase::GetProbeNoManager()
{
    return probeNoManager;
}

SharedPLXCommunication PlatformBase::GetPLX()
{
    return pPLX;
}

void PlatformBase::CreateCardWork()
{
    return;
}

pointerCardWorkVec PlatformBase::GetAecWork() const
{
    return pAecWork;
}

sharedCardWork PlatformBase::GetMfcWork() const
{
    return pMfcWork;
}

void PlatformBase::AecWorkInit()
{
    for (CardWorkVector::iterator it = pAecWork->begin(); it != pAecWork->end(); ++it)
    {
        sharedCardWork aecWork = *it;
        aecWork->Init();
    }
}

void PlatformBase::AecWorkStart()
{
    for (CardWorkVector::iterator it = pAecWork->begin(); it != pAecWork->end(); ++it)
    {
        sharedCardWork aecWork = *it;
        aecWork->Start();
    }
}

void PlatformBase::AecWorkStop()
{
    for (CardWorkVector::iterator it = pAecWork->begin(); it != pAecWork->end(); ++it)
    {
        sharedCardWork aecWork = *it;
        aecWork->Stop();
    }
}

bool PlatformBase::IsFlaw() const
{
    return isFlaw;
}

CPLXCommunication::E_InitResult PlatformBase::GetPlxInitResult()
{
    emit PlxInitResult(static_cast<int>(pPLX.data()->GetInitResult()));
    return pPLX.data()->GetInitResult();
}

int PlatformBase::GetAecCardNum()
{
    return Get("AecCardNum").toInt();
}
