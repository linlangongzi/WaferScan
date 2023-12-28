#pragma once

#include <extensionsystem/iplugin.h>

namespace Core {

namespace Internal {

class AcquisitionPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Acquisition.json")

public:
    AcquisitionPlugin() = default;
    ~AcquisitionPlugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;
};

} // namespace Internal
} // namespace Core
