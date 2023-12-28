#pragma once

#include "QtGlobal"
#include <QElapsedTimer>
#include <QMap>
#include <QObject>
#include <QVector>

struct lua_State;
class QTcpSocket;

class Client : public QObject
{
    Q_OBJECT

public:
    Client(QTcpSocket *s, QObject *parent);
    virtual ~Client();

    void onSocketReadyRead();
    void onDataReady(int channel, int level);
    void SendText(const char *data, size_t len = 0, bool holdLine = false);
    void SendBinary(const char **data, size_t *len, int num);

    int CallProtected(int numArgs, int numRets = 0);
    void doString(const char *str, size_t len);
    void doFile(const char *fileName);
    void callGlobal(const char *name, int numArgs = 0);

    void report();

    static Client *client(lua_State *L);

    static bool RemoveFirstBadConnection();

protected:
    virtual void processData(const char *data, size_t len);

public:
    QTcpSocket *socket = nullptr;
    lua_State *L = nullptr;
    std::string dataBuffer;
    QVector<QMap<int, bool> > dataLevel;
    size_t overrun = 0;
    size_t overrunPerSec = 0;
    size_t sent = 0;
    size_t sentPerSec = 0;
    QElapsedTimer elapsed;

    typedef std::vector<Client *> EngineList;
    static EngineList engines;
};
