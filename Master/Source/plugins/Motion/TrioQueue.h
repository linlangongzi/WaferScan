#pragma once

#include <QObject>
#include <QStringList>
#include <QTime>
#include "MotionPath.h"

namespace Motion {

class TrioDriver;

static const int QUEUE_PROCESS_NUMBER = 5;

/*! Trio 程序状态，来自 TrioBASIC 文档 PROC_STATUS */
enum ProgramStatus {
    PROGRAM_UNKNOWN = -1,   /*!< 尚未从控制器获取 */
    PROGRAM_STOPPED = 0,    /*!< 已停止 */
    PROGRAM_RUNNING = 1,    /*!< 正在运行 */
    PROGRAM_STEPPING = 2,
    PROGRAM_PAUSED = 3,
    PROGRAM_PAUSING = 4,
    PROGRAM_STOPPING = 5,
    PROGRAM_SUSPEND = 6,
    PROGRAM_SUSPENDED = 7,
    PROGRAM_SLEEP = 8,
};

class MOTION_EXPORT TrioQueue : public QObject
{
    Q_OBJECT

public:
    using Motion = QMap<int, double>;
    using Cam = QList<double>;
    using CamMotion = QMap<int, Cam>;
    enum ReturnState {
        SUCCESS = 0,
        ERR_NO_CONNECTION,
        ERR_WRONG_STATE,
        ERR_BAD_STRUCTURE,
    };
    enum State {
        STATE_UNAVAILABLE = 0,
        STATE_IDLE,
        STATE_SENDING,
        STATE_STARTING,
        STATE_RUNNING,
        STATE_PAUSED,
        STATE_ERROR
    };
    Q_ENUM(State)

//    Q_PROPERTY(State state READ GetState NOTIFY StateUpdated)

public:
    explicit TrioQueue(TrioDriver *parent);
    int Send();
    int Start(int queueHandle, int index = 0, bool isReverse = false);
    int Stop();
    int Continue();
    int Reset();
    State GetState() const;
    void UpdateProgram(const MotionPath &path);
    void MoveElement(const MotionElement &prev, const MotionElement &next, int startIndex, bool isReverse, const QList<RotationInfo> &rotations, const QList<MotionElement> &prerequisities = QList<MotionElement>());
    void MovePoseList(const MotionElement &element, const QList<MotionPose> &intervals, const QString &i, bool isReversed = false,
                      const QList<MotionElement> &prerequisities = QList<MotionElement>(), const QList<RotationInfo> &rotations = QList<RotationInfo>());
    void MovePose(const MotionAxisList &axes, qreal speed, int mainAxis, const MotionPose &a, const MotionPose &b = MotionPose(), const QString &i = "",
                  const QList<MotionElement> &prerequisities = QList<MotionElement>(), const QList<RotationInfo> &rotations = QList<RotationInfo>());
    void MoveLink(const QString &destPos, const QString &linkDestPos, int currentAxis, int linkAxis);
    void AddParameter(const QString &parameter, const QString &value);
    void reverseBoolean(const QString &parameter);
    void While(const QString &text);
    void Wend();
    void If(const QString &text);
    void ElseIf(const QString &text = QString());
    void Endif();
    void WaitConditions(const QList<MotionCondition> &conditions);
    void CheckCondition(const QString &result, const MotionCondition &condition, const QString &state);
    void ResetPrerequisityAxes(const QList<MotionElement> &elements);
    void WaitRising(int sensor);
    void WaitFalling(int sensor);
    void Repeat();
    void Until(const QString &condition);
    void CancelAxesLink(const MotionAxisList &axes, int mainAxis);
    void Quit();

    void Sync(const QString &name, bool pause = false);
    void SetVariable(const QString &name, const QString &value);
    void SetAxisParameter(int axis, const QString &key, double value);
    void SetAxisParameter(int axis, const QString &key, const QString &value);
    void ResetAxisPos(qreal pos, int axis);
    void SetSpeed(int axis, double speed);
    void SetSpeed(int axis, const QString &speed);
    void Cancel(int axis);
    void Cancel(const QList<int> &axis);
    void MoveContinuous(int axis, bool isReversed = false);
    void Move(const Motion &displacement);
    void Move(const QString &pos, int axis);
    void MoveAbs(const Motion &pos);
    void MoveAbs(qreal pos, int axis);
    void MoveAbs(const MotionPose &pos, const MotionAxisList &axis);
    void MoveCam(const MotionElement &element, double speed, double returnSpeed);
    void MoveBSpline(const CamMotion &motion, double speed);
    void MoveCircle(int axisX, int axisY, int axisC, const QPointF &center, qreal radius, qreal startAngle, qreal offsetAngle, qreal moveAngle);
    void CamBox(int tableStart, int tableEnd, int axis, qreal linkScale, int linkAxis, qreal linkDistance, qreal linkOffset);
    void Home(int axis, double speed, double creep, int datumIn, bool isReversed);
    void WaitIdle(int axis = -1);
    void WaitInput(int inputIo, bool waitForOff = false);
    void reportMoveStart(int id);
    void reportMoveStart(const QString &id);
    void reportMoveEnd(int id);
    void reportMoveEnd(const QString &id);
    void reportError(const QString &error);
    void addReport(const QString &token, const QString &id);
    void AddBreakPoint();
    void CheckAxisPosition(qreal pos, int axis, const QString &op = ">");
    void OutputIo(int outputIo, bool value = false);
    void Wait(int msec);
    void SetTable(int offset, const QVector<qreal> &data);

    void For(const QString &key, int from, int to, int step = 1);
    void For(const QString &key, const QString &from, const QString &to, int step = 1);
    int Next(const QString &key);

signals:
    void StateUpdated(int state, int progress, int total);

private slots:
    void onDisconnected();
    void OnTokenReceived(const QString &token);
    void OnStartTimeout();
    void setProgramStatus(int status);
    void OnCommandSent(int id);

private:
    using AxisList = QList<int>;

    Motion CheckAxisRange(const Motion &table);
    bool UpdateBase(const MotionAxisList &newBase);
    void UpdateState(State newState, int newProgress, int newTotal);

    TrioDriver *p;
    int m_programStatus = PROGRAM_UNKNOWN;
    int vrAddr = 0;
    int camBase = 0;
    QStringList queue;
    MotionAxisList base;
    QStringList callStack;
    bool isVirtualConfigured = false;
    State state = STATE_UNAVAILABLE;
    QTime stateTime;
    int progress = -1;
    int total = -1;
    int baseCommandId = 0;
    int finalCommandId = 0;
    QString errorString;

    friend class TrioDriver;
    static int handle;
};

}   /* namespace Motion */
