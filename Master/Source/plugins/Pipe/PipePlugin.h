#pragma once

#include <extensionsystem/iplugin.h>

namespace Core {

namespace Internal {

class PipePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Pipe.json")

public:
    PipePlugin() = default;
    ~PipePlugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;
};

} // namespace Internal
} // namespace Core
