#ifndef _P9054_LIB_H_
#define _P9054_LIB_H_

#if !defined(UNIX) && (defined(LINUX) || defined(SOLARIS))
    #define UNIX
#endif

#if defined(__KERNEL__)
    #include "../../../include/kdstdlib.h"
#endif 
#include "windrvr.h"
#include "pci_regs.h"
#include "bits.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PLX_DLL_EXPORT    
#define PLX_DLL_API            _declspec (dllexport)
#else
#define PLX_DLL_API            _declspec (dllimport)
#endif

// PLX register definitions 
enum {
    P9054_LAS0RR     = 0x00,
    P9054_LAS0BA     = 0x04,
    P9054_EEPROM_PROT_AREA = 0x0C,
    P9054_EROMRR     = 0x10,
    P9054_EROMBA     = 0x14,
    P9054_LBRD0      = 0x18,
    P9054_MBOX0      = 0x40,
    P9054_MBOX1      = 0x44,
    P9054_MBOX2      = 0x48,
    P9054_MBOX3      = 0x4c,
    P9054_MBOX4      = 0x50,
    P9054_MBOX5      = 0x54,
    P9054_MBOX6      = 0x58,
    P9054_MBOX7      = 0x5c,
    P9054_P2LDBELL   = 0x60,
    P9054_L2PDBELL   = 0x64,
    P9054_INTCSR     = 0x68,
    P9054_CNTRL      = 0x6c,
    P9054_DMAMODE    = 0x80,
    P9054_DMAPADR    = 0x84,
    P9054_DMALADR    = 0x88,
    P9054_DMASIZ     = 0x8c,
    P9054_DMADPR     = 0x90,
    P9054_DMACSR     = 0xA8
};

// PLX specific PCI configuration registers
enum {
    P9054_VPD_ADDR   = 0x4E,
    P9054_VPD_DATA   = 0x50,
};

typedef enum
{
    P9054_DMA_CHANNEL_0 = 0,
    P9054_DMA_CHANNEL_1 = 1
} P9054_DMA_CHANNEL;

typedef enum
{
    P9054_MODE_BYTE   = 0,
    P9054_MODE_WORD   = 1,
    P9054_MODE_DWORD  = 2
} P9054_MODE;

typedef enum
{
    P9054_ADDR_REG     = AD_PCI_BAR0,
    P9054_ADDR_REG_IO  = AD_PCI_BAR1,
    P9054_ADDR_SPACE0  = AD_PCI_BAR2,
    P9054_ADDR_SPACE1  = AD_PCI_BAR3,
    P9054_ADDR_SPACE2  = AD_PCI_BAR4,
    P9054_ADDR_SPACE3  = AD_PCI_BAR5,
    P9054_ADDR_EPROM   = AD_PCI_BAR_EPROM
} P9054_ADDR;

enum { P9054_RANGE_REG = 0x00000080 };

typedef struct P9054_DMA_STRUCT *P9054_DMA_HANDLE;

typedef struct 
{
	DWORD dwLocalBase;
	DWORD dwMask;
	DWORD dwBytes;
	DWORD dwAddr;
	DWORD dwAddrDirect;
	BOOL  fIsMemory;
} P9054_ADDR_DESC;

typedef struct P9054_STRUCT *P9054_HANDLE;

typedef struct
{
	DWORD dwCounter;   // number of interrupts received
	DWORD dwLost;      // number of interrupts not yet dealt with
	BOOL fStopped;     // was interrupt disabled during wait
	DWORD dwStatusReg; // value of status register when interrupt occured
} P9054_INT_RESULT;

typedef void (WINAPI *P9054_INT_HANDLER)( P9054_HANDLE hPlx, P9054_INT_RESULT *intResult);

typedef struct
{
	WD_INTERRUPT Int;
	HANDLE hThread;
	WD_TRANSFER Trans[2];
	P9054_INT_HANDLER funcIntHandler;
} P9054_INTERRUPT;

typedef struct P9054_STRUCT
{
	HANDLE hWD;
	WD_CARD cardLock;
	WD_PCI_SLOT pciSlot;
	WD_CARD_REGISTER cardReg;
	P9054_ADDR_DESC addrDesc[AD_PCI_BARS];
	DWORD  addrSpace;
	BOOL   fUseInt;
	P9054_INTERRUPT Int;
} P9054_STRUCT;




typedef void (WINAPI *P9054_INT_HANDLER)( P9054_HANDLE hPlx, P9054_INT_RESULT *intResult);

// options for PLX_Open
enum { P9054_OPEN_USE_INT =   0x1 };

PLX_DLL_API DWORD P9054_CountCards (DWORD dwVendorID, DWORD dwDeviceID);
PLX_DLL_API BOOL P9054_Open (P9054_HANDLE *phPlx, DWORD dwVendorID, DWORD dwDeviceID, DWORD nCardNum, DWORD dwOptions);
PLX_DLL_API void P9054_Close (P9054_HANDLE hPlx);
BOOL P9054_IsAddrSpaceActive(P9054_HANDLE hPlx, P9054_ADDR addrSpace);
void P9054_GetPciSlot(P9054_HANDLE hPlx, WD_PCI_SLOT *pPciSlot);

void P9054_ReadWriteBlockLocal (P9054_HANDLE hPlx, DWORD dwLocalAddr, PVOID buf, 
                    DWORD dwBytes, BOOL fIsRead, P9054_MODE mode);
void P9054_ReadBlockLocal (P9054_HANDLE hPlx, DWORD dwLocalAddr, PVOID buf, DWORD dwBytes, P9054_MODE mode);
void P9054_WriteBlockLocal (P9054_HANDLE hPlx, DWORD dwLocalAddr, PVOID buf, DWORD dwBytes, P9054_MODE mode);
BYTE P9054_ReadByteLocal (P9054_HANDLE hPlx, DWORD dwLocalAddr);
void P9054_WriteByteLocal (P9054_HANDLE hPlx, DWORD dwLocalAddr, BYTE data);
WORD P9054_ReadWordLocal (P9054_HANDLE hPlx, DWORD dwLocalAddr);
void P9054_WriteWordLocal (P9054_HANDLE hPlx, DWORD dwLocalAddr, WORD data);
DWORD P9054_ReadDWordLocal (P9054_HANDLE hPlx, DWORD dwLocalAddr);
void P9054_WriteDWordLocal (P9054_HANDLE hPlx, DWORD dwLocalAddr, DWORD data);

void P9054_ReadWriteBlock (P9054_HANDLE hPlx, DWORD dwOffset, PVOID buf, 
                    DWORD dwBytes, BOOL fIsRead, P9054_ADDR addrSpace, P9054_MODE mode);
BYTE P9054_ReadByte (P9054_HANDLE hPlx, P9054_ADDR addrSpace, DWORD dwOffset);
void P9054_WriteByte (P9054_HANDLE hPlx, P9054_ADDR addrSpace, DWORD dwOffset, BYTE data);
PLX_DLL_API WORD P9054_ReadWord (P9054_HANDLE hPlx, P9054_ADDR addrSpace, DWORD dwOffset);
PLX_DLL_API void P9054_WriteWord (P9054_HANDLE hPlx, P9054_ADDR addrSpace, DWORD dwOffset, WORD data);
PLX_DLL_API DWORD P9054_ReadDWord (P9054_HANDLE hPlx, P9054_ADDR addrSpace, DWORD dwOffset);
PLX_DLL_API void P9054_WriteDWord (P9054_HANDLE hPlx, P9054_ADDR addrSpace, DWORD dwOffset, DWORD data);
void P9054_ReadBlock (P9054_HANDLE hPlx, DWORD dwOffset, PVOID buf, 
                    DWORD dwBytes, P9054_ADDR addrSpace, P9054_MODE mode);
void P9054_WriteBlock (P9054_HANDLE hPlx, DWORD dwOffset, PVOID buf, 
                     DWORD dwBytes, P9054_ADDR addrSpace, P9054_MODE mode);

// interrupt functions
BOOL P9054_IntIsEnabled (P9054_HANDLE hPlx);
BOOL P9054_IntEnable (P9054_HANDLE hPlx, P9054_INT_HANDLER funcIntHandler);
void P9054_IntDisable (P9054_HANDLE hPlx);

// access registers
DWORD P9054_ReadReg (P9054_HANDLE hPlx, DWORD dwReg);
void P9054_WriteReg (P9054_HANDLE hPlx, DWORD dwReg, DWORD dwData);

// access PCI configuration registers
PLX_DLL_API DWORD P9054_ReadPCIReg(P9054_HANDLE hPlx, DWORD dwReg);
void P9054_WritePCIReg(P9054_HANDLE hPlx, DWORD dwReg, DWORD dwData);

// Start DMA to/from card.
// fIsRead - TRUE: read from card to buffer.  FALSE: write from buffer to card
// dwBytes - number of bytes to transfer (must be a multiple of 4)
// mode - local bus width.
// dwLocalAddr - local address on card to write to / read from
// channel - uses channel 0 or 1 of the 9054
// buf - the buffer to transfer
P9054_DMA_HANDLE P9054_DMAOpen (P9054_HANDLE hPlx, DWORD dwLocalAddr, PVOID buf, 
    DWORD dwBytes, BOOL fIsRead, P9054_MODE mode, P9054_DMA_CHANNEL dmaChannel);
void P9054_DMAClose (P9054_HANDLE hPlx, P9054_DMA_HANDLE hDma);
void P9054_DMAStart (P9054_HANDLE hPlx, P9054_DMA_HANDLE hDma, BOOL fBlocking);
BOOL P9054_DMAIsDone (P9054_HANDLE hPlx, P9054_DMA_HANDLE hDma);
PLX_DLL_API BOOL P9054_DMAReadWriteBlock (P9054_HANDLE hPlx, DWORD dwLocalAddr, PVOID buf, 
     DWORD dwBytes, BOOL fIsRead, P9054_MODE mode, P9054_DMA_CHANNEL dmaChannel);

BOOL P9054_EEPROMReadWord(P9054_HANDLE hPlx, DWORD dwOffset, PWORD pwData);
BOOL P9054_EEPROMWriteWord(P9054_HANDLE hPlx, DWORD dwOffset, WORD wData);
BOOL P9054_EEPROMReadDWord(P9054_HANDLE hPlx, DWORD dwOffset, PDWORD pdwData);
BOOL P9054_EEPROMWriteDWord(P9054_HANDLE hPlx, DWORD dwOffset, DWORD dwData);
BOOL P9054_EEPROMValid(P9054_HANDLE hPlx);

//自己编写的中断处理函数
//主要是设置中断标志位
void qkl_INTEnable(P9054_HANDLE hPlx);
void qkl_INTDisable(P9054_HANDLE hPlx);


void WINAPI qkl_IntHandler1(P9054_HANDLE hPlx,P9054_INT_RESULT * IntResult);
void WINAPI qkl_IntHandler2(P9054_HANDLE hPlx,P9054_INT_RESULT * IntResult);
void WINAPI qkl_IntHandler3(P9054_HANDLE hPlx,P9054_INT_RESULT * IntResult);
void WINAPI qkl_IntHandler4(P9054_HANDLE hPlx,P9054_INT_RESULT * IntResult);


extern int nINTFlag1;
extern int nINTFlag2;
extern int nINTFlag3;
extern int nINTFlag4;
extern int nDMAFlag;
extern int nCHXDAData;


// this string is set to an error message, if one occurs
extern CHAR P9054_ErrorString[];


#ifdef __cplusplus
}
#endif

#endif
