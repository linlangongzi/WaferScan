#pragma once

#include <extensionsystem/iplugin.h>

namespace Core {

namespace Internal {

class ImagingPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Imaging.json")

public:
    ImagingPlugin() = default;
    ~ImagingPlugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;
};

} // namespace Internal
} // namespace Core
