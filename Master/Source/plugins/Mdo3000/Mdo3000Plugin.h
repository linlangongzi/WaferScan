#pragma once

#include <extensionsystem/iplugin.h>

namespace Core {

namespace Internal {

class Mdo3000Plugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Mdo3000.json")

public:
    Mdo3000Plugin() = default;
    ~Mdo3000Plugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;

    // IPlugin interface
public:
    ShutdownFlag aboutToShutdown() override;
};

} // namespace Internal
} // namespace Core
