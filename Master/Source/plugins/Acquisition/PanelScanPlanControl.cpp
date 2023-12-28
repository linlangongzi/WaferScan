#include "PanelScanPlanControl.h"
#include "ui_PanelScanPlanControl.h"
#include "AcquisitionConstants.h"
#include "DialogScanTaskInfo.h"
#include "ScanController.h"
#include <Core/FileOperateManager.h>
#include <Core/ICore.h>
#include <Core/LicenseManager.h>
#include <Core/Platform.h>
#include <Core/actionmanager/actioncontainer.h>
#include <Core/actionmanager/actionmanager.h>
#include <Imaging/ReportDlg.h>
#include <Imaging/UTDocument.h>
#include <Imaging/UTLayer.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <Motion/TrioDriver.h>
#include <QFileDialog>
#include <QLoggingCategory>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <utils/algorithm.h>

#ifdef QT_WINEXTRAS_LIB
#   include <QWinTaskbarButton>
#   include <QWinTaskbarProgress>
#endif

static Q_LOGGING_CATEGORY(debug, "acquire.task.model");

using namespace Core;

PanelScanPlanControl::PanelScanPlanControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelScanPlanControl)
{
    ui->setupUi(this);
    ui->actionQuickStart->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->actionStart->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->actionPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->actionStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->scanPlan->initActions(this);
#ifdef QT_WINEXTRAS_LIB
    taskbar = new QWinTaskbarButton(this);
#endif

    createActions();
    SetScanControllerEnable(true);
    OnControllerStatusChanged(ScanController::IDLE);

    ui->tasks->setModel(ScanController::instance()->taskModel());
    connect(ui->tasks, &QWidget::customContextMenuRequested, this, &PanelScanPlanControl::onTaskContextMenu);
    connect(ui->tasks->selectionModel(), &QItemSelectionModel::currentChanged, this, &PanelScanPlanControl::refreshTaskButtons);
    connect(ui->tasks, &QTreeView::doubleClicked, this, &PanelScanPlanControl::on_actionEditTask_triggered);
    ui->actionAddLegacyCylinder->setVisible(platform()->Get("ShowLegacyScanPlan").toBool());
    ui->actionAddLegacyRectangle->setVisible(platform()->Get("ShowLegacyScanPlan").toBool());
    ui->actionAddLegacyDisk->setVisible(platform()->Get("ShowLegacyScanPlan").toBool());
}

PanelScanPlanControl::~PanelScanPlanControl()
{
    delete ui;
}

void PanelScanPlanControl::SetScanControllerEnable(bool enable)
{
    for (UTLayer *layer: document()->GetLayerMap()) {
        layer->SetDataInputEnabled(enable);
    }
    setEnabled(enable);
    ScanController *controller = ScanController::instance();
    if (enable) {
        connect(controller, &ScanController::StateChanged, this, &PanelScanPlanControl::OnControllerStatusChanged);
        connect(controller, &ScanController::currentTaskUpdated, this, &PanelScanPlanControl::refreshScanTask);
    } else {
        disconnect(controller, nullptr, this, nullptr);
    }
    refreshScanTask();
}

bool PanelScanPlanControl::checkController(ScanPlan *plan)
{
    if (!checkPlan(plan)) {
        return false;
    }
    // 判断运动控制器是否连接
    if (MotionManager::driver()->status().state != Motion::Normal) {
        if (ICore::isInternalVersion()) {
            if (QMessageBox::warning(this, QString(), tr("运动控制器未连接, 强制开始扫描吗?"), QMessageBox::Yes | QMessageBox::Cancel) != QMessageBox::Yes) {
                return false;
            }
        } else {
            QMessageBox::warning(this, QString(), tr("运动控制器未连接"));
            return false;
        }
    }
    // 判断ScanController是否处于IDLE或者PAUSED状态
    if ((currentStatus != ScanController::IDLE) && (currentStatus != ScanController::PAUSED)) {
        qCCritical(debug) << "Can't start task: Controller is not available";
        return false;
    }

    QStringList lockedAxis;
    QStringList needDatumAxis;
    // 判断相关轴的锁定状态
    foreach (int index, plan->associatedAxes().toList()) {
        Axis *axis = MotionManager::axis(index);
        if (!axis) {
            QMessageBox::warning(this, QString(), tr("扫描计划中指定的运动轴 %1 不存在").arg(index));
            return false;
        }
        if (axis->setup().disabled) {
            lockedAxis << axis->name();
        }
        if (!axis->datumDone() && axis->axisType() != AxisConfig::Rotation && !axis->config().hidden) {
            needDatumAxis << axis->name();
        }
    }
    if (!lockedAxis.isEmpty()) {
        QMessageBox::warning( this, QString(), tr("运动轴 %1 被锁定").arg(lockedAxis.join(", ")));
        return false;
    }
    // 判断扫描区域是否在安全区，不在的话，不允许扫描
    QRectF safeRect = MotionManager::safetyBarrier()->options().rect;
    QRectF scanRect = plan->boundingRect(0, 1);
    /* 由于QRectF::contains(QRectF)对于边缘相同时判为不包含，此处使用QRect::contains(QPointF) */
    if ( !safeRect.contains( scanRect.topLeft() ) || !safeRect.contains( scanRect.bottomRight() ) ) {
        qCDebug(debug) << "Check plan: safeRect" << safeRect << "scanRect" << scanRect;
        QMessageBox::warning( this, tr("警告"), tr("扫描计划超出安全区") );
        return false;
    }
    if (!needDatumAxis.isEmpty()) {
        if (platform()->Get("MotionScanWithoutDatum").toBool()) {
            int ret = QMessageBox::critical(this, QString(), tr("运动轴 %1 未经复位, 扫描可能导致机械故障, 确定继续吗?").arg(needDatumAxis.join(", ")), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (ret != QMessageBox::Yes) {
                return false;
            }
        } else {
            QMessageBox::critical(this, QString(), tr("运动轴 %1 未经复位, 无法进行扫描").arg(needDatumAxis.join(", ")));
            return false;
        }
    }
    return true;
}

bool PanelScanPlanControl::checkPlan(ScanPlan *plan)
{
    if (plan == nullptr) {
        QMessageBox::warning( this, QString(), tr("未选择有效的扫描计划") );
        return false;
    }

    if (!plan->hasValidPath()) {
        const QStringList errors = Utils::filteredUnique(plan->pathError());
        if (errors.isEmpty()) {
            QMessageBox::warning(this, QString(), tr("扫描计划包含错误"));
        } else {
            QMessageBox errorBox(QMessageBox::Warning, QString(), tr("扫描计划包含以下错误:"), QMessageBox::Ok, this);
            errorBox.setInformativeText(errors.join("\n"));
            errorBox.exec();
        }
        return false;
    }
    const QStringList warnings = Utils::filteredUnique(plan->pathWarning());
    if (!warnings.isEmpty()) {
        QMessageBox warningBox(QMessageBox::Warning, QString(), tr("警告: 创建的扫描计划存在以下问题:"), QMessageBox::Ok | QMessageBox::No, this);
        warningBox.setInformativeText(warnings.join("\n"));
        if (warningBox.exec() == QMessageBox::No) {
            return false;
        }
    }
    return true;
}

ScanTask *PanelScanPlanControl::createTask(ScanPlan *plan, ReportDlg *report)
{
    if (!checkPlan(plan) || !report) {
        return nullptr;
    }

    using namespace Core::Constants;
    const auto customData = report->customData(plan);
    QString fileName = report->askFileName(customData, tr("扫描数据文件 (*%1)").arg(ARS_FILE_EXT_DATA));
    if (fileName.isEmpty()) {
        return nullptr;
    }
    QFileInfo fileInfo(fileName);
    if (QString(".") + fileInfo.suffix() != ARS_FILE_EXT_DATA) {
        fileName += ARS_FILE_EXT_DATA;
    }
    // 创建扫描任务
    ScanTask *task = ScanController::instance()->createTask(plan);
    if (task) {
        task->setPath(plan->planPath());
        ScanTaskData data = task->data();
        data.filePath = fileName;
        data.customData = customData;
        task->setData(data);
    }
    return task;
}

QList<ScanTask *> PanelScanPlanControl::createTasks(ScanPlan *plan, ReportDlg *report)
{
    QList<ScanTask *> tasks;
    if (!report) {
        return tasks;
    }

    if (plan->hasValidPath()) {
        tasks << createTask(plan, report);
        return tasks;
    }

    for (ScanPlan *child: plan->children()) {
        for (ScanTask *task: createTasks(child, report)) {
            tasks << task;
        }
    }

    return tasks;
}

bool PanelScanPlanControl::createDocument(ScanTask *task)
{
    if (!task) {
        return false;
    }
    const QString fileName = task->data().filePath;
    if (task->state() == ScanTask::Paused && QFileInfo::exists(fileName)) {
        return true;
    }
    QFileInfo fileInfo(fileName);
    qCInfo(debug) << "Start scanplan: using file name" << fileName;
    QDir dir;
    if (!dir.mkpath(fileInfo.absolutePath())) {
        QMessageBox::warning(this, QString(), tr("新建文件失败 - 无法为下列文件创建文件夹：\n%1").arg(fileName));
        return false;
    }

    if (FileOperateManager::closeFile()) {
        return false;
    }

    int ret = document()->NewFile(fileName);
    if (ret != 0) {
        if (ret == 1) {
            QMessageBox::warning(this, QString(), tr("当前文件已经存在"));
        } else {
            QMessageBox::warning(this, QString(), tr("文件新建失败 %1").arg(ret));
        }
        return false;
    }
    ScanPlan *scan = task->scanPlan();
    document()->setScanTask(task);
    document()->LoadSetups();
    for (UTLayer *layer: document()->GetLayerMap()) {
        int id = layer->GetID() - 1;
        ImageOptions options = scan->imageOptionsByIndex(id);
        layer->Reset();
        layer->SetImageOptions(options);
        qCDebug(debug) << "Start Scan" << scan << scan->name() << "ImageAxis" << options.axisX() << options.axisY();
    }
    return true;
}

void PanelScanPlanControl::refreshScanTask()
{
    ScanTask *task = ScanController::instance()->currentTask();
    ui->progressContainer->setVisible(task);
    int max = 100;
    int percent = 0;
    if (task) {
        percent = task->percent();
        QString stateText = task->stateText();
        const auto status = task->status();
        if (task->state() == ScanTask::Error) {
            stateText += tr(" <font color=red>%1</font>").arg(task->errorString());
        }
        ui->status->setText(stateText);
        if (!task->hasProgress()) {
            max = 0;
            percent = 0;
        }
        ui->scanLine->setText(tr("%1 / %2").arg(status.linesDone).arg(status.linesTotal));
        ui->progress->setRange(0, max);
        ui->progress->setValue(percent);
        ui->progress->setVisible(task->hasProgressBar());
        QString expected = QTime::fromMSecsSinceStartOfDay(task->timeExpected() * 1000).toString();
        QString remain = QTime::fromMSecsSinceStartOfDay(task->timeRemaining() * 1000).toString();

        ui->time->setText(tr("%1 / %2").arg(remain, expected));
    }
#ifdef QT_WINEXTRAS_LIB
    taskbar->setWindow(Core::ICore::mainWindow()->windowHandle());
    auto state = task ? task->state() : ScanTask::Idle;
    QWinTaskbarProgress *progress = taskbar->progress();
    switch (state) {
    case ScanTask::Paused:      progress->pause(); break;
    case ScanTask::Error:       progress->stop(); break;
    case ScanTask::Idle:        // Fall through for all other states.
    case ScanTask::Pending:
    case ScanTask::Sending:
    case ScanTask::Starting:
    case ScanTask::Locating:
    case ScanTask::Running:
    case ScanTask::Pausing:
    case ScanTask::Restoring:
    case ScanTask::Finishing:
    case ScanTask::Completed:
    default:                    progress->resume(); break;
    }
    if (state == ScanTask::Idle || state == ScanTask::Completed) {
        progress->hide();
    } else {
        progress->show();
    }
    progress->setRange(0, max);
    progress->setValue(percent);
#endif
    refreshTaskButtons();
}

void PanelScanPlanControl::createActions()
{
    using namespace Constants;
    using namespace Acquisition;

    ActionContainer *menubar = ActionManager::actionContainer(MENU_BAR);
    ActionContainer *menu = ActionManager::createMenu(M_SCAN);
    menu->setOnAllDisabledBehavior(ActionContainer::Hide);
    menu->menu()->setTitle(tr("扫描"));
    menu->menu()->setEnabled(LicenseManager::instance()->hasProductLicense());

    menubar->insertGroup(G_HELP, G_SCAN);
    menubar->addMenu(ActionManager::actionContainer(M_HELP), menu, G_SCAN);

    Command *cmd = nullptr;

    // Group: Scan Plan
    menu->appendGroup(G_SCAN_PLAN);
    cmd = ActionManager::registerAction(ui->actionQuickStart, QUICK_START_PLAN);
    menu->addAction(cmd, G_SCAN_PLAN);

    menu->addSeparator(G_SCAN_PLAN);

    cmd = ActionManager::registerAction(ui->actionAddFolder, ADD_FOLDER);
    menu->addAction(cmd, G_SCAN_PLAN);

    cmd = ActionManager::registerAction(ui->actionAddRectangle, ADD_RECTANGLE);
    menu->addAction(cmd, G_SCAN_PLAN);

    cmd = ActionManager::registerAction(ui->actionAddLegacyRectangle, ADD_LEGACY_RECTANGLE);
    menu->addAction(cmd, G_SCAN_PLAN);

    cmd = ActionManager::registerAction(ui->actionAddCylinder, ADD_CYLINDER);
    menu->addAction(cmd, G_SCAN_PLAN);

    cmd = ActionManager::registerAction(ui->actionAddLegacyCylinder, ADD_LEGACY_CYLINDER);
    menu->addAction(cmd, G_SCAN_PLAN);

    cmd = ActionManager::registerAction(ui->actionAddDisk, ADD_DISK);
    menu->addAction(cmd, G_SCAN_PLAN);

    cmd = ActionManager::registerAction(ui->actionAddLegacyDisk, ADD_LEGACY_DISK);
    menu->addAction(cmd, G_SCAN_PLAN);

    cmd = ActionManager::registerAction(ui->actionAddAnnulus, ADD_RING);
    menu->addAction(cmd, G_SCAN_PLAN);

    menu->addSeparator(G_SCAN_PLAN);

    cmd = ActionManager::registerAction(ui->actionRename, RENAME_PLAN);
    menu->addAction(cmd, G_SCAN_PLAN);

    cmd = ActionManager::registerAction(ui->actionDelete, REMOVE_PLAN);
    menu->addAction(cmd, G_SCAN_PLAN);

    menu->addSeparator(G_SCAN_PLAN);

    // Group: Scan Task
    menu->appendGroup(G_SCAN_TASK);

    cmd = ActionManager::registerAction(ui->actionStart, START_TASK);
    menu->addAction(cmd, G_SCAN_TASK);

    cmd = ActionManager::registerAction(ui->actionStop, STOP_TASK);
    cmd->setDefaultKeySequence(QKeySequence("Esc"));
    menu->addAction(cmd, G_SCAN_TASK);

    cmd = ActionManager::registerAction(ui->actionCreateTask, CREATE_TASK);
    menu->addAction(cmd, G_SCAN_TASK);

    cmd = ActionManager::registerAction(ui->actionEditTask, EDIT_TASK);
    menu->addAction(cmd, G_SCAN_TASK);

    cmd = ActionManager::registerAction(ui->actionRemoveTask, REMOVE_TASK);
    menu->addAction(cmd, G_SCAN_TASK);

    cmd = ActionManager::registerAction(ui->actionOpenSelectedFile, OPEN_SELECTED_FILE);
    menu->addAction(cmd, G_SCAN_TASK);
}

void PanelScanPlanControl::onTaskContextMenu(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu;
    menu.addActions(ui->toolbar_2->actions());
    menu.exec(QCursor::pos());
    for (QAction *action: ui->toolbar_2->actions()) {
        action->setParent(ui->tasks);
    }
}

void PanelScanPlanControl::OnControllerStatusChanged(int status)
{
    currentStatus = status;
    ui->scanPlan->refreshActionsEnable(ui->scanPlan->currentIndex(), QModelIndex());
    refreshTaskButtons();
}

/*!
 * \brief PanelScanPlanControl::on_actionCreateTask_triggered
 * 支持单一创建任务、批量创建任务
 */
void PanelScanPlanControl::on_actionCreateTask_triggered()
{
    QModelIndex index = ui->scanPlan->currentIndex();
    if (!index.isValid()) {
        return;
    }

    ScanPlan *plan = ScanPlanModel::instance()->scanPlanFromIndex(index);
    if (!plan->hasValidPath() && QMessageBox::question(this, QString(), tr("是否创建计划组中所有任务"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }
    ReportDlg dlg(this);
    if (dlg.exec() == QDialog::Rejected) {
        return;
    }
    createTasks(plan, &dlg);
}

//! 编辑选中的Task
void PanelScanPlanControl::on_actionEditTask_triggered()
{
    DialogScanTaskInfo info;
    QModelIndex index = ui->tasks->currentIndex();
    ScanTask *task = ScanController::instance()->taskModel()->taskFromIndex(index);
    if (!task) {
        qCDebug(debug) << "Can't edit task: Invalid task";
        return;
    }
    info.setTask(task);
    info.exec();
}

//! 删除选中的Task
void PanelScanPlanControl::on_actionRemoveTask_triggered()
{
    QModelIndex index = ui->tasks->currentIndex();
    ScanTask *task = ScanController::instance()->taskModel()->taskFromIndex(index);
    if (!task) {
        qCDebug(debug) << "Can't remove task: Invalid task";
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定删除所选任务吗?"), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
        ScanController::instance()->removeTask(task);
    }
}

void PanelScanPlanControl::refreshTaskButtons()
{
    auto *controller = ScanController::instance();
    ScanTask *current = controller->taskModel()->taskFromIndex(ui->tasks->currentIndex());
    ui->actionStart->setEnabled(controller->canStart(current));
    ui->actionPause->setEnabled(controller->canPause());
    ui->actionStop->setEnabled(controller->canStop());
    ui->actionRemoveTask->setEnabled(controller->canRemove(current));
    ui->actionClearTasks->setEnabled(controller->canRemove(current));
    ui->actionEditTask->setEnabled(controller->canEdit(current));
    auto status = currentStatus;
    if (status == ScanController::IDLE) {
        ui->actionCreateTask->setEnabled(true);
        ui->actionQuickStart->setEnabled(true);
    } else {
        ui->actionQuickStart->setEnabled(false);
        ui->actionCreateTask->setEnabled(false);
    }
    if (!current) {
        return;
    }
    ui->actionOpenSelectedFile->setEnabled(current->state() == ScanTask::Completed);
}

//! 将选中的Task设置当前Task
//void PanelScanPlanControl::on_actionSetCurrent_triggered()
//{
//    QModelIndex index = ui->tasks->currentIndex();
//    ScanTask *task = ScanController::instance()->taskModel()->taskFromIndex(index);
//    if (!task) {
//        qCDebug(debug) << "Can't set current task: Invalid task";
//        return;
//    }
//    ScanController::instance()->setCurrentTask(task);
//}

//! 开始扫描任务
void PanelScanPlanControl::on_actionStart_triggered()
{
    QModelIndex index = ui->tasks->currentIndex();
    ScanTask *task = ScanController::instance()->taskModel()->taskFromIndex(index);
    if (!task || !checkController(task->scanPlan()) || !createDocument(task)) {
        return;
    }
    ScanController::instance()->setCurrentTask(task);
    ScanController::instance()->Start();
    document()->setScanTask(task);
}

//! 原“开始扫描”功能变更为“快速扫描”
void PanelScanPlanControl::on_actionQuickStart_triggered()
{
    if (currentStatus == ScanController::IDLE) {
        // 检查有效扫描计划
        QModelIndex index = ui->scanPlan->currentIndex();
        ScanPlan *plan = ScanPlanModel::instance()->scanPlanFromIndex(index);
        if (!checkController(plan)) {
            return;
        }
        ReportDlg report(this);
        if (report.exec() == QDialog::Rejected) {
            return;
        }
        ScanTask *task = createTask(plan, &report);
        if (!createDocument(task)) {
            return;
        }
        document()->setScanTask(task);
        // 开始扫描
        ScanController::instance()->setCurrentTask(task);
        ScanController::instance()->Start();
    } else if (currentStatus == ScanController::PAUSED) {
        ScanController::instance()->Start();
    }
}

void PanelScanPlanControl::on_actionPause_triggered()
{
    ScanController::instance()->Pause();
}

void PanelScanPlanControl::on_actionStop_triggered()
{
    if (currentStatus == ScanController::IDLE) {
        ScanController::instance()->Stop();                                // 停止状态下，也需要下发停止指令，以防在某些特殊情况下不能停止扫描
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定要终止扫描吗?"), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
        ScanController::instance()->Stop();
    }
}

void PanelScanPlanControl::on_actionOpenSelectedFile_triggered()
{
    QModelIndex index = ui->tasks->currentIndex();
    ScanTask *task = ScanController::instance()->taskModel()->taskFromIndex(index);
    if (!task) {
        qCDebug(debug) << "Invalid task";
        return;
    }
    if (task->state() != ScanTask::Completed) {
        qCWarning(debug) << "Can't open file: Current task is not completed";
        return;
    }
    if (QFileInfo(document()->GetCurrentFilePath()) == QFileInfo(task->data().filePath)) {
        ui->actionOpenSelectedFile->setEnabled(false);
        return;
    }
    int fd = FileOperateManager::closeFile();
    if (fd != 0) {
        return;
    }
    document()->clear();
    int ret = document()->Open(task->data().filePath);
    if (ret != 0) {
        QMessageBox::warning(this, QString(), tr("文件打开失败: %1").arg(ret));
        return;
    }
    document()->RequestDataReadAll();
}

void PanelScanPlanControl::on_actionClearTasks_triggered()
{
    if (!QMessageBox::question(this, QString(), tr("删除所有任务"), QMessageBox::Yes | QMessageBox::No)) {
        return;
    }
    ScanController::instance()->removeTasks();
}
