#pragma once

#include <QThread>
#include "LicenseDevice.h"
#include "CoreGlobal.h"

class LicenseDriver;

/*!
 * \brief LicenseManager 是一个授权管理器，负责在后台轮询各个授权设备，监视授权信息，并提供查询
 * 服务。
 *
 * LicenseManager 会维护一个授权设备 (LicenseDevice) 的列表。
 * 当列表更新时，会抛出 LicenseUpdated() 信号。
 *
 * 每个授权设备表达一个物理或者逻辑设备，例如网络授权服务器、USB加密锁等。
 *
 * 授权可以有多个来源，通过注册 LicenceDriver 的方式进行扩展。
 */
class CORE_EXPORT LicenseManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(LicenseDeviceList devices READ devices)
//    Q_PROPERTY(LicenseLevel::Level productLevel READ productLevel WRITE setProductLevel)

public:
    explicit LicenseManager(QObject *parent = nullptr);
    ~LicenseManager();

    LicenseDeviceList devices();
    LicenseDevice device(const QString &deviceId) const;
    LicenseDevice currentDevice() const;
    QString currentDeviceId() const;
    bool currentDeviceHasLevel(LicenseLevel::Level level) const;
    bool hasProductLicense() const;
    LicenseLevel productLicenseLevel() const;
    QDateTime productLicenseExpire() const;
    QDateTime lastSeenValid() const;
    /*! 检测是否有开发授权 */
    bool hasDevelopLicense() const;

    bool pickUserLicense();

    static bool registerDriver(LicenseDriver *driver);
    static LicenseManager *instance();

    template <typename Obj, typename Func>
    static void registerLicenseUpdate(Obj *obj, Func func);

    void initialize();

signals:
    void licenseUpdated();

public slots:
    void setProductLevel(LicenseLevel::Level level);
    void setDevelopLevel(LicenseLevel::Level level);
    void OnDevicesUpdated(const LicenseDeviceList &newDevices);
    QString updateDevice(const LicenseDevice &d);
    QString rechargeDevice(const QString &deviceId, const QString &code);
    QString generateRecharge(const QString &deviceId, const QString &targetDevice, const LicenseLevel &level);
    void setCredential(const QString &userName, const QString &password);

private:
    class WorkerThread;
    WorkerThread *worker;

    LicenseDeviceList m_devices;
    QString m_currentDevId;
    LicenseLevel::Level m_productLevel;
    LicenseLevel::Level m_developLevel;
    QDateTime m_lastSeenValid;
    static QPointer<LicenseManager> m_instance;
};

/*! LicenseManager内部的工作线程，负责轮询设备列表，并发送给LicenseManager */
class LicenseManager::WorkerThread : public QThread
{
    Q_OBJECT

public:
    WorkerThread(QObject *parent);

protected:
    virtual void run() override;
};

template <typename Obj, typename Func>
void LicenseManager::registerLicenseUpdate(Obj *obj, Func func)
{
    connect(instance(), &LicenseManager::licenseUpdated, obj, func);
    (obj->*func)();
}
