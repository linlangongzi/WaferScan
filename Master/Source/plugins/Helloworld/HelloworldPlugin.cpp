#include "HelloworldPlugin.h"

using namespace Core::Internal;

bool HelloworldPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);
    return true;
}

void HelloworldPlugin::extensionsInitialized()
{

}
