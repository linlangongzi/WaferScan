#pragma once

#include <extensionsystem/iplugin.h>

class TemperatureView;

namespace Core {

namespace Internal {

class TemperaturePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Temperature.json")

public:
    TemperaturePlugin();
    ~TemperaturePlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage = 0) Q_DECL_OVERRIDE;
    void extensionsInitialized() Q_DECL_OVERRIDE;
    ShutdownFlag aboutToShutdown() Q_DECL_OVERRIDE;

private:
    TemperatureView *tempView;
};

} // namespace Internal
} // namespace Core
