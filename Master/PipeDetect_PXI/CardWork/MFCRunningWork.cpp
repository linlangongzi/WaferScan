#include "MFCRunningWork.h"
#include "Platform/Platform.h"

MFCRunningWork::MFCRunningWork(QObject *parent) :
    CardRunningWork(parent, 10)
{
    ConnectTo();
}

void MFCRunningWork::ConnectTo()
{
    MFCRunningWork *work = dynamic_cast<MFCRunningWork*>(platform()->GetMfcWork().data());
    connect(this, SIGNAL(Runing()), work, SLOT(Run()));
}
