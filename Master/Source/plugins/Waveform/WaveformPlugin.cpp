#include "PanelAScan.h"
#include "PanelUTSetup.h"
#include "WaveformPlugin.h"
#include <Core/actionmanager/actioncontainer.h>
#include <Core/actionmanager/actionmanager.h>
#include "DialogMaterial.h"
#include "DialogProbeList.h"
#include <QAction>
#include <QMessageBox>
#include <Core/LicenseManager.h>
#include <Waveform/WaveformManager.h>

using namespace Core::Internal;

bool WaveformPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);
    manager = new WaveformManager(this);

    if (LicenseManager::instance()->hasProductLicense()) {
        ActionContainer *viewMenu = ActionManager::actionContainer(Core::Constants::M_VIEW);
        viewMenu->addSeparator(Constants::G_VIEW_PANEL);

        QAction *materialAction = new QAction(QIcon(tr(":/fatcow/16x16/chart_column_2.png")), tr("编辑材料"), this);
        materialAction->setEnabled(LicenseManager::instance()->hasProductLicense());
        connect(materialAction, &QAction::triggered, this, [] () {
            DialogMaterial material;
            material.setEditable(true);
            material.exec();
        });

        ActionContainer *fileMenu = ActionManager::actionContainer(Core::Constants::M_FILE);
        Command *cmd = nullptr;
        cmd = ActionManager::registerAction(materialAction, Core::Constants::MATERIAL);
        fileMenu->addAction(cmd, Core::Constants::G_FILE_OPTION);

        QAction *probeAction = new QAction(QIcon(tr(":/fatcow/16x16/chart_column_2.png")), tr("编辑探头"), this);
        connect(probeAction, &QAction::triggered, this, [] () {
            DialogProbeList probe;
            probe.setEditable(true);
            probe.exec();
        });
        cmd = ActionManager::registerAction(probeAction, Core::Constants::PROBE);
        fileMenu->addAction(cmd, Core::Constants::G_FILE_OPTION);
        fileMenu->addSeparator(Core::Constants::G_FILE_OPTION);
    }

    return true;
}

void WaveformPlugin::extensionsInitialized()
{
    manager->loadSettings();
}

ExtensionSystem::IPlugin::ShutdownFlag WaveformPlugin::aboutToShutdown()
{
    manager->saveSettings();
    return SynchronousShutdown;
}
