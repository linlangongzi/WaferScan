#pragma once

#include "Device.h"

class Emulator : public QObject
{
    Q_OBJECT

public:
    Emulator(int size, QObject *parent = nullptr) :
        QObject(parent),
        buffer(size, 0)
    {}

    void FillBuffer(Device::Section &section)
    {
        *section.bufferIndex = 1;
        *section.flag = 1;
    }

    void Reset(Device::Section &section)
    {
        *section.flag = 0;
    }

    QByteArray buffer;
};
