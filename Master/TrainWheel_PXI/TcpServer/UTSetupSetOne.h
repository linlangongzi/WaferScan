#ifndef UTSETUPSETONE_H
#define UTSETUPSETONE_H

#include <QObject>
#include <QVariant>
#include <QJsonValue>
#include <QSharedPointer>
#include "TcpServer/DataDef.h"

class UTSetupSetOne : public QObject
{
    Q_OBJECT

public:
    explicit UTSetupSetOne(QObject *parent = 0);

signals:
    void SetOne(UTSharedDataParam param, qint32 channel);

public slots:
    void ReceiveSetOneData(UTSharedDataParam param, qint32 channel);

private:
    void ConnectTo();
};

#endif // UTSETUPSETONE_H
