#include "TrioWarden.h"
#include "TrioAllocator.h"
#include "TrioDriver.h"
#include <QLoggingCategory>
#include <allrising/Util.h>

static Q_LOGGING_CATEGORY(debug, "trio.warden");

namespace Motion {

TrioWarden::TrioWarden(TrioDriver *parent) :
    QObject(parent),
    p(parent),
    addr(-1)
{
    addr = p->m_vrAlloc->Alloc(1);
    qCDebug(debug) << "Warden using vr addr" << addr;
}

void TrioWarden::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    p->setVr(addr, 10000);
}

void TrioWarden::start()
{
    return;
    if (timerId < 0) {
        timerId = startTimer(1000);
    }
    QString program = ReadFileAsString(":/Motion/WARDEN.BAS");
    p->setProgram("WARDEN", QStringList() << program.arg(addr).arg(p->maxAxisId()));
    p->runProgram("WARDEN");
}

void TrioWarden::stop()
{
    if (timerId >= 0) {
        killTimer(timerId);
        timerId = -1;
    }
}

} // namespace Motion

