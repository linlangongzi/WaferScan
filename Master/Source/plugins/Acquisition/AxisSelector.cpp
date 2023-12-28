#include "AxisSelector.h"
#include <Motion/MotionManager.h>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "acquire.axis", QtWarningMsg);

AxisSelector::AxisSelector(QWidget *parent) :
    QComboBox(parent)
{
    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), [this] (int currentIndex) {
        const QUuid id = itemData(currentIndex).toUuid();
        qCDebug(debug) << "Current axis" << id;
        emit currentAxisUuidChanged(id);
    });
    setFilter(filter);
}

Axis *AxisSelector::currentAxis() const
{
    QUuid id = currentData().toUuid();
    return MotionManager::axis(id);
}

void AxisSelector::setFilter(Filter newFilter)
{
    filter = newFilter;

    QSignalBlocker blocker(this);
    clear();
    qCDebug(debug) << "Set axis filter" << std::addressof(newFilter);
    foreach (Axis *axis, MotionManager::axisList()) {
        if (!filter || filter(axis)) {
            addItem(axis->config().name, axis->uuid());
            qCDebug(debug) << "Item axis address" << axis <<"Name: "<< axis->config().name << axis->uuid();
        }
    }
    setCurrentIndex(-1);
    blocker.unblock();
}

QUuid AxisSelector::currentAxisUuid() const
{
    return currentData().toUuid();
}

void AxisSelector::setCurrentAxis(Axis *axis)
{
    if (axis != nullptr) {
        setCurrentUuid(axis->uuid());
    }
}

void AxisSelector::setCurrentUuid(const QUuid &axisUuid)
{
    int index = findData(axisUuid);
    qCDebug(debug) << "Set current axis" << axisUuid << "at index" << index;
    if (index >= 0) {
        setCurrentIndex(index);
    }
}

int AxisSelector::currentAxisIndex()
{
    return MotionManager::axisIndex(currentAxisUuid());
}
