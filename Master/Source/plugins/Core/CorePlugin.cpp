#include "CorePlugin.h"
#include "DocumentManager.h"
#include "GeneralSettings.h"
#include "ICore.h"
#include "LicenseManager.h"
#include "MainWindow.h"
#include "Platform.h"
#include "PluginDialog.h"
#include "actionmanager/actionmanager.h"
#include "dialogs/shortcutsettings.h"
#include <extensionsystem/pluginmanager.h>
#include <QMessageBox>

using Core::ICore;
using namespace Core::Internal;
using namespace ExtensionSystem;

static const QString P_PLATFORM = "./Platform.json";

CorePlugin::~CorePlugin()
{
    delete mainwindow;
}

bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    auto *p = new Platform(this);
    p->FromJson(ReadFileAsJson(P_PLATFORM).toObject());

    PluginManager::splashMessage(tr("初始化数据库连接..."));
    ICore::reopenDatabase();

    licenseManager = new LicenseManager(this);
#ifdef PIPE_DETECT
    licenseManager->setProductLevel(LicenseLevel::Pipe);
#elif defined(TRAINWHEEL)
    licenseManager->setProductLevel(LicenseLevel::Train);
#endif
    if (!arguments.contains("-viewonly")) {
        licenseManager->initialize();
        licenseManager->pickUserLicense();
    }
    if (!licenseManager->hasProductLicense() && !arguments.contains("-viewonly")) {
        DialogAbout about;
        about.exec();
        *errorMessage = tr("未找到有效的授权");
        return false;
    }

    new ActionManager(this);
//    new DocumentManager(this);
    mainwindow = new MainWindow;

    new GeneralSettingsPage(this);
    new ShortcutSettings(this);

    return true;
}

void CorePlugin::extensionsInitialized()
{
    mainwindow->initUi();

    QSettings *settings = ICore::settings();
    settings->beginGroup("MainWindow");
    mainwindow->restoreSettings(settings);
    settings->endGroup();

    mainwindow->show();

    QDateTime expire = licenseManager->productLicenseExpire();
    qint64 days = QDateTime::currentDateTime().daysTo(expire);
    if (days <= 7 && days > 0) {
        QMessageBox::warning(mainwindow, QString(), tr("您的使用授权将在 %1 天后过期").arg(days));
    }
}

ExtensionSystem::IPlugin::ShutdownFlag CorePlugin::aboutToShutdown()
{
    WriteFileAsJson(P_PLATFORM, platform()->ToJson());

    QSettings *settings = ICore::settings();
    settings->beginGroup("MainWindow");
    mainwindow->saveSettings(settings);
    settings->endGroup();

    return SynchronousShutdown;
}
