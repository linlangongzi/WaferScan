#include "GamepadPlugin.h"
#include "GamepadConstants.h"
#include "GamepadSettings.h"
#include <Core/ICore.h>
#include <Motion/MotionManager.h>

using namespace Gamepad;
using namespace Core::Internal;

bool GamepadPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    new GamepadController(this);

    new GamepadSettingsPage(this);

    return true;
}

void GamepadPlugin::extensionsInitialized()
{
    QSettings *settings = ICore::settings();
    settings->beginGroup(K_GAMEPAD_SETTINGS_GROUP);
    auto options = settings->value(K_GAMEPAD_OPTIONS).value<GamepadOptions>();
    GamepadController::instance()->setOptions(options);
    settings->endGroup();
}

ExtensionSystem::IPlugin::ShutdownFlag GamepadPlugin::aboutToShutdown()
{
    QSettings *settings = ICore::settings();
    settings->beginGroup(K_GAMEPAD_SETTINGS_GROUP);
    auto options = GamepadController::instance()->options();
    settings->setValue(K_GAMEPAD_OPTIONS, QVariant::fromValue(options));
    settings->endGroup();

    MotionManager::driver()->cancel();

    return SynchronousShutdown;
}
