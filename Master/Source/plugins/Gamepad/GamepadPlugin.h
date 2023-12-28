#pragma once

#include <extensionsystem/iplugin.h>

namespace Core {

namespace Internal {

class GamepadPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Gamepad.json")

public:
    GamepadPlugin() = default;
    ~GamepadPlugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;
};

} // namespace Internal
} // namespace Core
