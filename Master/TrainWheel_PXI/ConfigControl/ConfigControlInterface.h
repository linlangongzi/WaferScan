#ifndef CONFIGCONTROLINTERFACE_H
#define CONFIGCONTROLINTERFACE_H

#include <QObject>
#include <QJsonValue>

class ConfigControlInterface
{

public:
    ConfigControlInterface();

public:
    virtual void ConnectTo(QObject *obj) = 0;
    virtual void Disconnect(QObject *obj) = 0;

};

#endif // CONFIGCONTROLINTERFACE_H
