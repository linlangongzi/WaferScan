#include "ThrottleUpdate.h"
#include <QCoreApplication>
#include <QEvent>
#include <QThreadStorage>

static QThreadStorage<ThrottleUpdate *> managers;

static const int UpdateEvent = QEvent::registerEventType();

ThrottleUpdate::ThrottleUpdate(QObject *parent) : QObject(parent)
{
}

void ThrottleUpdate::updateLater()
{
    if (!dirty) {
        dirty = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::Type(UpdateEvent)), Qt::LowEventPriority);
    }
}

bool ThrottleUpdate::event(QEvent *event)
{
    if (event->type() == UpdateEvent && dirty) {
        dirty = false;
        QHash<Key, Callback> funcList;
        qSwap(funcList, functions);
        for (const auto &func: funcList) {
            func();
        }
        return true;
    }
    return QObject::event(event);
}

ThrottleUpdate *ThrottleUpdate::instance()
{
    if (!managers.hasLocalData()) {
        managers.setLocalData(new ThrottleUpdate);
    }
    return managers.localData();
}
