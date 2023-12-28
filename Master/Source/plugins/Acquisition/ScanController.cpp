#include "ScanController.h"
#include "ScanPlan.h"
#include "ScanTask.h"
#include <Core/Platform.h>
#include <QLoggingCategory>
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <Waveform/WaveformManager.h>

static Q_LOGGING_CATEGORY(debug, "acquire.scan");

static ScanController *m_instance = nullptr;

ScanController::ScanController(QObject *parent) :
    QObject(parent)
{
    Q_ASSERT(m_instance == nullptr);
    m_instance = this;
    connect(&m_taskModel, &ScanTaskModel::currentTaskChanged, this, &ScanController::currentTaskUpdated);
}

ScanController::~ScanController()
{
    m_instance = nullptr;
}

ScanController *ScanController::instance()
{
    return m_instance;
}

ScanTaskModel *ScanController::taskModel()
{
    return &m_taskModel;
}

/*! 加载扫描根节点，更新scantask列表 */
void ScanController::LoadScanPlan(ScanPlan *root)
{
    m_taskModel.clear();
    m_rootScanPlan = root;
    ScanPlan *plan = root->nextValid();
    while (plan) {
        auto *task = new ScanTask(plan, this);
        m_taskModel.addTask(task);
        setCurrentTask(task);
        plan = plan->nextValid(root, false);
    }
}

/*! 获取当前状态 */
ScanController::State ScanController::currentState()
{
    return m_currentState;
}

/*! 更新当前状态 */
void ScanController::UpdateState(State state)
{
    if (state != m_currentState) {
        qCDebug(debug) << "State" << m_currentState << "->" << State(state);
        m_currentState = state;
        if (m_currentState == IDLE) {
            setCurrentTask(nullptr);
        }
        WaveformManager::setSetupLock(state != ScanController::IDLE);
        emit StateChanged(m_currentState);
    }
}

ScanTask *ScanController::currentTask() const
{
    return m_taskModel.currentTask();
}

void ScanController::setCurrentTask(ScanTask *task)
{
    if (currentTask() == task) {
        return;
    }
    if (m_currentState != IDLE) {
        qCWarning(debug) << "Can't set current Task: ScanController is not available";
        return;
    }
    ScanTask *oldTask = m_taskModel.currentTask();
    if (oldTask) {
        disconnect(oldTask, nullptr, this, nullptr);
    }
    m_taskModel.setCurrentTask(task);
    if (task) {
        connect(task, &ScanTask::stateChanged, this, &ScanController::OnScanTaskStateChanged);
        connect(task, &ScanTask::progressUpdated, this, &ScanController::currentTaskUpdated);
    }
}

//! 添加任务到计划
ScanTask *ScanController::createTask(ScanPlan *plan)
{
    if (m_currentState != IDLE) {
        qCWarning(debug) << "Can't create task: controller is not available";
        return nullptr;
    }
    if (!plan) {
        qCWarning(debug) << "Can't create task: invalid plan";
        return nullptr;
    }
    auto *task = new ScanTask(plan, this);
    m_taskModel.addTask(task);
    return task;
}

//! 删除所选任务
void ScanController::removeTask(ScanTask *task)
{
    if (!task || m_currentState != IDLE) {
        qCCritical(debug) << "Can't remove task: Invalid task or current state is not IDLE";
        return;
    }
    m_taskModel.removeTask(task);
}

void ScanController::removeTasks()
{
    if (m_currentState != IDLE) {
        qCCritical(debug) << "Can't remove task: Invalid task or current state is not IDLE";
        return;
    }
    m_taskModel.clear();
}

bool ScanController::canStart(ScanTask *task) const
{
    if (m_currentState != IDLE && task != currentTask()) {
        return false;
    }
    return task ? task->canStart() : false;
}

bool ScanController::canPause() const
{
    ScanTask *current = currentTask();
    return current ? current->canPause() : false;
}

bool ScanController::canStop() const
{
    ScanTask *current = currentTask();
    return current ? current->canStop() : false;
}

bool ScanController::canRemove(ScanTask *task) const
{
    if (m_currentState != IDLE) {
        return false;
    }
    return task && task->canRemove();
}

bool ScanController::canEdit(ScanTask *task) const
{
    if (m_currentState != IDLE) {
        return false;
    }
    return task && task->canEdit();
}

/*! 开始扫描 */
void ScanController::Start()
{
    ScanTask *task= currentTask();
    if (!task) {
        qCWarning(debug) << "Start failed: no current task";
        return;
    }
    if (m_currentState == IDLE) {             // 是开始
        StartTask(task);
    } else if (m_currentState == PAUSED) {   // 是恢复扫描
        if (currentTask()) {
            StartTask(task);
        } else {
            qCWarning(debug) << "Restore failed, currentTask is invalid";
            return;
        }
    } else {
        qCWarning(debug) << "Start failed, currentState is" << m_currentState;
    }
}

/*! 停止扫描 */
void ScanController::Stop()
{
    if (!currentTask()) {
        qCWarning(debug) << "Stop failed, current task is invalid";
        return;
    }
    UpdateState(FINISHING);
    currentTask()->stopTask();
}

/*! 暂停扫描 */
void ScanController::Pause()
{
    if (!currentTask()) {
        qCWarning(debug) << "Pause failed, current task is invalid";
        return;
    }
    UpdateState(PAUSING);
    currentTask()->pauseTask();
}

/*! 开始一个扫描任务 */
void ScanController::StartTask(ScanTask *task)
{
    if (!task) {
        qCCritical(debug) << "Can't start an invalid task";
        return;
    }
    if (task->state() != ScanTask::Idle && task->state() != ScanTask::Paused) {
        qCCritical(debug) << "Can't start task with invalid state" << task->state();
        return;
    }
    UpdateState(RUNNING);
    setCurrentTask(task);
    task->startTask();
}

/*! 当前scantask状态改变的曹函数 */
void ScanController::OnScanTaskStateChanged(int taskState)
{
    const auto state = ScanTask::State(taskState);
    qCDebug(debug) << "Task state" << state;
    switch (state) {
    case ScanTask::Idle: {
        UpdateState(IDLE);
        break;
    }
    case ScanTask::Pending:
    case ScanTask::Locating:
    case ScanTask::Running: {
        UpdateState(RUNNING);
        break;
    }
    case ScanTask::Pausing:
        UpdateState(PAUSING);
        break;
    case ScanTask::Paused: {
        UpdateState(PAUSED);
        break;
    }
    case ScanTask::Restoring:
        break;
    case ScanTask::Cancelling:
        break;
    case ScanTask::Finishing:
        break;
    case ScanTask::Completed: {
        UpdateState(IDLE);
        break;
    }
    case ScanTask::Error: {
        UpdateState(IDLE);
        break;
    }
    default :
        break;
    }
}
