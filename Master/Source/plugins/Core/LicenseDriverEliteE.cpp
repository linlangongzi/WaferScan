#include "LicenseDriverEliteE.h"
#include <allrising/Util.h>
#include "elepopular.h"
#include "LicenseManager.h"
#include <QCoreApplication>
#include <QLoggingCategory>
#include <QString>
#include <QtEndian>                                                             /* Elite-E为Big-Endian，由于上位机的endianness无法确定，此处使用自动转换 */

static Q_LOGGING_CATEGORY(debug, "core.license.elep");

#define ToDeviceEndian qToBigEndian                                             /*! 将Endianness转为设备Endianness */
#define FromDeviceEndian qFromBigEndian                                         /*! 从设备Endianness转为主机Endianness */

using namespace LicenseDriverInternal;

static const bool driverRegistered = LicenseManager::registerDriver(new LicenseDriverEliteE);

//! 存放RAS公钥的数组. 请确保该公钥与加密锁内存储的RSA私钥相匹配.
const uchar elitePubKey[] = {
    0xBA, 0x9F, 0x32, 0x80, 0xDF, 0x75, 0x69, 0x5C, 0x27, 0x2A, 0xD1, 0xA3, 0x18, 0x02, 0x09, 0x65,
    0xEF, 0xF6, 0x2B, 0xF9, 0xE8, 0x50, 0xDA, 0xED, 0xE2, 0x4D, 0x9B, 0xC9, 0xA0, 0x7B, 0x03, 0xDB,
    0x00, 0xC5, 0x6C, 0xB6, 0x6B, 0xF1, 0x84, 0xC0, 0xCD, 0x31, 0x26, 0xD2, 0x47, 0x8D, 0xCF, 0xCA,
    0x68, 0xF3, 0xC9, 0x00, 0xF7, 0xCC, 0x8B, 0xA3, 0xAC, 0x44, 0x91, 0x40, 0xB8, 0x8E, 0xEF, 0x9A,
    0x28, 0xDA, 0x40, 0x12, 0xFF, 0x08, 0x80, 0xB1, 0x60, 0xAE, 0x34, 0x40, 0x1B, 0x76, 0x06, 0xD6,
    0xDB, 0xC8, 0x02, 0x96, 0xD2, 0x38, 0x2C, 0xB0, 0x68, 0x6F, 0x07, 0xCE, 0xB2, 0x72, 0xF8, 0x06,
    0x50, 0x20, 0x91, 0x4B, 0x84, 0x8B, 0x27, 0x27, 0x08, 0x0A, 0x19, 0x7E, 0x75, 0x3C, 0xA1, 0xCC,
    0x36, 0x8C, 0x94, 0xFE, 0x13, 0x9E, 0xB3, 0xDF, 0x1C, 0xAC, 0x96, 0xCE, 0x1A, 0x48, 0xC1, 0xB5,
    0x00, 0x01, 0x00, 0x01
};
static const int AUC_DATA_SIZE = 1024;                                          /*!< 在对加密锁进行数字签名时，使用的随机填充缓冲区大小 */

static const QStringList defaultPin = {
   "30303030-30303030-30303030-30303030",
   "30303030-aa55aa55-55aa55aa-30303030",
   "30303030-55aa55aa-aa55aa55-30303030",
};
/* ElitE的内存布局:
 * 0x0   - 0x87F (0x880)    存储区
 * 0x9C0 - 0x9CF (0x10)     模块有效标记，每个模块1位
 * 0x9D0 - 0x9DF (0x10)     模块递减性，每个模块1位
 * 0x9E0 - 0xACF (0x100)    模块计数，每个模块2字节，大端存储
 * 0xAE0 - 0xCCF (0x200)    模块到期时间，每个模块4字节，大端存储，仅限时钟版，内容为到1970-01-01的秒数，对应QDateTime::fromTime_t
 */

using DataLengthWord = quint16;                                                 /*!< 数据块之前的长度标记 */
using AllowanceCount = quint16;                                                 /*!< 模块计数字，2字节 */
using AllowanceExpire = quint32;                                                /*!< 模块过期时间，4字节 */

static const int MEMORY_OFFSET = 0x0;                                           /*!< 存储区的偏移。在此应用中，加密锁的前三个区被联合使用，当作只读存储区使用 */
static const int MEMORY_SIZE = 0x880;                                           /*!< 存储区的尺寸 */

static const int MAX_ALLOWANCE_LEVEL = 128;                                     /*!< 总模块数，共128 */
static const int PRIVATE_KEY_OFFSET = 0x880;
static const int PRIVATE_KEY_SIZE = 0x140;
static const int ALLOWANCE_OFFSET = 0x9C0;
static const int ALLOWANCE_SIZE_NORMAL = 0x10 + 0x10 + 0x100;
static const int ALLOWANCE_SIZE_RTL = ALLOWANCE_SIZE_NORMAL + sizeof(AllowanceExpire) * MAX_ALLOWANCE_LEVEL;

Pin parsePin(const QString &pin)
{
    Pin p;
    for (const QString &part: pin.simplified().split('-')) {
        p << part.toULong(nullptr, 16);
    }
    return p;
}

QString fetchSerialNumber(Handle handle)
{
    ULONG32_T serial1 = 0;
    ULONG32_T serial2 = 0;
    ULONG32_T ret = ElepGetDeviceSN(handle, &serial1, &serial2);
//    qCDebug(debug) << "Fetched serial" << handle << ret << serial1 << serial2;
    if (ret != ELEP_SUCCESS) {
        return QString();
    }
    return QString("%1-%2").arg(serial1, 8, 16, QLatin1Char('0')).arg(serial2, 8, 16, QLatin1Char('0'));
}

QDateTime currentTime(Handle handle)
{
    quint32 time;                                                               /* Elite-E 获取时间，返回4字节的32位UNIX时间 */
    ULONG32_T ret = ElepControl(handle, ELEP_GET_CURRENT_TIME, nullptr, 0, reinterpret_cast<uchar *>(&time), sizeof(time), nullptr);
    if (ret == ELEP_SUCCESS) {
        return QDateTime::fromTime_t(time);
    }
    qCCritical(debug, "Failed fetching time from dongle %d: %p", handle, ret);
    return QDateTime();
}

bool rechargeAllowance(Handle handle, const QString &code)
{
    Pin p = parsePin(code);
    ULONG32_T ret = ElepModuleUpdate(handle, p.value(0), p.value(1), p.value(2), p.value(3));
    if (ret == ELEP_SUCCESS) {
        return true;
    }
    ret = ElepExpiredTimeUpdate(handle, p.value(0), p.value(1), p.value(2), p.value(3));
    return ret == ELEP_SUCCESS;
}

QList<Handle> openDevices(const QString &pin = {})
{
    Pin p = parsePin(pin);
    QList<Handle> devices;
    QList<Handle> invalid;
    Handle h;
    ULONG32_T ret = ElepOpenFirst(0, 0, p.value(0), p.value(1), p.value(2), p.value(3), &h);
    while (ret != ELEP_NO_MORE_DEVICE) {
        if (ret == ELEP_SUCCESS) {
            devices << h;
        } else {
            qCDebug(debug) << "Cannot open device" << h << "error" << QString::number(ret, 16);
            invalid << h;
        }
        ret = ElepOpenNext(h, &h);
    }
    for (Handle h: invalid) {
        ElepClose(h);
    }
    return devices;
}

struct DogInfo
{
    wchar_t wcMechineType[32];  // 设备型号
    wchar_t wcMechineNo[32];    // 设备序列号
    wchar_t wcUserCompany[32];  // 客户名称
    quint64 SaleTime;           // 出厂日期 原类型为CTime，根据MSDN，CTime为8字节长
    int nCardNum;               // 卡数
    quint8 FieldValue1[4];      // 模块1,2 IP
    quint8 FieldValue2[4];
    quint8 FieldValue3[4];
    quint8 FieldValue4[4];
    float fXRange;              // 扫查X范围
    float fYRange;              // 扫查Y范围
    float fZRange;              // 扫查Z范围
    int nXCoderPerRingNum;      // X编码器单圈脉冲数
    int nYCoderPerRingNum;      // Y编码器单圈脉冲数
    int nZCoderPerRingNum;      // Z编码器单圈脉冲数
    int nW1CoderPerRingNum;     // W1编码器单圈脉冲数
    int nACoderPerRingNum;      // A编码器单圈脉冲数
    int nBCoderPerRingNum;      // B编码器单圈脉冲数
    int nW2CoderPerRingNum;     // W2编码器单圈脉冲数
    float fXDiameter;           // X编码器直径
    float fYDiameter;           // Y编码器直径
    float fZDiameter;           // Z编码器直径
    float fW1Diameter;          // W1直径
    float fADiameter;           // A直径
    float fBDiameter;           // B直径
    float fW2Diameter;          // W2直径
    float fXJianSuBi;           // X减速比
    float fYJianSuBi;           // Y减速比
    float fZJianSuBi;           // Z减速比
    float fW1JianSuBi;          // W1减速比
    float fAJianSuBi;           // A减速比
    float fBJianSuBi;           // B减速比
    float fW2JianSuBi;          // W2减速比
    BOOL bXReverse;             // X方向反转
    BOOL bYReverse;             // Y方向反转
    BOOL bZReverse;             // Z方向反转
    float fXCorrect;            // X方向修正值
    float fYCorrect;            // Y方向修正值
    float fZCorrect;            // Z方向修正值
    float fW1Correct;           // W1方向修正值
    float fACorrect;            // A方向修正值
    float fBCorrect;            // B方向修正值
    float fW2Correct;           // W2方向修正值
    BOOL bXEnable;              // X轴使能标志
    BOOL bYEnable;              // Y轴使能标志
    BOOL bZEnable;              // Z轴使能标志
    BOOL bAEnable;              // A轴使能标志
    BOOL bBEnable;              // B轴使能标志
    BOOL bW1Enable;             // W1轴使能标志
    BOOL bW2Enable;             // W2轴使能标志
    int nCount;
};

void DongleEliteE::refresh()
{
    dev.deviceId = fetchSerialNumber(handle);
    if (dev.deviceId.isEmpty()) {
        dev.isValid = false;
        return;
    }
    dev.flags = LicenseDevice::DeviceRecharge | LicenseDevice::DevicegenerateRecharge | LicenseDevice::DeviceUpdate;

    const quint32 type = deviceType();
    dev.deviceType = tr("USB加密锁");
    dev.deviceDesc = deviceDescription(type);

    if ((type != ELEP_DEVICE_TYPE_USER_NORMAL && type != ELEP_DEVICE_TYPE_USER_RTL)) {
        return;                                                                 /* 非用户锁，返回一个无效的设备，只用于显示设备类型 */
    }

    const QByteArray rawData = readData(MEMORY_OFFSET, MEMORY_SIZE);
    if (rawData.size() != MEMORY_SIZE) {                                        /* 数据读取失败 */
        return;
    }
    if (type == ELEP_DEVICE_TYPE_USER_RTL) {
        dev.deviceTime = currentTime(handle);
    }
    dev.hostTime = QDateTime::currentDateTime();

    /* 旧水浸加密锁 */
    if (type == ELEP_DEVICE_TYPE_USER_NORMAL && extractUitDevice(rawData)) {
        dev.isValid = false;
        dev.deviceDesc += tr(" (UIT)");
    } else {
        if (type == ELEP_DEVICE_TYPE_USER_NORMAL || type == ELEP_DEVICE_TYPE_USER_RTL) {
            dev.isValid = isAuthentic();
//            if (!dev.isValid) {
//                Reopen();
//            }
        }
        /* 开头的2个字节为长度，强制使用LittleEndian保存 */
        auto size = qFromLittleEndian<quint16>(reinterpret_cast<const uchar *>(rawData.data()));
        if (size == 0 || size > MEMORY_SIZE) {
            size = MEMORY_SIZE;
        }
        const QByteArray data = QByteArray::fromRawData(rawData.data() + 2, size);

        const QJsonObject info = ByteArrayToJson(data, true).toObject();
        dev.licenseCustomer = info["Customer"].toString();
        dev.licenseModel = info["Type"].toString();
        dev.licenseSerial = info["SN"].toString();
        dev.licenseDate = info["ManuDate"].toString();
        dev.config = info["Axes"];

        masterKey = info["MasterKey"].toString();
        if (!masterKey.isEmpty()) {
            dev.deviceDesc += tr(" 含有控制密钥");
        }

        dev.licenseLevels = readLevels(type == ELEP_DEVICE_TYPE_USER_RTL);
    }
}

QList<LicenseLevel> DongleEliteE::readLevels(bool isRtl)
{
    int memorySize = isRtl ? ALLOWANCE_SIZE_RTL : ALLOWANCE_SIZE_NORMAL;
    const QByteArray data = readData(ALLOWANCE_OFFSET, memorySize);

    QList<LicenseLevel> levels;
    const char *enable = data.data();
    //    char *decrease = data.data() + 0x10;
    const auto *count = reinterpret_cast<const AllowanceCount *>(data.data() + 0x20);
    const auto *expire = reinterpret_cast<const AllowanceExpire *>(data.data() + 0x120);
    for (int index = 0; index < MAX_ALLOWANCE_LEVEL; ++index) {
        LicenseLevel level;
        level.level = LicenseLevel::Level(index);
        if (level.GetName().isEmpty()) {
            continue;
        }
        int byteIndex = index / 8;
        int bitMask = 1 << (index % 8);
        bool isEnabled = enable[byteIndex] & bitMask;
        //        bool isDecreasing = decrease[byteIndex] & bitMask;
        auto levelCount = FromDeviceEndian<AllowanceCount>(count[index]);
        level.isValid = isEnabled && (levelCount > 0);
        if (isRtl) {
            auto levelExpire = FromDeviceEndian<AllowanceExpire>(expire[index]);
            if (levelExpire) {
                level.expire = QDateTime::fromTime_t(levelExpire);
            }
        }
        levels << level;
    }
    return levels;
}

bool DongleEliteE::reopen(const QString &pin)
{
    QString serial = fetchSerialNumber(handle);
    qCDebug(debug) << "Reopening" << handle << "serial" << serial;
    if (serial.isEmpty()) {
        qCWarning(debug) << "Reopen dongle failed: device not found" << dev.deviceId;
        return false;
    }
    qCDebug(debug) << "Reopen: closed" << handle << dev.deviceId;
    ElepClose(handle);
    QList<Handle> handles = openDevices(pin);
    Handle newHandle = 0;
    qCDebug(debug) << "Reopen: opened device with pin" << pin << handles;
    for (Handle handle: handles) {
        if (fetchSerialNumber(handle) == serial) {
            qCDebug(debug) << "Reopen: pin matched" << handle << serial;
            newHandle = handle;
        } else {
            qCCritical(debug) << "Reopen: closed wrong device" << handle;
            ElepClose(handle);
        }
    }
    if (newHandle == 0) {
        QList<Handle> handles = openDevices();
        qCDebug(debug) << "Reopen: pin not correct. Reopened" << handles;
        for (Handle handle: handles) {
            if (fetchSerialNumber(handle) == serial) {
                qCDebug(debug) << "Reopen: null pin serial matched" << serial;
                newHandle = handle;
            } else {
                ElepClose(handle);
            }
        }
        handle = newHandle;
        return false;
    }
    handle = newHandle;
    return true;
}

bool DongleEliteE::tryChangePin(const QString &masterKey)
{
    qCDebug(debug) << "Try change pins...";
    for (const QString &p: defaultPin) {
        qCDebug(debug) << "Attempting with" << p;
        if (reopen(p)) {
            Pin pin = parsePin(masterKey);
            quint32 ret = ElepChangePin(handle, pin.value(0), pin.value(1), pin.value(2), pin.value(3));
            qCDebug(debug) << "Change pin from" << p << ": " << ret;
            if (ret == ELEP_SUCCESS) {
                return true;
            }
        }
    }
    return false;
}

quint32 DongleEliteE::deviceType() const
{
    quint32 deviceType = ELEP_DEVICE_TYPE_UNKNOWN;
    const ULONG32_T ret = ElepControl(handle, ELEP_GET_DEVICE_TYPE, nullptr, 0, reinterpret_cast<uchar *>(&deviceType), sizeof(deviceType), nullptr);
    Q_ASSERT(ret == ELEP_SUCCESS);
    Q_UNUSED(ret);
    return deviceType;
}

QString DongleEliteE::versionText() const
{
    quint32 version = ELEP_VERSION_UNKNOWN;
    const ULONG32_T ret = ElepControl(handle, ELEP_GET_FIRMWARE_VER, nullptr, 0, reinterpret_cast<uchar *>(&version), sizeof(version), nullptr);
    if (ret != ELEP_SUCCESS) {
        return tr("未知");
    }
    switch (version) {
    case ELEP_VERSION_V01000000:    return tr("1.0");
    case ELEP_VERSION_V02010000:    return tr("2.1");
    default:                        return tr("未知");
    }
}

QString DongleEliteE::deviceDescription(quint32 type) const
{
    const QString version = versionText();
    switch (type) {
    case ELEP_DEVICE_TYPE_USER_NORMAL:  return tr("标准 (%1)").arg(version);
    case ELEP_DEVICE_TYPE_USER_RTL:     return tr("时钟 (%1)").arg(version);
    case ELEP_DEVICE_TYPE_CONTROL:      return tr("控制 (%1)").arg(version);
    default:                            return tr("未知 (%1)").arg(version);
    }
}

bool DongleEliteE::isAuthentic() const
{
    /* 用随机数据或没有规律的自定义数据填充aucData缓冲区 */
    UCHAR8_T aucData[AUC_DATA_SIZE];
    Q_STATIC_ASSERT_X(RAND_MAX >= UCHAR_MAX, "qrand not capable for filling.");
    for (unsigned char &byte : aucData) {
        byte = qrand();
    }

    // 用于存放数字签名的数组. (128字节的定长数组)
    UCHAR8_T aucSignature[ELEP_LENGTH_SIGNATURE];
    // 对一条随机数据做数字签名
    ULONG32_T ret = ElepSign(handle, aucData, sizeof(aucData), aucSignature);
    if (ret != ELEP_SUCCESS) {
        if (ret != ELEP_VERIFY_SIGNATURE_ERROR) {
            qCWarning(debug, "ElepSign failed on (%d): %x", handle, ret);
        }
        return false;
    }

    ret = ElepVerifySign(elitePubKey, aucData, sizeof(aucData), aucSignature);
    if (ret != ELEP_SUCCESS && ret != ELEP_VERIFY_SIGNATURE_ERROR) {
        qCWarning(debug, "VerifySign failed on (%d): %x", handle, ret);
    }
    return ret == ELEP_SUCCESS;
}

QByteArray DongleEliteE::readData(quint16 offset, quint16 size)
{
    QByteArray data(size, 0);
    ULONG32_T ret = ElepReadMemory(handle, offset, reinterpret_cast<UCHAR8_T *>(data.data()), size);
    if (ret == ELEP_SUCCESS) {
        return data;
    }
    qCWarning(debug, "ReadMemory failed reading on (%d) at %d, length %d: %x", handle, offset, size, ret);
    return QByteArray();
}

bool DongleEliteE::extractUitDevice(const QByteArray &data)
{
    int offset = 0x480;
    if (size_t(data.size()) < (offset + sizeof(DogInfo))) {
        return false;
    }
    const auto *dog = reinterpret_cast<const DogInfo *>(data.data() + offset);
    if (dog->FieldValue1[0] != 192 || dog->FieldValue2[0] != 168) {
        return false;
    }
    dev.licenseModel = QString::fromWCharArray(dog->wcMechineType);
    dev.licenseSerial = QString::fromWCharArray(dog->wcMechineNo);
    dev.licenseCustomer = QString::fromWCharArray(dog->wcUserCompany);
    dev.licenseDate = QDateTime::fromTime_t(dog->SaleTime, Qt::UTC).toString();

    LicenseLevel level;
    level.isValid = true;
    level.level = LicenseLevel::WaferScan;
    dev.licenseLevels = QList<LicenseLevel>() << level;

    return true;
}

bool DongleEliteE::writeData(Handle handle, const QByteArray &data, quint16 offset)
{
    ULONG32_T ret = ElepWriteMemory(handle, offset, reinterpret_cast<const UCHAR8_T *>(data.data()), data.size());
    return ret == ELEP_SUCCESS;
}

bool DongleEliteE::writeLevels(const QList<LicenseLevel> &levels)
{
    bool isRtl = (deviceType() == ELEP_DEVICE_TYPE_USER_RTL);
    QByteArray data(isRtl ? ALLOWANCE_SIZE_RTL : ALLOWANCE_SIZE_NORMAL, '\0');
    char *enable = data.data();
//    char *decrease = data.data() + 0x10;
    auto *count = reinterpret_cast<AllowanceCount *>(data.data() + 0x20);
    auto *expire = reinterpret_cast<AllowanceExpire *>(data.data() + 0x120);

    for (const auto &level: levels) {
        if (!level.isValid) {
            continue;
        }
        int byteIndex = level.level / 8;
        const char bitMask = 1 << (level.level % 8);
        enable[byteIndex] |= bitMask;
        count[level.level] = ToDeviceEndian<AllowanceCount>(1);
        if (isRtl) {
            AllowanceExpire time = level.expire.isValid() ? level.expire.toTime_t() : 0;
            expire[level.level] = ToDeviceEndian<AllowanceExpire>(time);
        }
    }
    qCDebug(debug, "Writing allowance data: %s", data.toHex().constData());
    return writeData(handle, data, ALLOWANCE_OFFSET);
}

////////////////////////////////////////////////////////////////////////////////
LicenseDeviceList LicenseDriverEliteE::enumerate()
{
    while (removeClosed()) {}

    QList<Handle> handles = openDevices();
    for (Handle handle: handles) {
        DongleEliteE dongle(handle);
        dongle.refresh();
        qCInfo(debug) << "New dongle" << dongle.dev.deviceId << dongle.dev;
        dongles << dongle;
    }
    LicenseDeviceList devices;
    for (DongleEliteE &dongle: dongles) {
        auto &hostTime = dongle.dev.hostTime;
        if (!hostTime.isValid() || hostTime.secsTo(QDateTime::currentDateTime()) > 5) {
            dongle.refresh();
        }
        devices << dongle.dev;
    }
    return devices;
}

QString LicenseDriverEliteE::update(const LicenseDevice &newDevice)
{
    DongleEliteE *master = findMaster();
    if (!master) {
        return tr("未找到有效的控制密钥");
    }

    DongleEliteE *target = nullptr;
    for (DongleEliteE &dongle: dongles) {
        if (!dongle.masterKey.isEmpty()) {
            master = &dongle;
        }
        if (fetchSerialNumber(dongle.handle) == newDevice.deviceId) {
            target = &dongle;
        }
    }

    if (!target) {
        return tr("未找到指定的授权设备");
    }

    qCDebug(debug) << "Update: Device found" << target->handle << newDevice.deviceId;

    QJsonObject info;
    info["Customer"] = newDevice.licenseCustomer;
    info["Type"] = newDevice.licenseModel;
    info["SN"] = newDevice.licenseSerial;
    info["ManuDate"] = newDevice.licenseDate;
    info["Axes"] = newDevice.config;
    if (!target->masterKey.isEmpty()) {
        info["MasterKey"] = target->masterKey;
    }
    if (target->masterKey.isEmpty() && newDevice.deviceDesc.contains("密钥")) {
        info["MasterKey"] = master->masterKey;
    }
    if (!target->masterKey.isEmpty() && !newDevice.deviceDesc.contains("密钥")) {
        info.remove("MasterKey");
    }

    QByteArray data = JsonToByteArray(info, false);
    if (data.size() > MEMORY_SIZE) {
        const QByteArray compressed = qCompress(data);
        if (compressed.size() < data.size()) {
            data = compressed;
        }
    }
    if (data.size() > MEMORY_SIZE - 2) {
        return tr("无法写入配置: 存储器容量不足");
    }
    auto size = qToLittleEndian<DataLengthWord>(data.size());
    data.prepend(reinterpret_cast<const char *>(&size), sizeof(size));

    qCDebug(debug) << "Update: Reopening master to read private key";
    if (!master->reopen(master->masterKey)) {
        if (master->tryChangePin(master->masterKey)) {
            return tr("控制锁与控制密钥不匹配");
        }
    }
    QByteArray privateKey = newDevice.isValid ? master->readData(PRIVATE_KEY_OFFSET, PRIVATE_KEY_SIZE) : QByteArray(PRIVATE_KEY_SIZE, '\0');
    qCDebug(debug) << "Update: Finished reading private key, reopening";
    master->reopen();

    qCDebug(debug) << "Update: Reopening target" << target->handle << "with masterKey" << master->masterKey;
    if (!target->reopen(master->masterKey)) {
        /* 尝试将默认PIN(全新的加密锁)更改为正确的PIN */
        qCDebug(debug) << "MasterKey not correct.";
        if (!target->tryChangePin(master->masterKey)) {
            return tr("目标设备与控制密钥不匹配");
        }
    }
    if (privateKey != target->readData(PRIVATE_KEY_OFFSET, PRIVATE_KEY_SIZE)) {
        target->writeData(target->handle, privateKey, PRIVATE_KEY_OFFSET);
    }
    target->writeData(target->handle, data.leftJustified(MEMORY_SIZE, '\0', true), MEMORY_OFFSET);

    target->writeLevels(newDevice.licenseLevels);

    target->reopen();
    target->refresh();

    return QString();
}

QString LicenseDriverEliteE::recharge(const QString &deviceId, const QString &code)
{
    for (const auto &dongle: dongles) {
        if (fetchSerialNumber(dongle.handle) == deviceId) {
            bool ret = rechargeAllowance(dongle.handle, code);
            return ret ? QString() : tr("授权码不正确");
        }
    }
    return tr("未找到指定的授权设备");
}

QString LicenseDriverEliteE::generateRecharge(const QString &deviceId, const LicenseLevel &level)
{
    qCInfo(debug) << "Generating recharge for" << deviceId << level;
    DongleEliteE *master = findMaster();
    if (!master) {
        return tr("未找到有效的控制密钥");
    }
    for (auto &dongle: dongles) {
        if (dongle.deviceType() != ELEP_DEVICE_TYPE_CONTROL) {
            continue;
        }

        const Pin pin = parsePin(deviceId);
        const QByteArray dateText = level.expire.toString("yyyy-MM-dd").toLatin1();
        ULONG32_T code[4] = {0, 0, 0, 0};

        dongle.reopen(master->masterKey);
        ULONG32_T ret = ElepGenModuleUpdateRTC(
            dongle.handle,
            pin.value(0), pin.value(1),
            ushort(level.level), 1, 0,
            dateText.data(),
            &code[0], &code[1], &code[2], &code[3]
        );
        dongle.reopen();

        if (ret != ELEP_SUCCESS) {
            return tr("错误: %1").arg(ret, 0, 16);
        }
        QStringList codeParts;
        for (unsigned long part : code) {
            codeParts << QString("%1").arg(part, 8, 16, QLatin1Char('0')).toUpper();
        }
        return codeParts.join('-');
    }
    return tr("未找到正确的控制锁");
}

DongleEliteE *LicenseDriverEliteE::findMaster()
{
    for (DongleEliteE &dongle: dongles) {
        if (!dongle.masterKey.isEmpty()) {
            return &dongle;
        }
    }
    return nullptr;
}

bool LicenseDriverEliteE::removeClosed()
{
    for (const auto &dongle: dongles) {
        const QString serial = fetchSerialNumber(dongle.handle);
        if (serial.isEmpty()) {
            qCDebug(debug) << "Dongle closed" << dongle.dev.deviceId;
            ElepClose(dongle.handle);
            dongles.removeAll(dongle);
            return true;
        }
    }
    return false;
}
