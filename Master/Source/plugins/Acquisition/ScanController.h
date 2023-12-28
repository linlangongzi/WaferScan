#pragma once

#include "ScanTaskModel.h"

#include <QObject>

class ScanTask;
class ScanPlan;

class ScanController : public QObject
{
    Q_OBJECT

public:
    enum State {
        IDLE = 0,
        RUNNING,
        PAUSING,
        PAUSED,
        FINISHING
    };
    Q_ENUM(State)

public:
    explicit ScanController(QObject *parent = nullptr);
    ~ScanController();

public:
    static ScanController *instance();

    ScanTaskModel *taskModel();
    void LoadScanPlan(ScanPlan *root);
    State currentState();
    void UpdateState(State state);
    QString errorString();
    ScanTask *currentTask() const;

    void setCurrentTask(ScanTask *task);
    ScanTask *createTask(ScanPlan *plan);
    void removeTask(ScanTask *task);
    void removeTasks();

    bool canStart(ScanTask *task) const;
    bool canPause() const;
    bool canStop() const;
    bool canRemove(ScanTask *task) const;
    bool canEdit(ScanTask *task) const;

signals:
    void StateChanged(int state);
    void currentTaskUpdated();

public slots:
    void Start();
    void Stop();
    void Pause();

private:
    void StartTask(ScanTask *task);

private slots:
    void OnScanTaskStateChanged(int taskState);

private:
    ScanTaskModel m_taskModel;
    State m_currentState = IDLE;
    ScanPlan *m_rootScanPlan = nullptr;
};
