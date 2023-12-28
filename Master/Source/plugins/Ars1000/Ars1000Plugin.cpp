#include "Ars1000Plugin.h"
#include "Ars1000.h"
#include "PipeInstrumentDriver.h"
using namespace Core::Internal;

bool Ars1000Plugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    new PipeInstrumentDriverFactory(this);

    return true;
}

void Ars1000Plugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag Ars1000Plugin::aboutToShutdown()
{
    return SynchronousShutdown;
}
