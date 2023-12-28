#include "TemperatureManager.h"
#include "TemperaturePlugin.h"
#include "TemperatureView.h"
#include <QMessageBox>
#include <Core/ICore.h>
#include <Core/MainWindow.h>
#include <extensionsystem/pluginmanager.h>

using namespace Core::Internal;

static const char G_CONFIG[] = "Temperature";
static const char G_CONFIG_VIEW[] = "TemperatureView";

TemperaturePlugin::TemperaturePlugin()
{
}

TemperaturePlugin::~TemperaturePlugin()
{
}

bool TemperaturePlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    new TemperatureManager(this);
    tempView = new TemperatureView(ICore::mainwindow());
    ICore::mainwindow()->setCentralWidget(tempView);
    return true;
}

void TemperaturePlugin::extensionsInitialized()
{
    QSettings *settings = ExtensionSystem::PluginManager::settings();

    settings->beginGroup(G_CONFIG);
    TemperatureManager::instance()->loadSettings(settings);
    settings->endGroup();

    settings->beginGroup(G_CONFIG_VIEW);
    tempView->loadSettings(settings);
    settings->endGroup();
}

ExtensionSystem::IPlugin::ShutdownFlag TemperaturePlugin::aboutToShutdown()
{
    QSettings *settings = ExtensionSystem::PluginManager::settings();
    settings->beginGroup(G_CONFIG);
    TemperatureManager::instance()->saveSettings(settings);
    settings->endGroup();

    settings->beginGroup(G_CONFIG_VIEW);
    tempView->saveSettings(settings);
    settings->endGroup();

    return SynchronousShutdown;
}
