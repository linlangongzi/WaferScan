#ifndef AECRUNINGWORK_H
#define AECRUNINGWORK_H

#include <QSharedPointer>
#include "CardWork/CardRunningWork.h"

class AECRuningWork : public CardRunningWork
{
    Q_OBJECT
public:
    explicit AECRuningWork(QObject *parent = 0);

private:
    virtual void ConnectTo();
};

typedef QSharedPointer<AECRuningWork> sharedAECRuningWork;

#endif // AECRUNINGWORK_H
