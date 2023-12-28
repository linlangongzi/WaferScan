#include "UTGate.h"
#include <QDebug>

UTGate::UTGate(int gateIndex, QObject *parent) :
    QObject(parent),
    GateIndex(gateIndex)
{
    parameterPrefixFlaw = QString("FlawGate%1").arg(gateIndex + 1);
    parameterPrefixSurface = QString("SurfaceGate%1").arg(gateIndex + 1);
    connect(GetUTSetup(), SIGNAL(ParameterLimit(QString, QJsonValue)), this, SLOT(ActionParameterLimit(QString, QJsonValue)));
}

void UTGate::UTSetupParameterChanged(const QString &key, const QJsonValue &value)
{
    if (key.contains(parameterPrefixFlaw))
    {
        QString partKey;
        QRegExp RegExp("FlawGate\\d+(.*)$");
        int pos = 0;
        if (RegExp.indexIn(key, pos) != -1)
        {
            partKey = RegExp.cap(1);
        }
        emit ValueChanged(partKey, value);
    }
    else if (key.contains(parameterPrefixSurface))
    {
        QString partKey;
        QRegExp RegExp("SurfaceGate\\d+(.*)$");
        int pos = 0;
        if (RegExp.indexIn(key, pos) != -1)
        {
            partKey = RegExp.cap(1);
        }
        emit ValueChanged(partKey, value);
    }
}

void UTGate::RequestUpdate(const QString &key)
{
    emit RequestAnswer(key, Get(key));
}

void UTGate::SetParameter(const QString &key, const QJsonValue &value)
{
    if (key == "StartFix")  // 如果是闸门左伸缩的信号
    { 
        QString startKey;
        QString widthKey;

        if (GetUTSetup()->GetWaveType() == "Flaw")
        {
            startKey = parameterPrefixFlaw;
            widthKey = parameterPrefixFlaw;
        }
        else
        {
            startKey = parameterPrefixSurface;
            widthKey = parameterPrefixSurface;
        }

        startKey.append("Start");                       // fullKey Start
        widthKey.append("Width");                       // fullKey Width
        double start = Get("Start").toDouble();         // 获取表中的原始start
        double end = Get("Width").toDouble() + start;   // 获取表中原始的width，算出原始的end
        GetUTSetup()->Set(startKey, value);             // 写入新的start
        start = Get("Start").toDouble();                // 获取新的start
        GetUTSetup()->Set(widthKey, end - start);       // 根据end不变原理，算出新的width，写入新的width
        return;
    }
    if (GetUTSetup()->GetWaveType() == "Flaw")
    {
        QString fullKey(parameterPrefixFlaw);
        fullKey.append(key);
        GetUTSetup()->Set(fullKey, value);
    }
    else if (GetUTSetup()->GetWaveType() == "Surface")
    {
        QString fullKey(parameterPrefixSurface);
        fullKey.append(key);
        GetUTSetup()->Set(fullKey, value);
    }
}

void UTGate::ActionParameterLimit(const QString &key, const QJsonValue &value)
{
    if (key.contains(parameterPrefixFlaw))
    {
        QString partKey;
        QRegExp RegExp("FlawGate\\d+(.*)$");
        int pos = 0;
        if (RegExp.indexIn(key, pos) != -1)
        {
            partKey = RegExp.cap(1);
        }
        emit ParameterLimit(partKey, value);
    }
    else if (key.contains(parameterPrefixSurface))
    {
        QString partKey;
        QRegExp RegExp("SurfaceGate\\d+(.*)$");
        int pos = 0;
        if (RegExp.indexIn(key, pos) != -1)
        {
            partKey = RegExp.cap(1);
        }
        emit ParameterLimit(partKey, value);
    }
}

QJsonValue UTGate::Get(const QString key)
{
    QString fullKey;
    if (GetUTSetup()->GetWaveType() == "Flaw")
    {
        fullKey = parameterPrefixFlaw;
    }
    else if (GetUTSetup()->GetWaveType() == "Surface")
    {
        fullKey = parameterPrefixSurface;
    }

    fullKey.append(key);
    return dynamic_cast<UTSetup *>(parent())->Get(fullKey);
}

UTSetup* UTGate::GetUTSetup()
{
    return dynamic_cast<UTSetup *>(parent());
}

int UTGate::GetIndex()
{
    return GateIndex;
}

