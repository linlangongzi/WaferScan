#pragma once

#include "InstrumentOptions.h"
#include <extensionsystem/iplugin.h>

class UltrasonicManager;
class InstrumentStatus;
namespace Core {

namespace Internal {

class UltrasonicPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Ultrasonic.json")

public:
    UltrasonicPlugin() = default;
    ~UltrasonicPlugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private slots:
    void statusBarMessage(const InstrumentStatus &state);

private:
    UltrasonicManager *manager = nullptr;
};

} // namespace Internal
} // namespace Core
