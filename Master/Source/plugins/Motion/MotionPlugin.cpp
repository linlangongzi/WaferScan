#include "MotionManager.h"
#include "MotionOptions.h"
#include "MotionPlugin.h"
#include "PanelAxisControl.h"
#include "PanelMotionConsole.h"
#include <Core/ICore.h>
#include <Core/LicenseManager.h>

using namespace Core;
using namespace Core::Internal;
using namespace ExtensionSystem;
using namespace Motion;

bool MotionPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    motionManager = new MotionManager(this);

    if (LicenseManager::instance()->hasProductLicense()) {
        MainWindow *mw = ICore::mainWindow();
        mw->AddPanelToDockWidget(new PanelAxisControl(mw), Qt::LeftDockWidgetArea, tr("轴控制器"), "AxisController", LicenseManager::instance()->hasProductLicense());           /*!< 轴控制器面板 */
        mw->AddPanelToDockWidget(new PanelMotionConsole(mw), Qt::RightDockWidgetArea, tr("运动控制器调试"), "MotionConsole", false);
    }
    new MotionOptionsPage(this);

    return true;
}

void MotionPlugin::extensionsInitialized()
{
    motionManager->loadConfig();
}

ExtensionSystem::IPlugin::ShutdownFlag MotionPlugin::aboutToShutdown()
{
    motionManager->saveConfig();

    return SynchronousShutdown;
}
