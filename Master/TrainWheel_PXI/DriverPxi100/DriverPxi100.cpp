#include "DriverPxi100.h"
#include "p9054_lib.h"
#include "plx_lib_x.h"
#include "Platform/Platform.h"
#include "CardWork/AECWork.h"
#include "CardWork/MFCWork.h"
#include "CardWork/CardDataDefine.h"

DriverPxi100::DriverPxi100(QObject *parent) :
    DeviceDriver(parent)
{

}

bool DriverPxi100::SetInt16(int address, quint16 value, int cardIdx)
{
    if(cardIdx == 0)
    {
        WriteMfcInt16(address, value);
    }
    else if(cardIdx > 1)
    {
        WriteAecInt16(address, value, cardIdx);
    }
    else
    {
        return false;
    }

    return true;
}

bool DriverPxi100::SetInt32(int address, quint32 value, int cardIdx)
{
    if(cardIdx == 0)
    {
        WriteMfcInt32(address, value);
    }
    else if(cardIdx > 1)
    {
        WriteAecInt32(address, value, cardIdx);
    }
    else
    {
        return false;
    }

    return true;
}

quint16 DriverPxi100::GetInt16(int address, int cardIdx)
{
    if(cardIdx == 0)
    {
        return ReadMfcInt16(address);
    }
    else if(cardIdx > 1)
    {
        return ReadAecInt16(address, cardIdx);
    }
    else
    {
        return 0xFF;
    }
}

quint32 DriverPxi100::GetInt32(int address, int cardIdx)
{
    if(cardIdx == 0)
    {
        return ReadMfcInt32(address);
    }
    else if(cardIdx > 1)
    {
        return ReadAecInt32(address, cardIdx);
    }
    else
    {
        return 0xFFFF;
    }
}

quint16 DriverPxi100::ReadMfcInt16(int address)
{
    MFCWork* pMfcWork = platform()->GetMfcWork<MFCWork>();
    if(pMfcWork)
    {
        return P9054_ReadWord(pMfcWork->GetCardParam()->hDev, P9054_ADDR_SPACE0, address);
    }
    else
    {
        return 0xFF;
    }
}

quint32 DriverPxi100::ReadMfcInt32(int address)
{
    MFCWork* pMfcWork = platform()->GetMfcWork<MFCWork>();
    if(pMfcWork)
    {
        return P9054_ReadDWord(pMfcWork->GetCardParam()->hDev, P9054_ADDR_SPACE0, address);
    }
    else
    {
        return 0xFFFF;
    }
}

void DriverPxi100::WriteMfcInt16(int address, quint16 value)
{
    MFCWork* pMfcWork = platform()->GetMfcWork<MFCWork>();
    if(pMfcWork)
    {
       P9054_WriteWord(pMfcWork->GetCardParam()->hDev, P9054_ADDR_SPACE0, address, value);
    }
}

void DriverPxi100::WriteMfcInt32(int address, quint32 value)
{
    MFCWork* pMfcWork = platform()->GetMfcWork<MFCWork>();
    if(pMfcWork)
    {
       P9054_WriteDWord(pMfcWork->GetCardParam()->hDev, P9054_ADDR_SPACE0, address, value);
    }
}

quint16 DriverPxi100::ReadAecInt16(int address, int cardIdx)
{
    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();
    sharedAecWork aecWork =  pAecWorkVec->value(cardIdx - 1);
    if(aecWork)
    {
        return P9054_ReadWord(aecWork->GetCardParam()->hDev, P9054_ADDR_SPACE0, address);
    }
    else
    {
        return 0xFF;
    }
}

quint32 DriverPxi100::ReadAecInt32(int address, int cardIdx)
{
    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();
    sharedAecWork aecWork =  pAecWorkVec->value(cardIdx - 1);
    if(aecWork)
    {
        return P9054_ReadDWord(aecWork->GetCardParam()->hDev, P9054_ADDR_SPACE0, address);
    }
    else
    {
        return 0xFFFF;
    }
}

void DriverPxi100::WriteAecInt16(int address, quint16 value, int cardIdx)
{
    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();
    sharedAecWork aecWork =  pAecWorkVec->value(cardIdx - 1);
    if(aecWork)
    {
        P9054_WriteWord(aecWork->GetCardParam()->hDev, P9054_ADDR_SPACE0, address, value);
    }
}

void DriverPxi100::WriteAecInt32(int address, quint32 value, int cardIdx)
{
    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();
    sharedAecWork aecWork =  pAecWorkVec->value(cardIdx - 1);
    if(aecWork)
    {
        P9054_WriteDWord(aecWork->GetCardParam()->hDev, P9054_ADDR_SPACE0, address, value);
    }
}
