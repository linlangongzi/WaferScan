#include "EndPointSelector.h"
#include <Motion/EndPoint.h>
#include <Motion/MotionManager.h>

EndPointSelector::EndPointSelector(QWidget *parent) :
    QComboBox(parent)
{
    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), [this] (int currentIndex) {
        const QUuid id = itemData(currentIndex).toUuid();
        emit currentUuidChanged(id);
    });
    setFilter(m_filter);
}

EndPoint *EndPointSelector::currentEndPoint() const
{
    const QUuid id = currentData().toUuid();
    return MotionManager::endPoint(id);
}

QUuid EndPointSelector::currentId() const
{
    return currentData().toUuid();
}

void EndPointSelector::setFilter(EndPointSelector::Filter filter)
{
    const QUuid prevEndPoint = currentId();
    m_filter = filter;
    clear();
    foreach (EndPoint *endPoint, MotionManager::endPoints()) {
        if (!m_filter || m_filter(endPoint)) {
            addItem(endPoint->description(), endPoint->uuid());
        }
    }
    setCurrentUuid(prevEndPoint);
}

void EndPointSelector::setCurrentEndPoint(EndPoint *endPoint)
{
    if (endPoint != nullptr) {
        setCurrentUuid(endPoint->uuid());
    }
}

void EndPointSelector::setCurrentUuid(const QUuid &endPointId)
{
    const int index = findData(endPointId);
    if (index < 0) {
        return;
    }
    setCurrentIndex(index);
}
