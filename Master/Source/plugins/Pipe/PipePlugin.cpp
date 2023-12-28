#include "PipeManager.h"
#include "PipePlugin.h"
#include <Core/ICore.h>

using namespace Core::Internal;

bool PipePlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    auto *mw = Core::ICore::mainWindow();
    mw->setCentralWidget(new PipeManager(mw));

    return true;
}

void PipePlugin::extensionsInitialized()
{

}
