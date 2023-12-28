#pragma once

#include <extensionsystem/iplugin.h>

class QAction;
class WaveformManager;

namespace Core {

namespace Internal {

class WaveformPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Waveform.json")

public:
    WaveformPlugin() = default;
    ~WaveformPlugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private:
    WaveformManager *manager = nullptr;
};

} // namespace Internal
} // namespace Core
