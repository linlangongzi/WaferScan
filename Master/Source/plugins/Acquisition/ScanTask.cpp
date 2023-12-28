#include "ScanTask.h"
#include "ScanPlan.h"
#include "ScanPlanModel.h"
#include <Core/Platform.h>
#include <Imaging/UTDocument.h>
#include <Motion/Axis.h>
#include <Motion/EndPoint.h>
#include <Motion/MotionManager.h>
#include <Motion/TrioDriver.h>
#include <Motion/TrioQueue.h>
#include <QLoggingCategory>
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <Waveform/WaveformManager.h>
#include <allrising/Util.h>

Q_DECL_UNUSED static const bool registered = QMetaType::registerEqualsComparator<QMap<QString, QString>>();

using namespace Motion;

static Q_LOGGING_CATEGORY(debug, "acquire.task");

static auto *queue()
{
    return MotionManager::driver()->queue();
}

static auto *endPoint()
{
    return MotionManager::endPoint(0);
}

ScanTask::ScanTask(ScanPlan *plan, QObject *parent) :
    QObject(parent),
    prerequisityEndPoint(new EndPoint(this)),
    elementsEndPoint(new EndPoint(this))
{
    prerequisityEndPoint->setObjectName("prerequisites");
    elementsEndPoint->setObjectName("elements");

    m_scanPlan = new ScanPlan(this);
    if (plan) {
        m_scanPlan->fromJson(plan->toJson());
    }
    if (!endPoint()) {
        qCWarning(debug) << "Default end-point not exist";
    }
    connect(queue(), &TrioQueue::StateUpdated, this, &ScanTask::OnQueueStateChanged);
//    connect(DataPath::instance(), &DataPath::DisableFinished, this, &ScanTask::disableDataPath);
//    connect(endPoint, SIGNAL( EndPointIdle() ), this, SLOT( OnEndPointIdle() ));
    connect(endPoint(), &EndPoint::PositionUpdated, this, &ScanTask::OnEndPointPositionChanged);
    connect(prerequisityEndPoint, &EndPoint::EndPointIdle, this, &ScanTask::OnPrerequisityEndPointIdle);
    connect(elementsEndPoint, &EndPoint::EndPointIdle, this, &ScanTask::OnElementsEndPointIdle);

    m_data.createTime = QDateTime::currentDateTime();
}

ScanPlan *ScanTask::scanPlan() const
{
    return m_scanPlan;
}

QString ScanTask::stateText() const
{
    switch (state()) {
    case Idle:          return tr("未开始");
    case Pending:       return tr("正在等待");
    case Sending:       return tr("正在发送");
    case Starting:      return tr("正在启动");
    case Locating:      return tr("正在定位");
    case Running:       return tr("扫描");
    case Pausing:       return tr("正在暂停");
    case Paused:        return tr("暂停");
    case Restoring:     return tr("正在恢复");
    case Cancelling:    return tr("正在停止");
    case Finishing:     return tr("正在结束");
    case Completed:     return tr("完成");
    case Error:         return tr("错误");
    default:            return QString();
    }
}

QVariant ScanTask::stateIcon() const
{
    switch (state()) {
    case Idle:          return QColor(Qt::transparent);
    case Pending:       return QIcon(":/fatcow/16x16/bullet_green.png");
    case Sending:       return QIcon(":/fatcow/16x16/bullet_green.png");
    case Starting:      return QIcon(":/fatcow/16x16/bullet_green.png");
    case Locating:      return QIcon(":/fatcow/16x16/bullet_go.png");
    case Running:       return QIcon(":/fatcow/16x16/bullet_go.png");
    case Pausing:       return QIcon(":/fatcow/16x16/bullet_orange.png");
    case Paused:        return QIcon(":/fatcow/16x16/bullet_yellow.png");
    case Restoring:     return QIcon(":/fatcow/16x16/bullet_orange.png");
    case Cancelling:    return QIcon(":/fatcow/16x16/bullet_red.png");
    case Finishing:     return QIcon(":/fatcow/16x16/bullet_green.png");
    case Completed:     return QIcon(":/fatcow/16x16/bullet_valid.png");
    case Error:         return QIcon(":/fatcow/16x16/bullet_error.png");
    default:            return QIcon();
    }
}

bool ScanTask::canStart() const
{
    static const QSet<State> states = {Idle, Paused, Error};
    return states.contains(state());
}

bool ScanTask::canPause() const
{
    static const QSet<State> states = {Sending, Locating, Running};
    return states.contains(state());
}

bool ScanTask::canStop() const
{
    static const QSet<State> states = {Idle, Completed, Error};
    return !states.contains(state());
}

bool ScanTask::canRemove() const
{
    static const QSet<State> states = {Idle, Completed, Error};
    return states.contains(state());
}

bool ScanTask::canEdit() const
{
    static const QSet<State> states = {Idle, Error,Completed};
    return states.contains(state());
}

bool ScanTask::hasProgress() const
{
    static const QSet<State> states = {Sending, Pausing, Paused, Running};
    return states.contains(state());
}

bool ScanTask::hasProgressBar() const
{
    static const QSet<State> states = {Idle, Completed, Error};
    return !states.contains(state());
}

bool ScanTask::hasScanline() const
{
    static const QSet<State> states = {Locating, Running, Pausing, Paused, Restoring};
    return states.contains(state());
}

qreal ScanTask::percent() const
{
    if (state() == Sending) {
        return qreal(m_status.queueSent) / m_status.queueTotal * 100;
    }
    const qreal expected = timeExpected();
    const qreal remain = timeRemaining();
    if (qFuzzyCompare(expected, remain) || qFuzzyIsNull(expected)) {
        return 0;
    } else {
        return (expected - remain) / expected * 100;
    }
}

QPair<UTPose, MotionAxisList> ScanTask::progressPosition(bool isStart)
{
    MotionPose pos;
    MotionAxisList axes;
    MotionPath tempPath = m_status.path;
    if (m_status.linesDone < 0) {
        pos = tempPath.elements.value(0).pos.value(0);
        axes = tempPath.elements.value(0).axis;
    } else {
        bool isReverse = false;
        MotionElement element = tempPath.elementByMove(m_status.linesDone, isReverse);
        axes = element.axis;
        if (isStart) {
            pos = isReverse ? element.pos.value(element.pos.count() - 1) : element.pos.value(0);
        } else {
            pos = isReverse ? element.pos.value(0) : element.pos.value(element.pos.count() - 1);
        }
    }
    UTPose utpose(pos);
    return qMakePair(utpose, axes);
}

ScanTaskData ScanTask::data() const
{
    return m_data;
}

QString ScanTask::path() const
{
    return m_path;
}

void ScanTask::setPath(const QString &path)
{
    if (path != m_path) {
        m_path = path;
    }
}

void ScanTask::setErrorString(const QString &error)
{
    m_errorString = error;
    setState(Error);
}

void ScanTask::setTimeRemaining(qreal remain)
{
    m_status.secondsRemain = remain;
    emit progressUpdated();
}

void ScanTask::applySetups() const
{
    const auto setups = m_scanPlan->setups();
    qCDebug(debug) << "Applying" << setups.count() << "setups";
    for (auto it = setups.begin(); it != setups.end(); ++it) {
        UTChannel *channel = UltrasonicManager::channel(it.key());
        if (!channel) {
            continue;
        }
        channel->setSetup(it.value());
    }
    document()->LoadSetups();
}

void ScanTask::lockSetups(bool locked)
{
    WaveformManager::setSetupLock(locked);
}

void ScanTask::calculateTimeRemaining()
{
    if (m_timeDirty) {
        m_timeDirty = false;
        if (hasScanline()) {
            setTimeRemaining(m_status.path.timeRemains(m_status.linesDone, m_status.currentPos));
        } else {
            setTimeRemaining(0);
        }
    }
}

bool ScanTask::tryStopMotion()
{
    qCDebug(debug) << "try stop motion" << elementsEndPoint->isIdle() << prerequisityEndPoint->isIdle();
    if (queue()->GetState() != TrioQueue::STATE_IDLE) {
        qCDebug(debug) << "Stopping queue";
        int ret = queue()->Stop();
        if (ret != TrioQueue::SUCCESS) {
            qCWarning(debug) << "Failed stopping queue:" << ret;
            setErrorString(tr("无法停止扫描程序: %1").arg(ret));
        }
        return false;
    }
    if (!elementsEndPoint->isIdle()) {
        qCDebug(debug) << "Stopping element end-point";
        elementsEndPoint->Cancel();
        return false;
    }
    if (!prerequisityEndPoint->isIdle()) {
        qCDebug(debug) << "Stopping prerequisites end-point";
        prerequisityEndPoint->Cancel();
        return false;
    }
    return true;
}

/*! 开始扫描任务 */
void ScanTask::startTask()
{
    if (queue()->GetState() == TrioQueue::STATE_UNAVAILABLE) {
        setErrorString(tr("运动控制器意外断开"));
        return;
    }

    for (UTInstrument *intrument: UltrasonicManager::instruments()) {
        intrument->SetStandardPos(UTPose(endPoint()->currentPos()));
    }

    if (state() == Idle) {                                       // 由停止状态开始扫描
        m_status = Status();
        setState(Pending);
        m_data.startTime = QDateTime::currentDateTime();
        m_status.path = m_scanPlan->motionPath().expandMultiChannel();
        bool reverse = false;
        m_status.linesTotal = m_status.path.moveCountTo(-1, reverse);
        m_status.secondsTotal = m_status.path.timeExpected();
        prerequisityEndPoint->setAxes(m_status.path.prerequisitesAxes());
        elementsEndPoint->setAxes(m_status.path.ptpAxes());
        emit dataChanged(m_data);
    } else if (state() == Paused) {                              // 暂停之后的恢复
        prerequisityEndPoint->setAxes(m_status.path.prerequisitesAxes());
        elementsEndPoint->setAxes(m_status.path.ptpAxes());
        setState(Restoring);
    } else {
        qCWarning(debug) << "Failed starting task, invalid state" << state();
    }
}

/*! 停止扫描任务 */
void ScanTask::stopTask()
{
    switch (state()) {
    case Pending:
    case Sending:
    case Paused:
        return setState(Idle);
    case Starting:
    case Locating:
    case Running:
    case Pausing:
    case Restoring:
        return setState(Cancelling);
    case Finishing:
    case Cancelling:
        if (tryStopMotion()) {
            setState(Idle);
        }
        break;
    case Idle:
    case Error:
    case Completed:
    default:
        return;
    }
}

/*! 暂停扫描任务 */
void ScanTask::pauseTask()
{
    qCDebug(debug) << "PauseTask";
    setState(Pausing);
}


void ScanTask::setData(const ScanTaskData &data)
{
//    if (m_data == data) {
//        return;
//    }

    m_data = data;
    emit dataChanged(m_data);
}

void ScanTask::setScanPlan(ScanPlan *plan)
{
    m_scanPlan->fromJson(plan->toJson());
}

/*! 更新扫描状态 */
void ScanTask::setState(State newState)
{
    if (newState != m_status.state) {
        qCDebug(debug) << "ScanTask" << m_status.state << "->" << newState;
        if (newState != Error) {
            m_errorString.clear();
        }
        leaveState(m_status.state);
        m_status.state = newState;
        enterState(newState);
        emit stateChanged(state());
        emit progressUpdated();
    }
}

/*! 发送扫描计划 */
void ScanTask::sendScanPlan()
{
    if (m_status.queueTotal > 0) {
        restore();
    } else {
        queue()->Reset();
        queue()->UpdateProgram(m_scanPlan->motionPath());
        m_status.queueHandle = queue()->Send();
        if (m_status.queueHandle < 0) {
            qCCritical(debug) << "Error sending queue" << m_status.queueHandle;
            setState(Error);
        }
    }
}

/*! 定位功能 */
void ScanTask::locate()
{
    QPair<UTPose, MotionAxisList> destPos = progressPosition();
    qCDebug(debug) << "ScanTask locating to" << destPos;
    endPoint()->Cancel();
    if (endPoint()->currentPos().isEqualTo(destPos.second, destPos.first)) {
        qCDebug(debug) << "ScanTask locate pos match. starting.";
        enableDataPath();
        applySetups();
    } else {   // 不想等的情况下，需要调用locate,
        endPoint()->Locate(destPos.first, destPos.second);
    }
    m_located = true;
}

/*! 从暂停中还原 */
void ScanTask::restore()
{
    qCDebug(debug) << "Restoring queue" << m_status.queueHandle << "at line" << m_status.linesDone;
    setState(Running);
    bool isReverse = false;
    m_scanPlan->motionPath().elementByMove(m_status.linesDone, isReverse);
    queue()->Start(m_status.queueHandle, m_status.linesDone, isReverse);
}

/*! 扫描任务状态改变，在进入中间态的过程中，需要执行某些操作，都将在此接口中完成 */
void ScanTask::enterState(State state)
{
    if (state == Completed || state == Paused || state == Error || state == Idle) {
        lockSetups(false);
        MotionManager::driver()->setMode(TrioDriver::ModeManual);
    } else {
        MotionManager::driver()->setMode(TrioDriver::ModeProgram);
    }
    TrioQueue::State queueState = queue()->GetState();

    switch (state) {
    case Idle: {
        setTimeRemaining(0);
        prerequisityEndPoint->setAxes(MotionAxisList());
        elementsEndPoint->setAxes(MotionAxisList());
        break;
    }
    case Pending: {                                                 // 准备阶段发送扫描计划指令，datapath使能
        if (tryStopMotion()) {
            sendScanPlan();
        }
        break;
    }
    case Sending: {
        break;
    }
    case Starting: {
        break;
    }
    case Locating: {                                               // 定位过程需要使用endpoint的locate功能，用之前需要比较当前位置与起始位置是否相同
        locate();
        break;
    }
    case Cancelling: {
        if (tryStopMotion()) {
            setState(Idle);
        }
        break;
    }
    case Running: {
        break;
    }
    case Pausing: {                                                // 暂停阶段，需要确保程序暂停，datapath使能关闭
        qCDebug(debug) << "Pausing queue" << queueState;
        if (queueState == TrioQueue::STATE_RUNNING || queueState == TrioQueue::STATE_PAUSED) {
            int ret = queue()->Stop();
            if (ret != TrioQueue::SUCCESS) {
                qCWarning(debug) << "Failed pausing queue:" << ret;
                setErrorString(tr("无法暂停扫描程序： %1").arg(ret));
            }
        }
        break;
    }
    case Paused: {
        prerequisityEndPoint->setAxes(MotionAxisList());
        elementsEndPoint->setAxes(MotionAxisList());
        break;
    }
    case Restoring: {
        qCDebug(debug) << "Restoring queue" << EnumToKey<TrioQueue::State>(queueState);
        if (tryStopMotion()) {
            restore();
        }
        break;
    }
    case Finishing: {                          // 停止阶段，需要确保program停止，datapath使能关闭
        // 判断当前programe的状态，是暂停还是停止状态，暂停状态需要停止，停滞状态直接disable
        if (queueState == TrioQueue::STATE_PAUSED || queueState == TrioQueue::STATE_RUNNING || queueState == TrioQueue::STATE_SENDING) {
            int ret = queue()->Stop();
            if (ret != TrioQueue::SUCCESS) {  // 如果program已经是暂停或者running状态，需要先确保程序停止，然后datapath使能关闭
                qCWarning(debug) << "Failed stopping queue:" << ret;
                setErrorString(tr("无法停止扫描程序: %1").arg(ret));
            }
        } else if (queueState == TrioQueue::STATE_STARTING) {
            // 暂时这两个过程不能停止，只能等待这两个状态进入running/idle状态之后才可以被停止
        } else if (queueState == TrioQueue::STATE_IDLE || queueState == TrioQueue::STATE_UNAVAILABLE) { // program已经是停止状态，直接关闭datapath使能
            if (tryStopMotion()) {
                disableDataPath();
            }
        }
        break;
    }
    case Completed: {
        m_data.finishTime = QDateTime::currentDateTime();
        emit dataChanged(m_data);
        setTimeRemaining(0);
        if (document()->GetCurrentFilePath().isEmpty()) {
            qCDebug(debug)<<"Can't save file: Empty file path";
            break;
        } else {
            qCDebug(debug)<<"TASK COMPLETED: Saving File";
            document()->setScanTask(this);
            document()->Save();
        }
        break;
    }
    case Error: {
        setTimeRemaining(0);
        queue()->Stop();
        break;
    }
    default:
        break;
    }
}

void ScanTask::leaveState(ScanTask::State state)
{
    if (state == Idle || state == Completed || state == Error || state == Paused) {
        lockSetups(true);
    }
}

/*! 接收queue的状态改变接口 */
void ScanTask::OnQueueStateChanged(int queueState, int progress, int total)
{
    Q_UNUSED(total);
    if (m_queueState != queueState) {
        m_queueState = queueState;
        switch (queueState) {
        case TrioQueue::STATE_UNAVAILABLE: {
            qCDebug(debug) << "Queue state unavailable";
            setErrorString(tr("运动控制器意外断开"));                                // 任意状态下，都可能接收到unavalible信号，这时候直接转入error状态，断开连接
            break;
        }
        case TrioQueue::STATE_IDLE: {
            switch (state()) {
            case Running: {
                setState(Finishing);
                if (tryStopMotion()) {
                    disableDataPath();
                }
                break;
            }
            case Pausing:
            case Sending:
            case Starting:
            case Paused:    // 如果暂停之后的程序很短，很可能收不到由paused转到running的状态信号而直接收到idle
            case Finishing:
            case Cancelling:
            case Error: {
                if (tryStopMotion()) {
                    disableDataPath();
                }
                break;
            }
            default:
                break;
            }
            break;
        }
        case TrioQueue::STATE_SENDING: {
            if (state() == Pending) {
                setState(Sending);
            }
            break;
        }
        case TrioQueue::STATE_STARTING: {
            if (state() == Sending) {
                setState(Starting);
            }
            break;
        }
        case TrioQueue::STATE_RUNNING: {
            switch (state()) {
            case Starting:
            case Locating:
            case Restoring: {
                setState(Running);
                break;
            }
            case Pausing: {
                int ret = queue()->Stop();
                if (ret != TrioQueue::SUCCESS) {
                    qCWarning(debug) << "Failed stopping queue:" << ret;
                    setErrorString(tr("无法暂停扫描程序: %1").arg(ret));
                }
                break;
            }
            default:
                break;
            }
            break;
        }
        case TrioQueue::STATE_PAUSED: {                                          // 检测到program暂停
            switch (state()) {
            case Running:
                if (!m_located) {                                           // 没有定位过说明是起始的断点，需要定位
                    setState(Locating);
                } else {                                                    // 其余情况是程序中的断点
                    queue()->Continue();
                }
                break;
            case Restoring:                                                 // 在RESTORING状态下进入paused状态说明是在执行完前置条件之后进入的断点，这时候需要locate
            case Starting:                                                  // 在starting状态下queue突然暂停，有可能是程序刚启动就进入了暂停断点，这时候需要执行前期准备工作和locate
                setState(Locating);
                break;
            case Pausing:
            case Finishing: {                                               // 停滞状态下进入暂停，有可能是sending或者starting的状态下，被停止
                if (queue()->Stop() != TrioQueue::SUCCESS) {                      // 如果program已经是暂停或者running状态，需要先确保程序停止，然后datapath使能关闭
                    qCWarning(debug) << "Failed stopping queue";
                    setErrorString(tr("无法停止扫描程序"));
                }
                break;
            }
            default:
                break;
            }
            break;
        }
        case TrioQueue::STATE_ERROR: {
            switch (state()) {
            case Sending:
            case Starting:
            case Locating:
            case Running:
            case Pausing:
            case Paused:
            case Restoring:
            case Finishing: {
                qCDebug(debug) << "Queue entered error state";
                setErrorString(tr("扫描程序错误"));
                break;
            }
            default:
                break;
            }
        }
        default:
            break;
        }
    }
    if (m_queueState == TrioQueue::STATE_RUNNING) {
        m_status.linesDone = progress;
        emit progressUpdated();
    } else if (m_queueState == TrioQueue::STATE_SENDING) {
        m_status.queueTotal = total;
        m_status.queueSent = progress;
        emit progressUpdated();
    }
}

/*! 接收dataPath的enable状态完成 */
void ScanTask::enableDataPath()
{
    if (state() == Locating) {                                    // 如果是locating状态下的enable完成，则说明是在定位完成之后的datapath使能
        qCDebug(debug) << "Locate finished. Continue queue";
        if (queue()->Continue() != TrioQueue::SUCCESS) {
            qCWarning(debug) << "Can't continue queue";
            setErrorString(tr("无法恢复执行扫描"));
            return;
        }
        setState(Running);   // 定位完成之后，很可能收不到queue转到running状态的信号，而直接转入了IDLE,所以需要在我们定位完成之后，直接手动修改当前状态为running，导致程序将endpoint的idle信号误认为是定位结束，实际上是程序结束
    }
    for (UTChannel *channel: UltrasonicManager::channels()) {
        connect(channel, &UTChannel::dataArrived, this, &ScanTask::onDataArrived, Qt::UniqueConnection);
    }
}

/*! 接受datapath的disable状态完成 */
void ScanTask::disableDataPath()
{
    if (state() == Finishing) {
        setState(Completed);
        m_located = false;
    } else if (state() == Sending || state() == Starting || state() == Cancelling) {
        setState(Idle);
        m_located = false;                                                     // 停止之后重置定位状态
    } else if (state() == Pausing) {
        setState(Paused);
        m_located = false;                                                     // 暂定之后重置定位状态
    } else if (state() == Running) {
        if (!m_scanPlan || !endPoint()) {
            return;
        }
        QPair<UTPose, MotionAxisList> destPosInfo = progressPosition(false);
        if (endPoint()->currentPos().isEqualTo(destPosInfo.second, destPosInfo.first)) {
            setState(Completed);
        } else {
            qCDebug(debug) << "Error: final pose mismatch" << destPosInfo.second << endPoint()->currentPos() << destPosInfo.first;
            setState(Error);
        }
    }
    for (UTChannel *channel: UltrasonicManager::channels()) {
        channel->disconnect(this);
    }
}

/*! 前提轴列表idle */
void ScanTask::OnPrerequisityEndPointIdle()
{
    if (state() == Pending) {
        if (tryStopMotion()) {
            sendScanPlan();
        }
    } else if (state() == Restoring) {
        if (tryStopMotion()) {
            restore();
        }
    } else if (state() == Finishing || state() == Pausing || state() == Cancelling) {
        if (tryStopMotion()) {
            disableDataPath();
        }
    } else if (state() == Error) {
        setState(Idle);
        m_located = false;                                                     // idle之后需要重置定位状态
    } else if (state() == Sending || state() == Starting || state() == Running) {
        if (queue()->GetState() == TrioQueue::STATE_IDLE) {
            disableDataPath();
        }
    }
}

/*! PTP的轴列表idle */
void ScanTask::OnElementsEndPointIdle()
{
    qCDebug(debug) << "Element end-point idle" << state();
    if (state() == Pending) {
        if (tryStopMotion()) {
            sendScanPlan();
        }
    } else if (state() == Locating) {
        QPair<UTPose, MotionAxisList> destPosInfo = progressPosition();
        if (endPoint()->currentPos().isEqualTo(destPosInfo.second, destPosInfo.first)) {  // 定位完成
            qCDebug(debug) << "Locate finished";
            enableDataPath();
            applySetups();
        } else {
            qCWarning(debug) << "Error: final pose mismatch" << endPoint()->currentPos() << progressPosition();
            setErrorString("机械臂定位失败，请检查限位");
        }
    } else if (state() == Restoring) {
        if (tryStopMotion()) {
            restore();
        }
    } else if (state() == Finishing || state() == Pausing || state() == Cancelling) {
        if (tryStopMotion()) {
            disableDataPath();
        }
    } else if (state() == Error) {
        setState(Idle);
        m_located = false;                                                     // idle之后重置定位状态
    } else if (state() == Sending || state() == Starting) {
        if (tryStopMotion()) {
            disableDataPath();
        }
    }
}

/*! 接收endPoint的位置改变信号的槽函数 */
void ScanTask::OnEndPointPositionChanged(const MotionPose &pos)
{
    if (hasScanline()) {
        m_status.currentPos = pos;
        if (!m_timeDirty) {
            QTimer::singleShot(500, this, &ScanTask::calculateTimeRemaining);
            m_timeDirty = true;
        }
    }
}

void ScanTask::onDataArrived(const UTDataSegment &data)
{
    document()->OnDataArrived(data);
    for (UTLayer *layer: document()->GetLayerMap()) {
        layer->OnDataArrived(data);
    }
}
