#pragma once

#include <extensionsystem/iplugin.h>

class LicenseManager;

namespace Core {

class MainWindow;

namespace Internal {

class CorePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Core.json")

public:
    CorePlugin() = default;
    ~CorePlugin() override;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private:
    Core::MainWindow *mainwindow = nullptr;
    LicenseManager *licenseManager = nullptr;
};

} // namespace Internal
} // namespace Core
