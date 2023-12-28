#include "Ars500Plugin.h"
#include "ARS500.h"

using namespace Core::Internal;

bool Core::Internal::Ars500Plugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    new Ars500Factory(this);

    return true;
}

void Core::Internal::Ars500Plugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag Core::Internal::Ars500Plugin::aboutToShutdown()
{
    return SynchronousShutdown;
}
