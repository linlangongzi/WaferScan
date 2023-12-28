#include "Client.h"
#include "Server.h"
#include <../../app/app_version.h>
#include <QFile>
#include <QHostAddress>
#include <QLoggingCategory>
#include <QTcpSocket>
#include <QThread>
#include <lua.hpp>

static size_t physicalBase = 0x6E000000;

#define SOFTWARE_VER 1.0
static const size_t BUFFER_SIZE = 1024;

using namespace std;

static const char BINARY_SIGNATURE[] = "\033\000ARS";
static const size_t binarySignatureSize = sizeof(BINARY_SIGNATURE) - 1;
static const std::string binarySignature(BINARY_SIGNATURE, binarySignatureSize);
static const size_t binaryHeaderSize = binarySignatureSize + sizeof(quint32);

Client::EngineList Client::engines;

#define L_CLIENT Client *client = Client::client(L);

static Q_LOGGING_CATEGORY(debug, "server.lua")

LUA_API int SocketPrint(lua_State *L) {
    L_CLIENT;
    if (client) {
        size_t len;
        const char *str = luaL_checklstring(L, 1, &len);
        client->SendText(str, len, true);
    }
    return 0;
}

LUA_API int LuaSleep(lua_State *L) {
    lua_Number time = luaL_checknumber(L, 1);
    if (time > 0) {
        QThread::msleep(time);
    }
    return 0;
}

LUA_API int GetNumDevices(lua_State *L) {
    lua_pushinteger(L, Server::deviceCount());
    return 1;
}

template <typename T>
LUA_API int SetInt(lua_State *L) {
    int deviceIndex = 0;
    int offset;
    T value;
    if (lua_gettop(L) < 3) {
        offset = luaL_checkinteger(L, 1);
        value = T(luaL_checkinteger(L, 2));
    } else {
        deviceIndex = luaL_checkinteger(L, 1);
        offset = luaL_checkinteger(L, 2);
        value = T(luaL_checkinteger(L, 3));
    }
    Device *dev = Server::device(deviceIndex);
    if (!dev) {
        return luaL_error(L, "Cannot write invalid device %d", deviceIndex);
    }
    dev->iowrite<T>(offset, value);
    return 0;
}

template <typename T>
LUA_API int GetInt(lua_State *L) {
    int deviceIndex = 0;
    int offset;
    if (lua_gettop(L) < 2) {
        offset = luaL_checkinteger(L, 1);
    } else {
        deviceIndex = luaL_checkinteger(L, 1);
        offset = luaL_checkinteger(L, 2);
    }
    Device *dev = Server::device(deviceIndex);
    if (!dev) {
        return luaL_error(L, "Cannot read invalid device %d", deviceIndex);
    }
    lua_pushinteger(L, dev->ioread<T>(offset));
    return 1;
}

LUA_API int GetPhysical(lua_State *L) {
    qint64 addr = qint64(physicalBase + luaL_checkinteger(L, 1));
    qCDebug(debug, "Reading at %llx", addr);
    QFile file("/dev/mem");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Unbuffered)) {
        return luaL_error(L, "Failed reading physical mem: error opening mem as file");
    }
    if (!file.seek(addr)) {
        return luaL_error(L, "Failed reading physical mem: seek failed");
    }
    quint32 v = 0;
    qint64 sizeRead = file.read(reinterpret_cast<char *>(&v), sizeof(v));
    if (sizeRead < qint64(sizeof(v))) {
        return luaL_error(L, "Failed reading physical mem: read failed, got %d, error %s", sizeRead, qPrintable(file.errorString()));
    }
    lua_pushinteger(L, v);
    return 1;
}

LUA_API int SetPhysical(lua_State *L) {
    qint64 addr = qint64(physicalBase + luaL_checkinteger(L, 1));
    quint32 v = luaL_checkinteger(L, 2);
    qCDebug(debug, "Writing %i at %llx", v, addr);
    QFile file("/dev/mem");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Unbuffered)) {
        return luaL_error(L, "Failed writing physical mem: error opening mem as file");
    }
    if (!file.seek(addr)) {
        return luaL_error(L, "Failed writing physical mem: seek failed");
    }
    if (file.write(reinterpret_cast<char *>(&v), sizeof(v)) < qint64(sizeof(v))) {
        return luaL_error(L, "Failed writing physical mem: write failed");
    }
    return 0;
}

LUA_API int GetVersion(lua_State *L) {
    lua_pushnumber(L, SOFTWARE_VER);
    lua_pushstring(L, Core::Constants::DS_REVISION_STR);
    return 2;
}

LUA_API int GetClientIP(lua_State *L) {
    L_CLIENT;
    lua_pushstring(L, qPrintable(client->socket->peerAddress().toString()));
    return 1;
}

LUA_API int GetHostIP(lua_State *L) {
    L_CLIENT;
    lua_pushstring(L, qPrintable(client->socket->localAddress().toString()));
    return 1;
}

LUA_API int GetClients(lua_State *L) {
    for (Client::EngineList::iterator it = Client::engines.begin(); it != Client::engines.end(); ++it) {
        lua_pushstring(L, qPrintable((*it)->socket->peerAddress().toString()));
    }
    return int(Client::engines.size());
}

LUA_API int CloseSocket(lua_State *L) {
    L_CLIENT;
    client->socket->disconnectFromHost();
    return 1;
}

LUA_API int GetNumChannels(lua_State *L) {
    L_CLIENT;
    lua_pushinteger(L, client->dataLevel.size());
    return 1;
}

LUA_API int SetDataLevel(lua_State *L) {
    int channel = luaL_checkinteger(L, 1);
    int level = int(luaL_checkinteger(L, 2));
    bool enabled = (lua_toboolean(L, 3) != 0);                                  /* 将int转为bool触发C4800警告，使用"!= 0"可以压制该警告 */
    L_CLIENT;
    client->dataLevel[channel][level] = enabled;
    return 0;
}

LUA_API int GetDataLevel(lua_State *L) {
    int channel = luaL_checkinteger(L, 1);
    int level = int(luaL_checkinteger(L, 2));
    L_CLIENT;
    lua_pushboolean(L, client->dataLevel[channel][level]);
    return 1;
}

LUA_API int GetDeviceID(lua_State *L) {
    Q_UNUSED(L);
    return 0;
}

Client::Client(QTcpSocket *s, QObject *parent) :
    QObject(parent),
    socket(s),
    overrun(0),
    overrunPerSec(0),
    sent(0),
    sentPerSec(0)
{
    socket->setParent(this);
    dataLevel.resize(2);
    L = luaL_newstate();
    qCDebug(debug, "Creating Lua state %p", L);
    luaL_openlibs(L);
    lua_pushlightuserdata(L, this);
    lua_setfield(L, LUA_REGISTRYINDEX, "LuaEngine");

    /* _REG.ErrorHandler = debug.traceback */
    lua_getglobal(L, "debug");                                                  /* _G.debug */
    lua_getfield(L, -1, "traceback");                                           /* traceback, debug */
    lua_setfield(L, LUA_REGISTRYINDEX, "ErrorHandler");                         /* debug */
    lua_pop(L, 1);                                                              /* (empty) */

    /* _G._OS = "Linux" */
    lua_pushstring(L, "Linux");
    lua_setglobal(L, "_OS");

    lua_register(L, "SocketPrint", &SocketPrint);
    lua_register(L, "Sleep", &LuaSleep);
    lua_register(L, "GetVersion", &GetVersion);
    lua_register(L, "GetDeviceID", &GetDeviceID);
    lua_register(L, "GetClientIP", &GetClientIP);
    lua_register(L, "GetClients", &GetClients);
    lua_register(L, "GetHostIP", &GetHostIP);
    lua_register(L, "CloseSocket", &CloseSocket);

    lua_register(L, "GetNumChannels", &GetNumChannels);
    lua_register(L, "SetDataLevel", &SetDataLevel);
    lua_register(L, "GetDataLevel", &GetDataLevel);

    lua_register(L, "GetNumDevices", &GetNumDevices);
    lua_register(L, "SetMemoryInt16", &SetInt<quint16>);
    lua_register(L, "SetMemoryInt32", &SetInt<quint32>);
//    lua_register(L, "SetMemoryFloat", &SetFloat<float>);
//    lua_register(L, "SetMemoryDouble", &SetFloat<double>);

    lua_register(L, "GetMemoryInt16", &GetInt<quint16>);
    lua_register(L, "GetMemoryInt32", &GetInt<quint32>);
//    lua_register(L, "GetMemoryFloat", &GetFloat<float>);
//    lua_register(L, "GetMemoryDouble", &GetFloat<double>);

    lua_register(L, "GetPhysical", &GetPhysical);
    lua_register(L, "SetPhysical", &SetPhysical);

    doFile("CeStartup.lua");

    engines.push_back(this);
}

Client::~Client()
{
    engines.erase(std::remove(engines.begin(), engines.end(), this), engines.end());
    qCDebug(debug, "Closing Lua state %p", L);
    lua_close(L);
    delete socket;
}

void Client::onSocketReadyRead()
{
    while (true) {
        char buffer[BUFFER_SIZE];
        qint64 bytesRead = socket->read(buffer, BUFFER_SIZE);
        if (bytesRead <= 0) {
            return;
        }
//        qCDebug(debug) << "Bytes read: " << bytesRead << endl;
        dataBuffer.append(buffer, bytesRead);
        size_t startPos = 0;
        while (true) {
            if (dataBuffer.compare(startPos, binarySignatureSize, binarySignature) == 0) {
               if (dataBuffer.size() < binaryHeaderSize) {
                   break;
               }
               size_t size = *reinterpret_cast<const quint32 *>(dataBuffer.c_str() + startPos + binarySignatureSize);
               if (dataBuffer.size() - binaryHeaderSize < size) {
                   break;
               }
               processData(dataBuffer.data() + startPos + binaryHeaderSize, size);
               startPos += binaryHeaderSize + size;
            } else {
                size_t newLinePos = dataBuffer.find_first_of("\n\003", startPos);
                if (newLinePos == std::string::npos) {
                    break;
                }
                doString(dataBuffer.data() + startPos, newLinePos - startPos);
                startPos = newLinePos + 1;
            }
        }
        if (startPos > 0) {
            dataBuffer = dataBuffer.substr(startPos);
        }
    }
}

void Client::onDataReady(int channel, int level)
{
    lua_pushinteger(L, channel);
    lua_pushinteger(L, level);
    callGlobal("OnDataReady", 2);
}

void Client::SendText(const char *data, size_t len, bool holdLine)
{
    if (data) {
        if (len <= 0) {
            len = strlen(data);
        }
        sent += socket->write(data, len);
        static const char newLine[] = "\n\r";
        if (!holdLine) {
            sent += socket->write(newLine, sizeof(newLine) - 1);
        }
    }
}

void Client::SendBinary(const char **data, size_t *len, int num)
{
    if (data == nullptr || len == nullptr || num == 0) {
        return;
    }
    quint32 size = 0;
    for (int index = 0; index < num; ++index) {
        size += quint32(len[index]);
    }
    sent += socket->write("\n", 1);
    sent += socket->write(BINARY_SIGNATURE, binarySignatureSize);
    sent += socket->write(reinterpret_cast<const char *>(&size), sizeof(size));
    for (int index = 0; index < num; ++index) {
        sent += socket->write(data[index], len[index]);
    }
}

int Client::CallProtected(int numArgs, int numRets)
{                                                                               /* (-1): func,  (-2)~(-numArgs-1): args */
    lua_getfield(L, LUA_REGISTRYINDEX, "ErrorHandler");                         /* (-1): err,   (-2): func, (-3)~(-numArgs-2) */
    lua_insert(L, 1);                                                           /* (-1): func,  (-2)~(-numArgs-1): args, (-numArgs-2): err */
    int ret = lua_pcall(L, numArgs, numRets, -numArgs - 2);                     /* (-1)~(-numRets): rets,   (-numRets-1): err */
    if (ret) {
        SendText(lua_tostring(L, -1));
        lua_pop(L, 1);
        numRets = 0;
    }
    lua_remove(L, 1);
    return ret;
}

void Client::doString(const char *str, size_t len)
{
    int ret = luaL_loadbuffer(L, str, len, nullptr);
    if (ret) {
        SendText(lua_tostring(L, -1));
        return lua_pop(L, 1);
    }
    CallProtected(0, 0);
}

void Client::doFile(const char *fileName)
{
    int ret = luaL_loadfile(L, fileName);
    if (ret) {
        SendText(lua_tostring(L, -1));
        return lua_pop(L, 1);
    }
    CallProtected(0, 0);
}

void Client::callGlobal(const char *name, int numArgs)
{
    lua_getglobal(L, name);
    if (lua_isfunction(L, -1)) {
        lua_insert(L, -numArgs - 1);
        CallProtected(numArgs, 0);
    } else {
        lua_pop(L, numArgs + 1);
    }
}

void Client::report()
{
    overrunPerSec = overrun;
    overrun = 0;
    sentPerSec = sent;
    sent = 0;
    elapsed.start();
    qCDebug(debug, "socket %d, Lua state %p, peer %s", socket->socketDescriptor(), L, qPrintable(socket->peerAddress().toString()));
    qCDebug(debug, "  Sent %d bytes/sec; Overrun %u bytes/sec; Send buffer %lld bytes.", sentPerSec, overrunPerSec, socket->bytesToWrite());
}

Client *Client::client(lua_State *L)
{
    if (L == nullptr) {
        return nullptr;
    }
    lua_getfield(L, LUA_REGISTRYINDEX, "LuaEngine");
    Client *client = reinterpret_cast<Client *>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    return client;
}

bool Client::RemoveFirstBadConnection()
{
    for (Client *client: engines) {
        if (client->socket->state() == QTcpSocket::UnconnectedState) {
            qCDebug(debug, "Client disconnected (%d)", client->socket->socketDescriptor());
            delete client;
            return true;
        }
    }
    return false;
}

void Client::processData(const char *data, size_t len)
{
    (void)data;
    (void)len;
}
