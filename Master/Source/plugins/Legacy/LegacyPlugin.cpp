#include "LegacyPlugin.h"
#include "CameraView.h"
#include "WizardArdFileMaintain.h"
#include <Acquisition/ScanPlanModel.h>
#include <Core/CoreConstants.h>
#include <Core/ICore.h>
#include <Core/actionmanager/actioncontainer.h>
#include <Core/actionmanager/actionmanager.h>
#include <Motion/MotionManager.h>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <Ultrasonic/UltrasonicManager.h>

using namespace Core;
using namespace Core::Internal;
using namespace Core::Constants;

bool LegacyPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    auto *mw = ICore::mainWindow();
    mw->AddPanelToDockWidget(new CameraView(mw), Qt::RightDockWidgetArea, tr("摄像机"), "Camera", false);

    ActionContainer *viewMenu = ActionManager::actionContainer(M_VIEW);

    viewMenu->appendGroup(G_VIEW_WORKSPACE);
    viewMenu->addSeparator(G_VIEW_WORKSPACE);

    Command *cmd = nullptr;
    QAction *action = nullptr;

    action = new QAction(tr("导入工作区..."), this);
    connect(action, &QAction::triggered, this, &LegacyPlugin::onImportWorkspace);
    cmd = ActionManager::registerAction(action, IMPORT_WORKSPACE);
    viewMenu->addAction(cmd, G_VIEW_WORKSPACE);

    action = new QAction(tr("导出工作区..."), this);
    connect(action, &QAction::triggered, this, &LegacyPlugin::onExportWorkspace);
    cmd = ActionManager::registerAction(action, EXPORT_WORKSPACE);
    viewMenu->addAction(cmd, G_VIEW_WORKSPACE);

//    action = new QAction(tr("文件格式转换"));
//    connect(action, &QAction::triggered, this, [] () {
//        WizardArdFileMaintain wizard;
//        wizard.exec();
//    });
//    const char M_CONVERT[]             = "WaferScan.Menu.File.Convert";
//    cmd = ActionManager::registerAction(action, M_CONVERT);
//    ActionContainer *fileMenu = ActionManager::actionContainer(M_FILE);
//    fileMenu->addAction(cmd, G_DOCUMENT);

//    mw->AddPanelToDockWidget(new FileTransform(mw), Qt::LeftDockWidgetArea, tr("旧文件转换"), "FileTransform", false);

    return true;
}

void LegacyPlugin::extensionsInitialized()
{

}

void LegacyPlugin::onImportWorkspace()
{
    QString fileName = QFileDialog::getOpenFileName(ICore::dialogParent(), tr("导入工作区"), QString(), tr("工作区文件 (*.workspace)"));
    if (fileName.isEmpty()) {
        return;
    }
    QJsonObject info = ReadFileAsJson(fileName).toObject();
    QList<UTSetup> setups = convertFromJson<QList<UTSetup>>(info["UTSetups"]);
    for (int index = 0; index < setups.count(); ++index) {
        UTChannel *channel = UltrasonicManager::channel(index);
        if (channel != nullptr) {
            channel->setSetup(setups[index]);
        }
    }

    ScanPlanModel::instance()->fromJson(info["ScanPlans"].toArray());
    MotionManager::safetyBarrier()->setOptions(convertFromJson<SafetyBarrierOptions>(info["SafeRect"].toObject()));
}

void LegacyPlugin::onExportWorkspace()
{
    QString fileName = QFileDialog::getSaveFileName(ICore::dialogParent(), tr("导出工作区"), QString(), tr("工作区文件 (*.workspace)"));
    if (fileName.isEmpty()) {
        return;
    }
    QJsonObject info;
    info["UTSetups"] = convertToJson(UltrasonicManager::setups());
    info["ScanPlans"] = ScanPlanModel::instance()->toJson();
    info["SafeRect"] = convertToJson(MotionManager::safetyBarrier()->options());
    if (!WriteFileAsJson(fileName, info)) {
        QMessageBox::warning(ICore::dialogParent(), QString(), tr("导出工作区失败：无法写入文件"));
    }
}
