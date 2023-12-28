#pragma once

#include <extensionsystem/iplugin.h>

namespace Core {

namespace Internal {

class Ars3000Plugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Ars3000.json")

public:
    Ars3000Plugin() = default;
    ~Ars3000Plugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;

    ShutdownFlag aboutToShutdown() override;
};

} // namespace Internal
} // namespace Core
