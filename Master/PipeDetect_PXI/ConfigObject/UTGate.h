#ifndef UTGATE_H
#define UTGATE_H

#include <QObject>
#include <QJsonValue>
#include "UTSetup.h"

class UTSetup;

class UTGate : public QObject
{
    Q_OBJECT

public:
    explicit UTGate(int gateIndex, QObject *parent = 0);

signals:
    void ValueChanged(const QString &key, const QJsonValue &value);
    void RequestAnswer(const QString &key, const QJsonValue &value);
    void ParameterLimit(const QString &key, const QJsonValue &value);

public slots:
    void UTSetupParameterChanged(const QString &key, const QJsonValue &value);
    void RequestUpdate(const QString &key);
    void SetParameter(const QString &key, const QJsonValue &value);
    void ActionParameterLimit(const QString &key, const QJsonValue &value);

public:
    QJsonValue Get(const QString key);

    UTSetup *GetUTSetup();

    int GetIndex();

private:
    QString parameterPrefixFlaw;
    QString parameterPrefixSurface;

    const int GateIndex;
};

#endif // UTGATE_H
