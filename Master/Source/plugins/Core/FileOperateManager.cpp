#include "FileOperateManager.h"
#include "CoreConstants.h"
#include "ICore.h"
#include "Platform.h"
#include "actionmanager/actioncontainer.h"
#include "actionmanager/actionmanager.h"
#include <Acquisition/AcquisitionConstants.h>
#include <Acquisition/DialogScanTaskInfo.h>
#include <Imaging/UTDocument.h>
#include <QAction>
#include <QFileDialog>
#include <QLoggingCategory>
#include <QMenu>
#include <QMessageBox>
#include <QProgressDialog>

static Q_LOGGING_CATEGORY(debug, "core.file");

using namespace Core::Constants;

FileOperateManager::FileOperateManager(QObject *parent):
    QObject(parent)
{
    createActions();
}

FileOperateManager::~FileOperateManager() = default;

int FileOperateManager::closeFile()
{
   if (document()->Close()) {
       return 0;
   }
   if (document()->GetCurrentFilePath().isEmpty()) {        // 无路径，是新建文件
       return document()->Close();
   } else {                                                                             // 已经是已打开的数据文件
       return document()->Save();
   }
}

QString FileOperateManager::newFile()
{
    return QFileDialog::getSaveFileName(Core::ICore::mainWindow(), tr("新建文件"), QString(), tr("Data File(*%1)").arg(ARS_FILE_EXT_DATA));
}

QString FileOperateManager::openFile()
{
    return QFileDialog::getOpenFileName(Core::ICore::mainWindow(), tr("打开文件..."), QString(), tr("Data File(*%1)").arg(ARS_FILE_EXT_DATA));
}

QString FileOperateManager::saveFile()
{
    return QFileDialog::getSaveFileName(Core::ICore::mainWindow(), tr("另存为"), QString(), tr("Data File(*%1)").arg(ARS_FILE_EXT_DATA));
}

void FileOperateManager::createActions()
{
    QAction *open = new QAction(QIcon(":/fatcow/16x16/folder_go.png"), tr("打开文件"), this);
    QAction *taskInfo = new QAction(QIcon(":/fatcow/16x16/align_compact.png"), tr("任务信息"), this);
    QAction *reload = new QAction(QIcon(":/fatcow/16x16/arrow_refresh.png"), tr("重新加载"), this);
    QAction *cutFile = new QAction(QIcon(":/fatcow/16x16/cut.png"), tr("精简数据文件"), this);

    connect(open, &QAction::triggered, this, &FileOperateManager::on_actionOpen_triggered);
    connect(taskInfo, &QAction::triggered, this, &FileOperateManager::on_actionTaskInfo_triggered);
    connect(reload, &QAction::triggered, this, &FileOperateManager::on_actionReloadFile_triggered);
    connect(cutFile, &QAction::triggered, this, &FileOperateManager::on_actionCutFile_triggered);

    using namespace Core;
    using namespace Constants;

    ActionContainer *menuContainer = ActionManager::actionContainer(Constants::MENU_BAR);
    ActionContainer *fileMenu = ActionManager::createMenu(M_FILE);
    fileMenu->menu()->setTitle(tr("文件"));

    menuContainer->appendGroup(G_FILE);
    menuContainer->addMenu(fileMenu);

    fileMenu->appendGroup(G_DOCUMENT);

    Command *cmd = nullptr;
    cmd = ActionManager::registerAction(open, OPEN);
    fileMenu->addAction(cmd, G_DOCUMENT);

    cmd = ActionManager::registerAction(cutFile, CUT);
    fileMenu->addAction(cmd, G_DOCUMENT);

    cmd = ActionManager::registerAction(taskInfo, TASK_INFO);
    fileMenu->addAction(cmd, G_DOCUMENT);

    cmd = ActionManager::registerAction(reload, RELOAD_FILE);
    fileMenu->addAction(cmd, G_DOCUMENT);

    fileMenu->addSeparator(G_DOCUMENT);
}

void FileOperateManager::on_actionTaskInfo_triggered()
{
    if (document()->GetCurrentFilePath().isEmpty()) {
        return;
    }
    DialogScanTaskInfo info;
    info.setWindowTitle(tr("任务详细"));
    info.setTask(document()->scanTask());
    info.setEditable();
    info.exec();
}

void FileOperateManager::on_actionOpen_triggered()
{
    // 打开文件
    QString fileName = FileOperateManager::openFile();
    if (fileName.isEmpty()) {
        return;
    }

    int ret = closeFile();
    if (ret != 0) {
        return;
    }
    // 清空缓冲区
    document()->clear();

    ret = document()->Open(fileName);

    if (ret != 0) {
        QMessageBox::warning(Core::ICore::mainWindow() ,QString(), tr("文件打开失败: %1").arg(ret));
        return;
    }
    document()->RequestDataReadAll();
}

void FileOperateManager::on_actionReloadFile_triggered()
{
    document()->Reload();
}

void FileOperateManager::on_actionCutFile_triggered()
{
    QStringList openFilesList = QFileDialog::getOpenFileNames(Core::ICore::mainWindow(), tr("Cut Files"), QString(), tr("Data File(*%1)").arg(ARS_FILE_EXT_DATA));
    if (openFilesList.isEmpty()) {
        return;
    }
    if (QMessageBox::warning(Core::ICore::mainWindow() ,QString(), tr("精简文件将导致A扫数据丢失，是否继续？"), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel) {
        return;
    }
    if (document()->cutFiles(openFilesList) != 0) {
        QMessageBox::warning(Core::ICore::mainWindow() ,QString(), tr("精简文件失败"));
        return;
    }
}
