#include "LicenseDriverLdap.h"
#include "LicenseManager.h"
#include <allrising/Util.h>
#include <utils/executeondestruction.h>
#include <QLoggingCategory>

/* Winldap.h 需要在 Windows.h 之后 */
#include <qt_windows.h>
#include <Winldap.h>

static const bool driverRegistered = LicenseManager::registerDriver(new LicenseDriverLdap);
static Q_LOGGING_CATEGORY(debug, "license.ldap");

/* 服务器的SSL签名未经认证, 将此callback注册到LDAP中, 禁用SSL数字证书 */
static BOOLEAN __cdecl verifySecret(PLDAP connection, PCCERT_CONTEXT *ppServerCert)
{
    Q_UNUSED(connection);
    Q_UNUSED(ppServerCert);
    return TRUE;
}

/*! 读取一个LDAP属性的值 */
static QString readAttribute(LDAP *ld, LDAPMessage *entry, PSTR attribute)
{
    PCHAR *values = ldap_get_valuesA(ld, entry, attribute);
    if (!values) {
        return QString();
    }
    QStringList result;

    int count = ldap_count_valuesA(values);
    for (int index = 0; index < count; ++index) {
        result << QString(values[index]);
    }

    ldap_value_freeA(values);
    return result.join(';');
}

/*! 读取一个LDAP查询结果 */
static QHash<QString, QString> readEntry(LDAP *ld, LDAPMessage *entry)
{
    QHash<QString, QString> values;

    BerElement *element = nullptr;
    PSTR attribute = ldap_first_attributeA(ld, entry, &element);
    while (attribute != nullptr) {
        values[attribute] = readAttribute(ld, entry, attribute);
        ldap_memfreeA(attribute);
        attribute = ldap_next_attributeA(ld, entry, element);
    }
    return values;
}

LicenseDeviceList LicenseDriverLdap::enumerate()
{
    static const QString host("207.allrising.com.cn");
    static const QString dn("uid=%1,cn=users,dc=allrising,dc=com,dc=cn");

    LicenseDeviceList devices;

    /* 服务器禁止匿名访问 */
    if (userName.isEmpty() || password.isEmpty()) {
        return devices;
    }

    const QString userDn = dn.arg(userName);

    /* 使用SSL连接 */
    LDAP *ld = ldap_sslinitA(const_cast<PSTR>(qPrintable(host)), LDAP_SSL_PORT, true);
    if (ld == nullptr) {
        qCWarning(debug) << "Failed creating LDAP connection:" << GetLastError();
        return devices;
    }

    /* 超时时间2秒, 启用SSL, 禁用SSL证书验证, 协议版本3 */
    ULONG timeout = 2;
    ldap_set_option(ld, LDAP_OPT_TIMELIMIT, &timeout);
    ldap_set_option(ld, LDAP_OPT_SSL, LDAP_OPT_ON);
    ldap_set_option(ld, LDAP_OPT_SERVER_CERTIFICATE, reinterpret_cast<void *>(&verifySecret));
    ULONG version = LDAP_VERSION3;
    ldap_set_option(ld, LDAP_OPT_VERSION, &version);

    /* 尝试连接, 并使用密码进行bind */
    ULONG ret = ldap_bind_sA(
        ld,
        const_cast<PSTR>(qPrintable(userDn)),
        const_cast<PCHAR>(qPrintable(password)),
        LDAP_AUTH_SIMPLE
    );
    if (ret != LDAP_SUCCESS) {
        return devices;
    }
    Utils::ExecuteOnDestruction unbind([&]() {
        ldap_unbind_s(ld);
    });
    Q_UNUSED(unbind);

    /* 查询LDAP服务器, 获取帐号过期时间, 及授权类型(写在employeeType中) */
    LDAPMessage *result = nullptr;
    const char *attributes[] = {
        "shadowExpire",
        "employeeType",
        nullptr
    };
    ret = ldap_search_sA(
        ld,
        const_cast<const PSTR>(qPrintable(userDn)),
        LDAP_SCOPE_BASE,
        nullptr,
        const_cast<PZPSTR>(attributes),
        0,
        &result
    );
    if (ret != LDAP_SUCCESS) {
        qCWarning(debug) << "Failed searching ldap server:" << ret;
        return devices;
    }
    /* 由于指定了uid, 只查看第一个结果即可 */
    LDAPMessage *entry = ldap_first_entry(ld, result);
    QHash<QString, QString> values = readEntry(ld, entry);
    ldap_msgfree(result);

    /* 解析帐号过期时间 (shadowExpire) */
    qlonglong expireTime = values["shadowExpire"].toLongLong();
    QDateTime expire;
    if (expireTime >= 0) {      /*!< 过期时间为1970-1-1之后的天数, -1表示永远不过期, 参见RFC 2307 */
        expire = QDateTime(QDate(1970, 1, 1).addDays(expireTime), QTime(), Qt::LocalTime);
    }

    /* 生成授权 */
    LicenseDevice device;
    device.isValid = true;
    device.deviceId = host;
    device.deviceType = tr("内部员工授权");
    device.deviceDesc = tr("内部员工在线授权服务器");
    device.licenseCustomer = userName;

    QStringList levels = values["employeeType"].split(',', QString::SkipEmptyParts);
    for (auto l: EnumValueList<LicenseLevel::Level>()) {
        if (l != LicenseLevel::Invalid) {
            LicenseLevel level;
            level.isValid = levels.isEmpty() || levels.contains(EnumToKey(l));
            level.level = l;
            level.expire = expire;
            device.licenseLevels << level;
        }
    }
    devices << device;

    return devices;
}

void LicenseDriverLdap::setCredential(const QString &user, const QString &pass)
{
    userName = user;
    password = pass;
}
