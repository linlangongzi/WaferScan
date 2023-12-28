#include "Mdo3000.h"
#include "Mdo3000Plugin.h"
#include <Ultrasonic/UltrasonicManager.h>

using namespace Core::Internal;

bool Mdo3000Plugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    new Mdo3000Factory(this);

    return true;
}

void Mdo3000Plugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag Mdo3000Plugin::aboutToShutdown()
{
    return SynchronousShutdown;
}
