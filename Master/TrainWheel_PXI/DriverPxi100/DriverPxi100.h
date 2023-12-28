#ifndef DRIVERPXI100_H
#define DRIVERPXI100_H

#include "DeviceDriver/DeviceDriver.h"

class DriverPxi100 : public DeviceDriver
{
    Q_OBJECT
private:
    enum eReadWrite
    {
        READ = 0,
        WRITE
    };

public:
    explicit DriverPxi100(QObject *parent = 0);

public:
    virtual bool SetInt16(int address, quint16 value, int cardIdx);
    virtual bool SetInt32(int address, quint32 value, int cardIdx);
    virtual quint16 GetInt16(int address, int cardIdx);
    virtual quint32 GetInt32(int address, int cardIdx);

private:
    quint16 ReadMfcInt16(int address);
    quint32 ReadMfcInt32(int address);
    void WriteMfcInt16(int address, quint16 value);
    void WriteMfcInt32(int address, quint32 value);
    quint16 ReadAecInt16(int address, int cardIdx);
    quint32 ReadAecInt32(int address, int cardIdx);
    void WriteAecInt16(int address, quint16 value, int cardIdx);
    void WriteAecInt32(int address, quint32 value, int cardIdx);

signals:

public slots:
};

#endif // DRIVERPXI1000_H
