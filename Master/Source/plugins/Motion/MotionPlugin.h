#pragma once

#include <extensionsystem/iplugin.h>

class MotionManager;

namespace Core {

namespace Internal {

class MotionPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "cn.com.allrising.dolphinsense.plugin" FILE "Motion.json")

public:
    MotionPlugin() = default;
    ~MotionPlugin() override = default;

    bool initialize(const QStringList &arguments, QString *errorMessage = nullptr) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private:
    MotionManager *motionManager = nullptr;
};

} // namespace Internal
} // namespace Core
