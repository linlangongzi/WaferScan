#pragma once

#include "ScanTask.h"
#include <QAbstractTableModel>

/*!
 * \brief ScanTaskModel 提供一个 ScanTask 的列表, 供界面显示及 ScanController 控制.
 * ScanTaskModel 中管理以下资源:
 *  tasks: 扫描任务列表. ScanTaskModel 拥有 tasks 中全部任务的所有权.
 *  currentTask: 当前活跃的扫描任务. 当前没有活跃任务时为 nullptr. 当前活跃的任务属于 tasks
 */
class ScanTaskModel : public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(ScanTask *currentTask READ currentTask WRITE setCurrentTask NOTIFY currentTaskChanged)

public:
    enum Columns {
        ColStatus,
        ColName,
        ColProgress,
        ColOperator
    };
    Q_ENUM(Columns)

    explicit ScanTaskModel(QObject *parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QList<ScanTask *> tasks() const;

    ScanTask *currentTask() const;
    ScanTask *taskFromIndex(const QModelIndex &index) const;

protected:
    void addTask(ScanTask *task);
    void removeTask(ScanTask *task);
    void clear();

public slots:
    void setCurrentTask(ScanTask *currentTask);

signals:
    void currentTaskChanged(ScanTask *currentTask);

private slots:
    void onTaskStateChanged();
    void notifyTaskChanged(ScanTask *task);

private:
    QList<ScanTask *> m_tasks;
    ScanTask *m_currentTask = nullptr;
    friend class ScanController;
};
