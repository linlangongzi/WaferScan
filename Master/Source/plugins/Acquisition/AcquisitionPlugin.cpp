#include "AcquisitionPlugin.h"
#include "PanelScanPlanControl.h"
#include "ScanController.h"
#include "ScanPathAdvanced.h"
#include "ScanPathCylinder.h"
#include "ScanPathDisk.h"
#include "ScanPathRectangle.h"
#include "ScanPathRing.h"
#include "ScanPathRingSettings.h"
#include "ScanSettings.h"
#include <Core/ICore.h>
#include <Core/LicenseManager.h>
#include <Core/actionmanager/actioncontainer.h>
#include <Core/actionmanager/actionmanager.h>
#include <Motion/PanelAxisControl.h>
#include <QMessageBox>
#include <allrising/Util.h>

using namespace Core::Internal;

bool AcquisitionPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    MainWindow *mw = ICore::mainWindow();

    new ScanPathRectangle(this);
    new ScanPathCylinder(this);
    new ScanPathDisk(this);
    new ScanPathAdvanced(this);
    new ScanPathRing(this);

    new ScanController(this);
    new ScanPlanModel(this);

    new ScanSettingsPage(this);
    new ScanPathRingSettingsPage(this);

    if (LicenseManager::instance()->hasProductLicense()) {
        auto *planControl = new PanelScanPlanControl(mw);
        mw->AddPanelToDockWidget(planControl, Qt::RightDockWidgetArea, tr("扫描控制"), "ScanController");
    }

    mw->addPreCloseListener([mw] () {
        ScanController *controller = ScanController::instance();
        if (controller) {
            auto type = controller->currentState();
            if (type != ScanController::IDLE) {
                QMessageBox::warning(mw, QString(), tr("扫查未结束"));
                return false;
            }
        }
        return true;
    });

    ActionContainer *viewMenu = ActionManager::actionContainer(Constants::M_VIEW);
    viewMenu->addSeparator(Constants::G_VIEW_PANEL);

    return true;
}

void AcquisitionPlugin::extensionsInitialized()
{
    ScanPlanModel::instance()->loadSettings();
}

ExtensionSystem::IPlugin::ShutdownFlag AcquisitionPlugin::aboutToShutdown()
{
    ScanPlanModel::instance()->saveSettings();
    return SynchronousShutdown;
}
