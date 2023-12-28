#include "UltrasonicPlugin.h"
#include "InstrumentStatusButton.h"
#include "UltrasonicManager.h"
#include "UltrasonicOptions.h"
#include "ChannelOptions.h"
#include <Core/ICore.h>
#include <QLabel>
#include <QStatusBar>
#include <extensionsystem/pluginmanager.h>

using namespace Core::Internal;
using namespace ExtensionSystem;

static Q_LOGGING_CATEGORY(debug, "ut.plugin")

bool UltrasonicPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);
    manager = new UltrasonicManager(this);
    Core::ICore::mainWindow()->statusBar()->addPermanentWidget(new InstrumentStatusButton);

    new UltrasonicOptionsPage(this);
    new InstrumentOptionsPage(this);
    new ChannelOptionsPage(this);

    return true;
}

void UltrasonicPlugin::extensionsInitialized()
{
    manager->loadConfig();

    const auto instruments = UltrasonicManager::instruments();
    for (UTInstrument *instrument: instruments) {
        connect(instrument, &UTInstrument::instrumentStatusChanged, this, &UltrasonicPlugin::statusBarMessage);
    }
}

IPlugin::ShutdownFlag UltrasonicPlugin::aboutToShutdown()
{
    manager->saveConfig();
    return SynchronousShutdown;
}

void UltrasonicPlugin::statusBarMessage(const InstrumentStatus &state)
{
    auto *instrument = qobject_cast<UTInstrument *>(sender());
    if (!state.errorString.isEmpty() && instrument) {
        Core::ICore::mainWindow()->statusBar()->showMessage(tr("%1: %2").arg(instrument->name(), state.errorString));
    }
}
