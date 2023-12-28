#include "ScanTaskModel.h"
#include "ScanPlan.h"
#include <allrising/Util.h>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "acquire.task.model");

ScanTaskModel::ScanTaskModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant ScanTaskModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        return QString::number(section + 1);
    } else if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case ColStatus:     return tr("状态");
        case ColName:       return tr("名称");
        case ColProgress:   return tr("进度");
        case ColOperator:   return tr("操作人");
        default:            return QString();
        }
    }
    return QVariant();
}

int ScanTaskModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_tasks.count();
}

int ScanTaskModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return EnumValueList<Columns>().count();
}

QVariant ScanTaskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int col = index.column();
    ScanTask *task = m_tasks.value(index.row());
    if (!task) {
        qCCritical(debug) << "Invalid task on row" << index.row();
        return QVariant();
    }
    if (role == Qt::FontRole && m_currentTask == task) {
        QFont font;
        font.setBold(true);
        return font;
    }
    if (col == ColName && role == Qt::DisplayRole) {
        return task->path();
    } else if (col == ColStatus && role == Qt::DisplayRole) {
        return task->stateText();
    } else if (col == ColStatus && role == Qt::DecorationRole) {
        return task->stateIcon();
    } else if (col == ColProgress && role == Qt::DisplayRole) {
        if (task->hasProgress()) {
            return tr("%1 %").arg(task->percent(), 0, 'f', 0, QChar());
        }
    }
    // FIXME: Implement me!
    return QVariant();
}

QList<ScanTask *> ScanTaskModel::tasks() const
{
    return m_tasks;
}

void ScanTaskModel::addTask(ScanTask *task)
{
    qCDebug(debug) << "Adding task" << m_tasks.count() << task;
    beginInsertRows(QModelIndex(), m_tasks.count(), m_tasks.count());
    m_tasks.append(task);
    connect(task, &ScanTask::stateChanged, this, &ScanTaskModel::onTaskStateChanged);
    connect(task, &ScanTask::progressUpdated, this, &ScanTaskModel::onTaskStateChanged);
    endInsertRows();
}

ScanTask *ScanTaskModel::taskFromIndex(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return nullptr;
    }
    //! 判断当前QModelIndex是否属于ScanTaskModel
    if (index.model() != this) {
        qCCritical(debug) << "Model mismatch";
        return nullptr;
    }
    return m_tasks.value(index.row());
}

/*!
 * \brief 删除任务
 *  如果是设置的当前任务，则把当前任务置空
 */
void ScanTaskModel::removeTask(ScanTask *task)
{
    //! 如果删除的任务是当前任务，则置空，并发出TaskChanged信号
    if (task == currentTask()) {
        setCurrentTask(nullptr);
    }
    const int index = m_tasks.indexOf(task);
    if (index == -1) {
        qCCritical(debug) << "Can't remove task: Invalid task";
        return;
    }
    //! 通过使用beginRemoveRows，通知model进行删除
    beginRemoveRows(QModelIndex(), index, index);
    //! 从QList中删除任务
    m_tasks.removeAt(index);
    //! 使用endRemoveRows，通知Model进行数据更新
    endRemoveRows();
}

void ScanTaskModel::clear()
{
    beginResetModel();
    setCurrentTask(nullptr);
    qDeleteAll(m_tasks);
    m_tasks.clear();
    endResetModel();
}

ScanTask *ScanTaskModel::currentTask() const
{
    return m_currentTask;
}

void ScanTaskModel::setCurrentTask(ScanTask *task)
{
    if (m_currentTask == task) {
        return;
    }
    qSwap(m_currentTask, task);  // m_currentTask = task;
    emit currentTaskChanged(m_currentTask);
    notifyTaskChanged(m_currentTask);   // 当前task标识改变.
    notifyTaskChanged(task);
}

void ScanTaskModel::onTaskStateChanged()
{
    auto *task = qobject_cast<ScanTask *>(sender());
    notifyTaskChanged(task);
}

void ScanTaskModel::notifyTaskChanged(ScanTask *task)
{
    int row = m_tasks.indexOf(task);
    if (!task || row < 0) {
        return;
    }
    emit dataChanged(index(row, 0), index(row, columnCount() - 1));
}
