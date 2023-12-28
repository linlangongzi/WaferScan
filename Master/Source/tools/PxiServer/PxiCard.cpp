#include "PxiCard.h"
#include "p9054_lib.h"
#include <QLoggingCategory>
#include <QThread>

static Q_LOGGING_CATEGORY(debug, "pxi.card")

PxiCard::PxiCard(P9054_HANDLE handle, QObject *parent) :
    QObject(parent),
    handle(handle),
    addressSpace(P9054_ADDR_SPACE0)
{

}

PxiCard::~PxiCard()
{
    if (handle) {
        P9054_Close(handle);
    }
}

void PxiCard::dumpInfo() const
{
    QByteArray name(handle->cardReg.cName, sizeof(handle->cardReg.cName));
    QByteArray description(handle->cardReg.cDescription, sizeof(handle->cardReg.cDescription));
    DWORD serial = P9054_ReadPCIReg(handle, 0x2E);
    qCInfo(debug, "Device %d - handle %p", index, handle->hWD);
    qCInfo(debug, "\tName: %s", name.constData());
    qCInfo(debug, "\tDesc: %s", description.constData());
    qCInfo(debug, "\tBus %ld, Slot %ld, Function %ld", handle->pciSlot.dwBus, handle->pciSlot.dwSlot, handle->pciSlot.dwFunction);
    qCInfo(debug, "\tSpace 0x%lX, Serial 0x%lX", handle->addrSpace, serial);
    for (int bar = 0; bar < AD_PCI_BARS; ++bar) {
        auto &info = handle->addrDesc[bar];
        qCInfo(debug, "\tBar %d: LocalBase 0x%lX, Mask 0x%lX, Bytes 0x%lX, Addr 0x%lX, AddrDirect 0x%lX", bar, info.dwLocalBase, info.dwMask, info.dwBytes, info.dwAddr, info.dwAddrDirect);
    }
}

bool PxiCard::ramTest(quint32 startAddress, quint32 size, int count)
{
    for (int index = 0; index < count; ++index) {
        qintptr seed = qintptr(QThread::currentThreadId()) + index;

        qsrand(seed);
        const quint32 end = startAddress + size;
        for (quint32 address = startAddress; address < end; address += 2) {
            write(address, qrand());
        }
        qsrand(seed);
        for (quint32 address = startAddress; address < end; address += 2) {
            quint16 readValue = read(address);
            quint16 expected = qrand();
            if (readValue != expected) {
                qCCritical(debug, "RAM test failed at %x (offset %x), expecting %x, got %x", address, address - startAddress, expected, readValue);
                return false;
            }
        }
    }
    return true;
}

quint16 PxiCard::read(quint32 address) const
{
    return P9054_ReadWord(handle, P9054_ADDR(addressSpace), address);
}

void PxiCard::write(quint32 address, quint16 data)
{
    P9054_WriteWord(handle, P9054_ADDR(addressSpace), address, data);
}
