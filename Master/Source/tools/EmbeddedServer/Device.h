#pragma once

#include <QElapsedTimer>
#include <QObject>
#include <QVector>

class Emulator;

class Device : public QObject
{
    Q_OBJECT

public:
    Device(qint64 baseAddr, QObject *parent = nullptr);
    ~Device();

    static QList<Device *> enumerate();
    void updateMonitor(unsigned int channelIndex);
    bool checkLevel(int channelIndex, int level);
    uchar *mapPhysical(qint64 physicalAddr, qint64 size);
    void reset();

    template <typename T>
    void iowrite(int offset, T value)
    {
        volatile T *addr = reinterpret_cast<T *>(base + offset);
        *addr = value;
    }

    template <typename T>
    T ioread(int offset)
    {
        volatile T *addr = reinterpret_cast<T *>(base + offset);
        return *addr;
    }

    uchar *base = nullptr;

    struct FeatureData
    {
        quint16 wave[512];
        quint16 nXLow16;
        quint16 nXHigh16;
        quint16 nYLow16;
        quint16 nYHigh16;
        quint16 nZLow16;
        quint16 nZHigh16;
        quint16 nW1Low16;
        quint16 nW1High16;
        quint16 nALow16;
        quint16 nAHigh16;
        quint16 nBLow16;
        quint16 nBHigh16;
        quint16 nW2Low16;
        quint16 nW2High16;
        quint16 nGate1SyncTime;
        quint16 nGate1MPTime;
        quint16 nGate1MPAmp;
        quint16 nGate2MPTime;
        quint16 nGate2MPAmp;
        quint16 nGate2FPFirstTime;
        quint16 nGate2FPTime;
        quint16 nGate2FPAmp;
        quint16 nGate3MPTime;
        quint16 nGate3MPAmp;
        quint16 nGate3FPFirstTime;
        quint16 nGate3FPTime;
        quint16 nGate3FPAmp;
        quint16 nGate4MPTime;
        quint16 nGate4MPAmp;
        quint16 nGate4FPFirstTime;
        quint16 nGate4FPTime;
        quint16 nGate4FPAmp;
    };

    FeatureData monitor;

    struct Section
    {
        Section()
        {
            elapsed.start();
        }
        size_t sectionSize = 0;
        int sectionCount = 1;
        volatile quint16 *flag;
        volatile quint16 *bufferIndex;
        volatile quint16 *reset;
        volatile quint16 *entry[3];
        typedef void (Device::*UpdateFunction)(unsigned int);
        UpdateFunction update;
        QElapsedTimer elapsed;
        qint64 updateInterval= 0;
    };

    typedef QVector<Section> Channel;
    typedef QVector<Channel> ChannelList;
    ChannelList channels;
    bool hardwareValid = false;

    friend class Emulator;
};
