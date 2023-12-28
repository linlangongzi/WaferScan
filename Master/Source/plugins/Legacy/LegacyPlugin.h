#pragma once

#include <extensionsystem/iplugin.h>

namespace Core {

namespace Internal {

class LegacyPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Legacy.json")

public:
    LegacyPlugin() = default;
    ~LegacyPlugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;

private slots:
    void onImportWorkspace();
    void onExportWorkspace();
};

} // namespace Internal
} // namespace Core
