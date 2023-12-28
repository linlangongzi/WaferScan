#pragma once

#include <extensionsystem/iplugin.h>

namespace Core {

namespace Internal {

class Ars500Plugin : public ExtensionSystem::IPlugin
{
public:
    Ars500Plugin() = default;
    ~Ars500Plugin() override = default;

public:
    bool initialize(const QStringList &arguments, QString *errorString = nullptr) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;
};

}
}
