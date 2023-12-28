#pragma once

#include <Imaging/UTImage.h>
#include <QObject>
#include <Motion/TrioQueue.h>
#include <allrising/MetaType/MetaType.h>

class ScanPlan;
class EndPoint;

namespace Motion {
class TrioQueue;
}

class ScanTaskData
{
    Q_GADGET

    Q_PROPERTY(QDateTime createTime MEMBER createTime)
    Q_PROPERTY(QDateTime startTime MEMBER startTime)
    Q_PROPERTY(QDateTime finishTime MEMBER finishTime)
    Q_PROPERTY(QString filePath MEMBER filePath)
    Q_PROPERTY(QString taskOperator MEMBER taskOperator)
    Q_PROPERTY(QMap<QString, QString> customData MEMBER customData)

public:
    DECLARE_COMPARE_OPERATORS(ScanTaskData)

    QDateTime createTime;
    QDateTime startTime;
    QDateTime finishTime;
    QString filePath;
    QString taskOperator;
    QMap<QString, QString> customData;
};

class ScanTask : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ScanTaskData data READ data WRITE setData NOTIFY dataChanged)

public:
    enum State {
        Idle = 0,
        Pending,
        Sending,
        Starting,
        Locating,
        Running,
        Pausing,
        Paused,
        Restoring,
        Cancelling,
        Finishing,
        Completed,
        Error
    };
    Q_ENUM(State)

    struct Status
    {
        State state = Idle;
        int queueSent = 0;
        int queueTotal = 0;
        int queueHandle = -1;
        int linesDone = 0;
        int linesTotal = 0;
        MotionPose currentPos;
        qreal secondsTotal = 0;
        qreal secondsRemain = 0;
        MotionPath path;
    };

public:
    explicit ScanTask(ScanPlan *plan, QObject *parent = nullptr);
    ScanPlan *scanPlan() const;
    State state() const {return m_status.state;}
    Status status() const {return m_status;}
    QString stateText() const;
    QVariant stateIcon() const;
    QString errorString() const {return m_errorString;}
    ScanTaskData data() const;

    bool canStart() const;
    bool canPause() const;
    bool canStop() const;
    bool canRemove() const;
    bool canEdit() const;
    bool hasProgress() const;
    bool hasProgressBar() const;
    bool hasScanline() const;
    qreal percent() const;
    qreal timeRemaining() const {return m_status.secondsRemain;}
    qreal timeExpected() const {return m_status.secondsTotal;}

    /*! 根据进度报告计算当前位置 */
    QPair<UTPose, MotionAxisList> progressPosition(bool isStart = true);

signals:
    void stateChanged(int state);
    void progressUpdated();
    void dataChanged(ScanTaskData data);

public slots:
    void startTask();
    void stopTask();
    void pauseTask();
    void setData(const ScanTaskData &data);
    void setScanPlan(ScanPlan *plan);
    QString path() const;
    void setPath(const QString &path);

private:
    void sendScanPlan();
    void locate();
    void restore();

private slots:
    void setState(State state);
    void enterState(State state);
    void leaveState(State state);
    void setErrorString(const QString &error);
    void setTimeRemaining(qreal remain);
    void applySetups() const;
    void lockSetups(bool locked);
    void calculateTimeRemaining();
    bool tryStopMotion();
    void OnQueueStateChanged(int queueState, int progress, int total);
    void enableDataPath();
    void disableDataPath();
    void OnPrerequisityEndPointIdle();
    void OnElementsEndPointIdle();
    void OnEndPointPositionChanged(const MotionPose &pos);
    void onDataArrived(const UTDataSegment &data);

private:
    ScanPlan *m_scanPlan = nullptr;
    EndPoint *prerequisityEndPoint;
    EndPoint *elementsEndPoint;
    int m_queueState = Motion::TrioQueue::STATE_IDLE;
    bool m_timeDirty = false;
    bool m_located = false;
    QString m_errorString;
    Status m_status;
    ScanTaskData m_data;
    QString m_path;
    friend class ScanTaskModel;
};
