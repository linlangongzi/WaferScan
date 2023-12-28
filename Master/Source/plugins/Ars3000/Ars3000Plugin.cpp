#include "Ars3000.h"
#include "Ars3000Plugin.h"

using namespace Core::Internal;

bool Ars3000Plugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    new Ars3000Factory(this);

    return true;
}

void Ars3000Plugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag Ars3000Plugin::aboutToShutdown()
{
    return SynchronousShutdown;
}
