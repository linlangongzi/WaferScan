#ifndef _PLX_LIB_H_
#define _PLX_LIB_H_

/************************************************************************
*  File: plx_lib.h
*
*  Library for accessing PLX devices.
*  The code accesses hardware using WinDriver's WDC library.
*  Code was generated by DriverWizard v7.01.
*
*  Copyright (c) 2003 - 2005 Jungo Ltd.  http://www.jungo.com
*************************************************************************/
#define WD_DRIVER_NAME_CHANGE

#include "wdc_lib.h"
#include "mpb_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PLX_DLL_EXPORT
    #ifdef __GNUC__
        #define PLX_DLL_API __attribute__((dllexport))
    #else
        #define PLX_DLL_API            _declspec (dllexport)
    #endif
#else
    #ifdef __GNUC__
        #define PLX_DLL_API __attribute__((dllexport))
    #else
        #define PLX_DLL_API            _declspec (dllimport)
    #endif
#endif


/* --------------------------------------------------
    PLX configuration registers information
   -------------------------------------------------- */
/* Configuration registers information array */


/*************************************************************
  General definitions
 *************************************************************/
/*************************************************************
  General definitions
 *************************************************************/
/* PLX configuration registers */
enum PLX_CFG_REGS {
    PLX_PMCAPID = 0x40,   /* Power Management Capability ID */
    PLX_PMNEXT = 0x41,    /* Power Management Next Capability Pointer */
    PLX_PMCAP = 0x42,     /* Power Management Capabilities Register */
    PLX_PMCSR = 0x44,     /* Power Management Control/Status Register */
    PLX_HS_CAPID = 0x48,  /* Hot Swap Capability ID */
    PLX_HS_NEXT = 0x49,   /* Hot Swap Next Capability Pointer */
    PLX_HS_CSR = 0x4A,    /* Hot Swap Control/Status Register */
    PLX_VPD_CAPID = 0x4C, /* PCI Vital Product Data Capability ID */
    PLX_VPD_NEXT = 0x4D,  /* PCI Vital Product Data Next Capability Pointer */
    PLX_VPD_ADDR = 0x4E,  /* PCI Vital Product Data Address */
    PLX_VPD_DATA = 0x50   /* PCI VPD Data */
};

/* Run-time registers of PLX master devices (9054, 9056, 9080, 9656) */
enum PLX_M_REGS {
    /* Local configuration registers */
    PLX_M_LAS0RR = 0x00,    /* Local Addr Space 0 Range for PCI-to-Local Bus */
    PLX_M_LAS0BA = 0x04,    /* Local BAR (Remap) for PCI-to-Local Addr Space 0 */
    PLX_M_MARBR = 0x08,     /* Mode/DMA Arbitration */
    PLX_M_BIGEND = 0x0C,    /* Big/Little Endian Descriptor */
    PLX_M_LMISC = 0x0D,     /* Local Miscellananeous Control */
    PLX_M_PROT_AREA = 0x0E, /* Serial EEPROM Write-Protected Addr Boundary */
    PLX_M_EROMRR = 0x10,    /* Expansion ROM Range */
    PLX_M_EROMBA = 0x14,    /* EROM Local BAR (Remap) & BREQ0 Control */
    PLX_M_LBRD0 = 0x18,     /* Local Addr Space 0 Bus Region Descriptor */
    PLX_M_DMRR = 0x1C,      /* Local Range for PCI initiatior-to-PCI */
    PLX_M_DMLBAM = 0x20,    /* Local Bus Addr for PCI Initiatior-to-PCI Mem */
    PLX_M_DMLBAI = 0x24,    /* Local BAR for PCI Initiatior-to-PCI I/O */
    PLX_M_DMPBAM = 0x28,    /* PCI BAR (Remap) for Initiatior-to-PCI Mem */
    PLX_M_DMCFGA = 0x2C,    /* PCI Config Addr for PCI Initiatior-to-PCI I/O */

    PLX_M_LAS1RR = 0xF0,    /* Local Addr Space 1 Range for PCI-to-Local Bus */
    PLX_M_LAS1BA = 0xF4,    /* Local Addr Space 1 Local BAR (Remap) */
    PLX_M_LBRD1 = 0xF8,     /* Local Addr Space 1 Bus Region Descriptor */
    PLX_M_DMDAC = 0xFC,     /* PCI Initiatior PCI Dual Address Cycle */
    PLX_M_PCIARB = 0x100,   /* PCI Arbiter Control*/
    PLX_M_PABTADR = 0x104,  /* PCI Abort Address */

    /* mailbox, doorbell, interrupt status, control, id registers */
    PLX_M_MBOX0 = 0x40,     /* Mailbox 0 */
    PLX_M_MBOX1 = 0x44,     /* Mailbox 1 */
    PLX_M_MBOX2 = 0x48,     /* Mailbox 2 */
    PLX_M_MBOX3 = 0x4C,     /* Mailbox 3 */
    PLX_M_MBOX4 = 0x50,     /* Mailbox 4 */
    PLX_M_MBOX5 = 0x54,     /* Mailbox 5 */
    PLX_M_MBOX6 = 0x58,     /* Mailbox 6 */
    PLX_M_MBOX7 = 0x5C,     /* Mailbox 7 */
    PLX_M_P2LDBELL = 0x60,  /* PCI-to-Local Doorbell */
    PLX_M_L2PDBELL = 0x64,  /* Local-to-PCI Doorbell */
    PLX_M_INTCSR = 0x68,    /* INTCSR - Interrupt Control/Status */
    PLX_M_CNTRL = 0x6C,     /* Serial EEPROM/User I/O/Init Ctr & PCI Cmd Codes */
    PLX_M_PCIHIDR = 0x70,   /* PCI Hardcoded Configuration ID */
    PLX_M_PCIHREV = 0x74,   /* PCI Hardcoded Revision ID */
    PLX_M_MBOX0_I2O = 0x78, /* Mailbox 0 - I2O enabled */
    PLX_M_MBOX1_I2O = 0x7C, /* Mailbox 1 - I2O enabled */

    /* DMA registers */
    PLX_M_DMAMODE0 = 0x80,  /* DMA Channel 0 Mode */
    PLX_M_DMAPADR0 = 0x84,  /* DMA Channel 0 PCI Address */
    PLX_M_DMALADR0 = 0x88,  /* DMA Channel 0 Local Address */
    PLX_M_DMASIZ0 = 0x8C,   /* DMA Channel 0 Transfer Size (bytes) */
    PLX_M_DMADPR0 = 0x90,   /* DMA Channel 0 Descriptor Pointer */
    PLX_M_DMAMODE1 = 0x94,  /* DMA Channel 1 Mode */
    PLX_M_DMAPADR1 = 0x98,  /* DMA Channel 1 PCI Address */
    PLX_M_DMALADR1 = 0x9C,  /* DMA Channel 1 Local Address */
    PLX_M_DMASIZ1 = 0xA0,   /* DMA Channel 1 Transfer Size (bytes) */
    PLX_M_DMADPR1 = 0xA4,   /* DMA Channel 1 Descriptor Pointer */
    PLX_M_DMACSR0 = 0xA8,   /* DMA Channel 0 Command/Status */
    PLX_M_DMACSR1 = 0xA9,   /* DMA Channel 1 Command/Status */
    PLX_M_DMAARB = 0xAC,    /* DMA Arbitration */
    PLX_M_DMATHR = 0xB0,    /* DMA Threshold (Channel 0 only) */
    PLX_M_DMADAC0 = 0xB4,   /* DMA 0 PCI Dual Address Cycle Address */
    PLX_M_DMADAC1 = 0xB8,   /* DMA 1 PCI Dual Address Cycle Address */

    /* Messaging queue (I20) registers */
    PLX_M_OPQIS = 0x30,     /* Outbound Post Queue Interrupt Status */
    PLX_M_OPQIM = 0x34,     /* Outbound Post Queue Interrupt Mask */
    PLX_M_IQP = 0x40,       /* Inbound Queue Post */
    PLX_M_OQP = 0x44,       /* Outbound Queue Post */
    PLX_M_MQCR = 0xC0,      /* Messaging Queue Configuration */
    PLX_M_QBAR = 0xC4,      /* Queue Base Address */
    PLX_M_IFHPR = 0xC8,     /* Inbound Free Head Pointer */
    PLX_M_IFTPR = 0xCC,     /* Inbound Free Tail Pointer */
    PLX_M_IPHPR = 0xD0,     /* Inbound Post Head Pointer */
    PLX_M_IPTPR = 0xD4,     /* Inbound Post Tail Pointer */
    PLX_M_OFHPR = 0xD8,     /* Outbound Free Head Pointer */
    PLX_M_OFTPR = 0xDC,     /* Outbound Free Tail Pointer */
    PLX_M_OPHPR = 0xE0,     /* Outbound Post Head Pointer */
    PLX_M_OPTPR = 0xE4,     /* Outbound Post Tail Pointer */
    PLX_M_QSR = 0xE8        /* Queue Status/Control */
};

/* Run-time registers of PLX target devices (9030, 9050, 9052) */
enum PLX_T_REGS {
    /* Local configuration registers */
    PLX_T_LAS0RR = 0x00,      /* Local Addr Space 0 Range */
    PLX_T_LAS1RR = 0x04,      /* Local Addr Space 1 Range */
    PLX_T_LAS2RR = 0x08,      /* Local Addr Space 2 Range */
    PLX_T_LAS3RR = 0x0C,      /* Local Addr Space 3 Range */
    PLX_T_EROMRR = 0x10,      /* Expansion ROM Range */
    PLX_T_LAS0BA = 0x14,      /* Local Addr Space 0 Local BAR (Remap) */
    PLX_T_LAS1BA = 0x18,      /* Local Addr Space 1 Local BAR (Remap) */
    PLX_T_LAS2BA = 0x1C,      /* Local Addr Space 2 Local BAR (Remap) */
    PLX_T_LAS3BA = 0x20,      /* Local Addr Space 3 Local BAR (Remap) */
    PLX_T_EROMBA = 0x24,      /* Expansion ROM Local BAR (Remap) */
    PLX_T_LAS0BRD = 0x28,     /* Local Addr Space 0 Bus Region Descriptors */
    PLX_T_LAS1BRD = 0x2C,     /* Local Addr Space 1 Bus Region Descriptors */
    PLX_T_LAS2BRD = 0x30,     /* Local Addr Space 2 Bus Region Descriptors */
    PLX_T_LAS3BRD = 0x34,     /* Local Addr Space 3 Bus Region Descriptors */
    PLX_T_EROMBRD = 0x38,     /* Expansion ROM Bus Region Descriptors */

    /* Chip select registers */
    PLX_T_CS0BASE = 0x3C,     /* Chip Select 0 Base Address */
    PLX_T_CS1BASE = 0x40,     /* Chip Select 1 Base Address */
    PLX_T_CS2BASE = 0x44,     /* Chip Select 2 Base Address */
    PLX_T_CS3BASE = 0x48,     /* Chip Select 3 Base Address */

    /* Control registers */
    PLX_T_INTCSR = 0x4C,      /* Interrupt Control/Status (16 bit)*/
    PLX_T_PROT_AREA = 0x4E,   /* Serial EEPROM Write-Protected Addr Boundary (16 bit)*/
    PLX_T_CNTRL = 0x50,       /* PCI Target Response; Serial EEPROM; Init Ctr */
    PLX_T_GPIOC = 0x54,       /* General Purpose I/O Control */
    PLX_T_PMDATASEL = 0x70,   /* Hidden 1 Power Management Data Select */
    PLX_T_PMDATASCALE = 0x74  /* Hidden 2 Power Management Data Scale */
};

/* DMA channels */
typedef enum {
    PLX_DMA_CHANNEL_0 = 0,
    PLX_DMA_CHANNEL_1 = 1
} PLX_DMA_CHANNEL;

/* PLX DMA handle */
typedef struct PLX_DMA_STRUCT *PLX_DMA_HANDLE;

/* PLX address spaces */
#if defined(SOLARIS) && defined(x86)
    /* On some Solaris x86 platforms, when the size of a physical memory range
       is not aligned to system page boundary, the mapping of the physical
       memory to virtual user-mode space might be inconsistent.

       PLX 9030, 9050, 9052, 9056, 9080 and 9656 boards map the run-time
       registers both to memory region BAR0 and to IO region BAR1.
       Since the size of BAR0 is smaller than a system page, this BAR might
       be mapped inconsistently. Using BAR1 to access the run-time registers
       will avoid the possible problem described above.
       The following macro definition, combined with the macro definition of
       PLX_ADDR_REG below, ensures that run-time registers will be accessed
       via IO mapping (BAR1). */
    #define REG_IO_ACCESS
#endif

#if defined(REG_IO_ACCESS)
    #define PLX_ADDR_REG AD_PCI_BAR1
#else
    #define PLX_ADDR_REG AD_PCI_BAR0
#endif
#define PLX_ADDR_REG_IO  AD_PCI_BAR1
#define PLX_ADDR_SPACE0  AD_PCI_BAR2
#define PLX_ADDR_SPACE1  AD_PCI_BAR3
#define PLX_ADDR_SPACE2  AD_PCI_BAR4
#define PLX_ADDR_SPACE3  AD_PCI_BAR5
typedef DWORD PLX_ADDR;

/* Address space information struct */
#define MAX_TYPE 8
typedef struct {
    DWORD dwAddrSpace;
    CHAR  sType[MAX_TYPE];
    CHAR  sName[MAX_NAME];
    CHAR  sDesc[MAX_DESC];
} PLX_ADDR_SPACE_INFO;

/* Interrupt result information struct */
typedef struct {
    DWORD dwCounter; /* Number of interrupts received */
    DWORD dwLost;    /* Number of interrupts not yet handled */
    WD_INTERRUPT_WAIT_RESULT waitResult; /* See WD_INTERRUPT_WAIT_RESULT values in windrvr.h */
    UINT32 u32INTCSR; /* value of interrupt control/status register */
    BYTE bDMACSR; /* value of DMA channel control/status register (relevant only for master device)*/

    /* TODO: You can add fields to PLX_INT_RESULT to store any additional
       information that you wish to pass to your diagnostics interrupt
       handler routine (DiagIntHandler() in xxx_diag.c) */
} PLX_INT_RESULT;

/* PLX diagnostics interrupt handler function type */
typedef void (*PLX_INT_HANDLER)(WDC_DEVICE_HANDLE hDev,
    PLX_INT_RESULT *pIntResult);

/* PLX diagnostics plug-and-play and power management events handler function type */
typedef void (*PLX_EVENT_HANDLER)(WDC_DEVICE_HANDLE hDev,
    DWORD dwAction);


/* Register information struct */
typedef struct {
    DWORD dwAddrSpace;       /* Number of address space in which the register resides */
                             /* For PCI configuration registers, use WDC_AD_CFG_SPACE */
    DWORD dwOffset;          /* Offset of the register in the dwAddrSpace address space */
    DWORD dwSize;            /* Register's size (in bytes) */
    WDC_DIRECTION direction; /* Read/write access mode - see WDC_DIRECTION options */
    CHAR  sName[MAX_NAME];   /* Register's name */
    CHAR  sDesc[MAX_DESC];   /* Register's description */
} WDC_REG;

typedef struct {
    PLX_DMA_HANDLE hDma;
    BOOL  fSG;
    PVOID pBuf;
} PLX_DIAG_DMA;

/*************************************************************
  Function prototypes
 *************************************************************/
PLX_DLL_API DWORD PLX_LibInit(void);
PLX_DLL_API DWORD PLX_LibUninit(void);

PLX_DLL_API BOOL PLX_DeviceFind(DWORD dwVendorId, DWORD dwDeviceId, WDC_PCI_SCAN_RESULT *pscanResult);
PLX_DLL_API DWORD PLX_DeviceOpenBySlot(WDC_DEVICE_HANDLE *pDeviceHandle, const WD_PCI_SLOT *pSlot, BOOL fIsMaster);
// songchenguang add start
//PLX_DLL_API DWORD PLX_DeviceOpenByBusSlot(WDC_DEVICE_HANDLE *pDeviceHandle, const WD_PCI_SLOT *pSlot, BOOL fIsMaster);
// songchenguang add end

PLX_DLL_API BOOL PLX_DeviceClose(WDC_DEVICE_HANDLE hDev);

PLX_DLL_API DWORD PLX_IntDisable(WDC_DEVICE_HANDLE hDev);
PLX_DLL_API BOOL PLX_IntIsEnabled(WDC_DEVICE_HANDLE hDev);
PLX_DLL_API DWORD PLX_IntEnable(WDC_DEVICE_HANDLE hDev, INT_HANDLER funcDiagIntHandler);
PLX_DLL_API DWORD IntDisable(WDC_DEVICE_HANDLE hDev);
//PLX_DLL_API DWORD P9054_IntEnable(WDC_DEVICE_HANDLE hDev);
PLX_DLL_API DWORD DLLCALLCONV Plx_PciReadCfg16(WDC_DEVICE_HANDLE hDev, DWORD dwOffset, WORD *val);

//PLX_DLL_API DWORD IntEnable(PWDC_DEVICE pDev, INT_HANDLER funcDiagIntHandler);


PLX_DLL_API void PLX_DIAG_DMAOpen(WDC_DEVICE_HANDLE hDev,
					  PLX_DIAG_DMA *pDma,
					  PLX_DMA_CHANNEL dmaChannel,
					  DWORD dwBytes,
					  DWORD dwOptions,
					  BOOL fSG);
PLX_DLL_API void PLX_DIAG_DMAClose(WDC_DEVICE_HANDLE hDev, PLX_DIAG_DMA *pDma);

BOOL WDC_DIAG_InputWriteData(PVOID pData, WDC_ADDR_SIZE dwSize);
BOOL PLX_EEPROMIsPresent(WDC_DEVICE_HANDLE hDev);
DWORD PLX_EEPROM_VPD_Read32(WDC_DEVICE_HANDLE hDev, DWORD dwOffset, UINT32 *pu32Data);
DWORD PLX_EEPROM_RT_Read16(WDC_DEVICE_HANDLE hDev, DWORD dwOffset, PWORD pwData, DWORD EEPROMmsb);

PLX_DLL_API void WDC_DIAG_ReadWriteAddr(WDC_DEVICE_HANDLE hDev, WDC_DIRECTION direction, DWORD dwAddrSpace, DWORD dwOffset, PVOID pData, WDC_ADDR_MODE mode);
PLX_DLL_API BOOL PLX_GetAddrSpaceInfo(WDC_DEVICE_HANDLE hDev, PLX_ADDR_SPACE_INFO *pAddrSpaceInfo);
PLX_DLL_API DWORD PLX_GetNumAddrSpaces(WDC_DEVICE_HANDLE hDev);
PLX_DLL_API void SetAddrSpace(WDC_DEVICE_HANDLE hDev, PLX_ADDR addrSpace);
PLX_DLL_API void WDC_DIAG_ReadWriteReg(WDC_DEVICE_HANDLE hDev, const WDC_REG *pRegs, DWORD dwNumRegs, PVOID pData, WDC_DIRECTION direction, BOOL fPciCfg);
PLX_DLL_API void PLX_SoftResetMaster(WDC_DEVICE_HANDLE hDev);
PLX_DLL_API void PLX_EEPROM_Read(WDC_DEVICE_HANDLE hDev, DWORD dwOffset, PVOID pData);

PLX_DLL_API void SwitchChannel(WDC_DEVICE_HANDLE hDev, DWORD channelNo);
PLX_DLL_API void SelectClock(WDC_DEVICE_HANDLE hDev, WORD dwValue);
PLX_DLL_API DWORD ReadProbeStatus(WDC_DEVICE_HANDLE hDev, DWORD dwProbeType);
PLX_DLL_API DWORD ReadRate(WDC_DEVICE_HANDLE hDev, DWORD dwRateType);

PLX_DLL_API void PSUTMonitorDataRead(WDC_DEVICE_HANDLE hDev,void * pData);

#define RATE_TYPE_WIDTH      1
#define RATE_TYPE_LENGTH     2

#define PROBE_1     1
#define PROBE_2     2
#define PROBE_3     3
#define PROBE_4     4
#define PROBE_5     5
#define PROBE_6     6
#define PROBE_7     7
#define PROBE_8     8


#define PLX_ReadReg8(hDev, dwOffset, pData) WDC_ReadAddr8(hDev, PLX_ADDR_REG, dwOffset, pData)
#define PLX_ReadReg16(hDev, dwOffset, pData) WDC_ReadAddr16(hDev, PLX_ADDR_REG, dwOffset, pData)
#define PLX_ReadReg32(hDev, dwOffset, pData) WDC_ReadAddr32(hDev, PLX_ADDR_REG, dwOffset, pData)
#define PLX_ReadReg64(hDev, dwOffset, pData) WDC_ReadAddr64(hDev, PLX_ADDR_REG, dwOffset, pData)

#define PLX_WriteReg8(hDev, dwOffset, pData) WDC_WriteAddr8(hDev, PLX_ADDR_REG, dwOffset, pData)
#define PLX_WriteReg16(hDev, dwOffset, pData) WDC_WriteAddr16(hDev, PLX_ADDR_REG, dwOffset, pData)
#define PLX_WriteReg32(hDev, dwOffset, pData) WDC_WriteAddr32(hDev, PLX_ADDR_REG, dwOffset, pData)
#define PLX_WriteReg64(hDev, dwOffset, pData) WDC_WriteAddr64(hDev, PLX_ADDR_REG, dwOffset, pData)

#ifdef __cplusplus
}
#endif

#endif
