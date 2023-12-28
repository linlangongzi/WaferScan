#pragma once

#include <QObject>
#include "allrising_global.h"
#include <functional>
#include <QHash>

class ALLRISING_UTILS_EXPORT ThrottleUpdate : public QObject
{
    Q_OBJECT

public:
    explicit ThrottleUpdate(QObject *parent = nullptr);

    void updateLater();

    template <class Func>
    static void call(Func func)
    {
        Key key = qMakePair(nullptr, reinterpret_cast<void *>(func));
        instance()->functions[key] = func;
        instance()->updateLater();
    }

    template <typename R, typename Func>
    static void call(R *r, Func func)
    {
        union {
            Func func;
            void *pointer;
        } f;
        f.func = func;
        Key key(r, f.pointer);
        instance()->functions[key] = [=]() {(r->*func)();};
        instance()->updateLater();
    }

private:
    bool event(QEvent *event) override;
    static ThrottleUpdate *instance();

    typedef QPair<void *, void *> Key;
    typedef std::function<void()> Callback;
    QHash<Key, Callback> functions;
    bool dirty = false;
};
