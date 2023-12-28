#pragma once

#include <extensionsystem/iplugin.h>

namespace Core {

namespace Internal {

class PlotPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Plot.json")

public:
    PlotPlugin() = default;
    ~PlotPlugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;
};

} // namespace Internal
} // namespace Core
