#include "AECRuningWork.h"
#include "Platform/Platform.h"

AECRuningWork::AECRuningWork(QObject *parent) :
    CardRunningWork(parent, 5)
{
    ConnectTo();
}

void AECRuningWork::ConnectTo()
{
    for(CardWorkVector::iterator it = platform()->GetAecWork()->begin(); it != platform()->GetAecWork()->end(); ++it)
    {
        sharedCardWork aecWork = *it;
        connect(this, SIGNAL(Runing()), aecWork.data(), SLOT(Run()));
    }
}
