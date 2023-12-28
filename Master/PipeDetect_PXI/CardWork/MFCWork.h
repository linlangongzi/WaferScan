#ifndef MFCWORK_H
#define MFCWORK_H

#include <QObject>
#include "CardWork/CardDataDefine.h"
#include "TcpServer/DataDef.h"
#include "CardWork/CardWorkBase.h"
#include <QVariant>

class MFCWork : public CardWorkBase
{
    Q_OBJECT

public:
    typedef CardWorkBase::MFCDataMap MFCDataMap;

public:
    explicit MFCWork(QObject *parent = 0);

public slots:
    void SetSimulationData(bool isOpen = 0);
    bool SetPRF(quint16 prf);
    void SetData(quint32 address, quint16 data);
    void SetDataMap(MFCDataMap &dataMap);
    void SetCoderMark(quint16 mark);

private:
    bool CardReading();

};

typedef QSharedPointer<MFCWork> sharedMFCWork;

#endif // MFCWORK_H
