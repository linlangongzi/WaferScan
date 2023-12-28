#pragma once

#include <extensionsystem/iplugin.h>

namespace Core {

namespace Internal {

class HelloworldPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Helloworld.json")

public:
    HelloworldPlugin() = default;
    ~HelloworldPlugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;
};

} // namespace Internal
} // namespace Core
