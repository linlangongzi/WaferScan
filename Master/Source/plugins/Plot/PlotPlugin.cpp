#include "LocalScan.h"
#include "PlotPlugin.h"
#include <Core/ICore.h>
#include <Core/MainWindow.h>
#include <Core/LicenseManager.h>

using namespace Core::Internal;

bool PlotPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    auto *mw = ICore::mainWindow();
    if (LicenseManager::instance()->hasProductLicense()) {
        mw->AddPanelToDockWidget(new LocalScan(mw), Qt::RightDockWidgetArea, tr("局部扫描"), "LocalScan", false);
    }

    return true;
}

void PlotPlugin::extensionsInitialized()
{

}
