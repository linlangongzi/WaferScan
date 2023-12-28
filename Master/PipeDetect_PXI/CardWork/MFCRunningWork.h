#ifndef MFCRUNNINGWORK_H
#define MFCRUNNINGWORK_H

#include <QSharedPointer>
#include "CardWork/CardRunningWork.h"

class MFCRunningWork : public CardRunningWork
{
    Q_OBJECT
public:
    explicit MFCRunningWork(QObject *parent = 0);

private:
    virtual void ConnectTo();
};

typedef QSharedPointer<MFCRunningWork> sharedMFCRunningWork;

#endif // MFCRUNNINGWORK_H

