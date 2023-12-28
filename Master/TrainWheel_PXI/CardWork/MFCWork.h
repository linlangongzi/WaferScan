#ifndef MFCWORK_H
#define MFCWORK_H

#include <QObject>
#include "TcpServer/DataDef.h"
#include "CardWork/CardWorkBase.h"
#include "CardWork/CardDataDefine.h"

class MFCWork : public CardWorkBase
{
    Q_OBJECT
public:
    explicit MFCWork(QObject *parent = 0);

public slots:
    void SetSimulationData(UTSharedDataSimulation simulation);

private:
    virtual bool InitCard();
    virtual bool CardReading();
    QByteArray readMfcData();

signals:
    void sendMfcData(QByteArray byteArray);
};

typedef QSharedPointer<MFCWork> sharedMFCWork;

#endif // MFCWORK_H
