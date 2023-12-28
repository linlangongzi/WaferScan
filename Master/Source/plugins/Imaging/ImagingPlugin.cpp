#include "ImagingPlugin.h"
#include "Panel3DSurface.h"
#include "Panel3DVolume.h"
#include "PanelCScan.h"
#include "PanelImageControl.h"
#include "PanelImport.h"
#include "UTDocument.h"
#include <Core/ICore.h>
#include <Core/LicenseManager.h>

using namespace Core::Internal;

bool ImagingPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    UTDocument *doc = new UTDocument(this);
    doc->InitializeLayers();

    MainWindow *mw = ICore::mainWindow();

//    PanelImageControl *control = new PanelImageControl(mw);
//    control->setObjectName("PanelImageControl");
//    mw->addDockWidget(Qt::LeftDockWidgetArea, mw->addDockForWidget(control));

    auto *cscan1 = new PanelCScan(mw);
    cscan1->SetLayerID(1);
    mw->AddPanelToDockWidget(cscan1, Qt::LeftDockWidgetArea, tr("C扫描 1"), "CScan1");

    auto *cscan2 = new PanelCScan(mw);
    cscan2->SetLayerID(2);
    mw->AddPanelToDockWidget(cscan2, Qt::LeftDockWidgetArea, tr("C扫描 2"), "CScan2", false);

    auto *cscan3 = new PanelCScan(mw);
    cscan3->SetLayerID(3);
    mw->AddPanelToDockWidget(cscan3, Qt::LeftDockWidgetArea, tr("C扫描 3"), "CScan3", false);

#ifdef QT_DATAVISUALIZATION_LIB
    if (LicenseManager::instance()->hasProductLicense()) {
        mw->AddPanelToDockWidget(new Panel3DSurface(mw), Qt::RightDockWidgetArea, tr("3D 曲面"), "3DSurface", false);
        mw->AddPanelToDockWidget(new Panel3DVolume(mw), Qt::RightDockWidgetArea, tr("3D 体渲染"), "3DVolume", false, true);
    }
#endif

    return true;
}

void ImagingPlugin::extensionsInitialized()
{

}
