#pragma once

#include <extensionsystem/iplugin.h>

namespace Core {

namespace Internal {

class Ars1000Plugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
  //  Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Ars1000.json")

public:
    Ars1000Plugin() = default;
    ~Ars1000Plugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;
};

} // namespace Internal
} // namespace Core
