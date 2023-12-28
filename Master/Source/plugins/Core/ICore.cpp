#include "ICore.h"
#include <extensionsystem/pluginmanager.h>
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include "LicenseManager.h"

using namespace ExtensionSystem;
using namespace Core;

// The Core Singleton
static ICore *m_instance = nullptr;
static MainWindow *m_mainwindow = nullptr;
static QSqlDatabase m_database;
static QString m_settingsPath;
//static QSettings m_settings;

static Q_LOGGING_CATEGORY(debug, "core");

ICore::ICore(MainWindow *parent)
{
    Q_ASSERT(m_instance == nullptr);

    m_instance = this;
    m_mainwindow = parent;
}

ICore *ICore::instance()
{
    return m_instance;
}

QSettings *ICore::settings(QSettings::Scope scope)
{
    if (scope != QSettings::SystemScope) {
        return PluginManager::settings();
    } else {
        return PluginManager::globalSettings();
    }
}

QString ICore::settingsPath()
{
    if (m_settingsPath.isEmpty()) {
        m_settingsPath = QFileInfo(settings()->fileName()).dir().path();
    }
    return m_settingsPath;
}

QString ICore::resourcePath()
{
#ifdef Q_OS_MAC
    return qApp->applicationDirPath() + "/../Resources";
#else
    return qApp->applicationDirPath() + "/resources";
#endif
}

QString ICore::documentPath()
{
    return settings()->value("Core/documentPath").toString();
}

QWidget *ICore::dialogParent()
{
    QWidget *active = QApplication::activeModalWidget();
    if (!active) {
        active = QApplication::activeWindow();
    }
    if (!active) {
        active = m_mainwindow;
    }
    return active;
}

MainWindow *ICore::mainWindow()
{
    return m_mainwindow;
}

QSqlDatabase ICore::database()
{
    return m_database;
}

void ICore::reopenDatabase()
{
    QSettings *s = settings();
    s->beginGroup("Database");
    QString type = s->value("type", "QSQLITE").toString();
    QString host = s->value("host").toString();
    QString name = s->value("name", ICore::settingsPath() + "/Allrsing.db").toString();
    QString user = s->value("user").toString();
    QByteArray passwordRaw = s->value("password").toByteArray();
    QString password = passwordRaw.isEmpty() ? QString() : QString::fromUtf8(qUncompress(passwordRaw));
    s->endGroup();

    if (!type.isEmpty() && !QSqlDatabase::drivers().contains(type, Qt::CaseInsensitive)) {
        qCWarning(debug) << "Invalid SQL driver type" << type;
        type = "QSQLITE";
    }
    m_database.close();
    m_database = QSqlDatabase();
    QSqlDatabase::removeDatabase("AllrisingDB");
    m_database = QSqlDatabase::addDatabase(type, "AllrisingDB");
    QUrl url = QUrl::fromUserInput(host);
    m_database.setHostName(url.host());
    m_database.setPort(url.port());
    m_database.setDatabaseName(name);
    m_database.open(user, password);
}

bool ICore::isInternalVersion()
{
    return LicenseManager::instance()->hasDevelopLicense();
}

void ICore::saveSettings()
{
    PluginManager::settings()->sync();
//    PluginManager::globalSettings()->sync();
}
