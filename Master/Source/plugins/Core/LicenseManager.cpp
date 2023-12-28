#include "LicenseManager.h"
#include "LicenseDriver.h"
#include <QElapsedTimer>
#include <QLoggingCategory>
#include <QPointer>

static Q_LOGGING_CATEGORY(debug, "core.license");

Q_GLOBAL_STATIC(QList<LicenseDriver *>, licenseManagerDevices)
QPointer<LicenseManager> LicenseManager::m_instance = nullptr;
const int licenseLevelTypeId = qRegisterMetaType<LicenseLevel>();
const int licenseDeviceListTypeId = qRegisterMetaType<LicenseDeviceList>();

LicenseManager::WorkerThread::WorkerThread(QObject *parent) :
    QThread(parent)
{}

void LicenseManager::WorkerThread::run()
{
    QElapsedTimer time;
    while (!isInterruptionRequested()) {
        time.start();

        /* 更新设备列表，发送给LicenseManager */
        LicenseDeviceList devices;
        for (LicenseDriver *driver: *licenseManagerDevices) {
            LicenseDeviceList dev = driver->enumerate();
            for (auto &device: dev) {
                device.driver = driver;
            }
            devices << dev;
        }
        QMetaObject::invokeMethod(parent(), "OnDevicesUpdated", Qt::QueuedConnection, Q_ARG(LicenseDeviceList, devices));

        /* 处理跨线程调用 */
        QCoreApplication::processEvents();

        /* 节流 */
        qint64 msecRemaining = 500 - time.elapsed();
        if (msecRemaining > 0 && !isInterruptionRequested()) {
            msleep(ulong(msecRemaining));
        }
    }
    qCDebug(debug) << "Worker finished";
}

LicenseManager::LicenseManager(QObject *parent) :
    QObject(parent),
    worker(new WorkerThread(this))
{
    m_productLevel = LicenseLevel::WaferScan;
    m_developLevel = LicenseLevel::WaferScanDev;

    m_instance = this;
}

LicenseManager::~LicenseManager()
{
    worker->requestInterruption();
    worker->wait();
    delete worker;
}

LicenseDeviceList LicenseManager::devices()
{
    return m_devices;
}

LicenseDevice LicenseManager::device(const QString &deviceId) const
{
    foreach (const LicenseDevice &device, m_devices) {
        if (device.deviceId == deviceId) {
            return device;
        }
    }
    return LicenseDevice();
}

LicenseDevice LicenseManager::currentDevice() const
{
    return device(m_currentDevId);
}

QString LicenseManager::currentDeviceId() const
{
    return m_currentDevId;
}

bool LicenseManager::hasDevelopLicense() const
{
    foreach (const LicenseDevice &dev, m_devices) {
        if (dev.HasValidLevel(m_developLevel)) {
            return true;
        }
    }
    return false;
}

bool LicenseManager::currentDeviceHasLevel(LicenseLevel::Level level) const
{
    LicenseDevice dev = currentDevice();
    return dev.HasValidLevel(level);
}

bool LicenseManager::hasProductLicense() const
{
    return currentDeviceHasLevel(m_productLevel);
}

LicenseLevel LicenseManager::productLicenseLevel() const
{
    return currentDevice().GetLevel(m_productLevel);
}

QDateTime LicenseManager::productLicenseExpire() const
{
    return currentDevice().GetLevel(m_productLevel).expire;
}

QDateTime LicenseManager::lastSeenValid() const
{
    return m_lastSeenValid;
}

bool LicenseManager::pickUserLicense()
{
    LicenseDeviceList valid;
    LicenseDeviceList expired;
    foreach (const LicenseDevice &device, m_devices) {
        if (device.HasValidLevel(m_productLevel)) {
            valid << device;
        }
        if (device.HasValidLevel(m_productLevel, true)) {
            expired << device;
        }
    }
    if (valid.isEmpty() && expired.isEmpty()) {                                 /* 没有任何有效授权设备 */
        return false;
    }

    for (const LicenseDevice &device: valid) {                                  /* 选择第一个非开发权限的设备 */
        if (!device.HasValidLevel(m_developLevel)) {
            m_currentDevId = device.deviceId;
            qCInfo(debug) << "Picking user license" << device.deviceId;
            return true;
        }
    }

    if (!valid.isEmpty()) {
        m_currentDevId = valid.first().deviceId;                                /* 选择第一个设备 */
        qCInfo(debug) << "Picking user license" << m_currentDevId;
    } else {
        m_currentDevId = expired.first().deviceId;                              /* 或第一个过期的设备 */
        qCInfo(debug) << "Picking expired user license" << m_currentDevId;
    }
    return true;
}

bool LicenseManager::registerDriver(LicenseDriver *driver)
{
    if (licenseManagerDevices->contains(driver)) {
        return false;
    }
    *licenseManagerDevices << driver;
    return true;
}

LicenseManager *LicenseManager::instance()
{
    return m_instance;
}

void LicenseManager::initialize()
{
    foreach (LicenseDriver *driver, *licenseManagerDevices) {
        driver->init();
        LicenseDeviceList dev = driver->enumerate();
        for (auto &device: dev) {
            device.driver = driver;
        }
        m_devices << dev;
        driver->moveToThread(worker);
    }

    worker->start(QThread::LowPriority);
}

void LicenseManager::setProductLevel(LicenseLevel::Level level)
{
    m_productLevel = level;
}

void LicenseManager::setDevelopLevel(LicenseLevel::Level level)
{
    m_developLevel = level;
}

void LicenseManager::OnDevicesUpdated(const LicenseDeviceList &newDevices)
{
    if (newDevices == m_devices) {
        return;
    }
    m_devices = newDevices;
    if (hasProductLicense()) {
        m_lastSeenValid = QDateTime::currentDateTime();
    }
    emit licenseUpdated();
}

QString LicenseManager::updateDevice(const LicenseDevice &d)
{
    if (!hasDevelopLicense()) {
        return tr("权限不足");
    }
    const LicenseDevice dev = device(d.deviceId);
    if (dev.deviceId.isEmpty() || dev.deviceId != d.deviceId) {
        return tr("未找到指定授权设备");
    }

    qCDebug(debug) << "Invoking update on" << dev.driver;
    QString ret;
    QMetaObject::invokeMethod(
        dev.driver, "update", Qt::BlockingQueuedConnection,
        Q_RETURN_ARG(QString, ret),
        Q_ARG(LicenseDevice, d)
    );
    return ret;
}

QString LicenseManager::rechargeDevice(const QString &deviceId, const QString &code)
{
    const LicenseDevice dev = device(deviceId);
    if (dev.deviceId.isEmpty() || dev.deviceId != deviceId) {
        return tr("未找到指定授权设备");
    }

    QString ret;
    QMetaObject::invokeMethod(
        dev.driver, "recharge", Qt::BlockingQueuedConnection,
        Q_RETURN_ARG(QString, ret),
        Q_ARG(QString, deviceId),
        Q_ARG(QString, code)
    );
    return ret;
}

QString LicenseManager::generateRecharge(const QString &deviceId, const QString &targetDevice, const LicenseLevel &level)
{
    const LicenseDevice dev = device(deviceId);
    if (dev.deviceId.isEmpty() || dev.deviceId != deviceId) {
        return tr("未找到指定授权设备");
    }

    QString ret;
    QMetaObject::invokeMethod(
        dev.driver, "generateRecharge", Qt::BlockingQueuedConnection,
        Q_RETURN_ARG(QString, ret),
        Q_ARG(QString, targetDevice),
        Q_ARG(LicenseLevel, level)
    );
    return ret;
}

void LicenseManager::setCredential(const QString &userName, const QString &password)
{
    for (LicenseDriver *driver: *licenseManagerDevices) {
        QMetaObject::invokeMethod(driver, "setCredential", Qt::QueuedConnection,
            Q_ARG(QString, userName),
            Q_ARG(QString, password)
        );
    }
}
