#ifndef _PLX_LIB_H_
#define _PLX_LIB_H_

/* Default vendor and device IDs */
#define PLX_DEFAULT_VENDOR_ID 0x10B5 /* Vendor ID */
#define PLX_DEFAULT_DEVICE_ID 0x9054 /* Device ID */


/* Address space information struct */
#define MAX_TYPE 8
typedef struct {
    DWORD dwAddrSpace;
    CHAR  sType[MAX_TYPE];
    CHAR  sName[MAX_NAME];
    CHAR  sDesc[MAX_DESC];
} PLX_ADDR_SPACE_INFO;

/* Interrupt result information struct */
typedef struct
{
    DWORD dwCounter; /* Number of interrupts received */
    DWORD dwLost;    /* Number of interrupts not yet handled */
    WD_INTERRUPT_WAIT_RESULT waitResult; /* See WD_INTERRUPT_WAIT_RESULT values in windrvr.h */
} PLX_INT_RESULT;
/* TODO: You can add fields to PLX_INT_RESULT to store any additional
         information that you wish to pass to your diagnostics interrupt
         handler routine (DiagIntHandler() in plx_diag.c) */

/* PLX diagnostics interrupt handler function type */
typedef void (*PLX_INT_HANDLER)(WDC_DEVICE_HANDLE hDev,
    PLX_INT_RESULT *pIntResult);

/* PLX diagnostics plug-and-play and power management events handler function type */
typedef void (*PLX_EVENT_HANDLER)(WDC_DEVICE_HANDLE hDev,
    DWORD dwAction);

_declspec (dllimport) DWORD PLX_LibInit(void);
_declspec (dllimport) DWORD PLX_LibUninit(void);

_declspec (dllimport) WDC_DEVICE_HANDLE PLX_DeviceOpen(const WD_PCI_CARD_INFO *pDeviceInfo);
_declspec (dllimport) BOOL PLX_DeviceClose(WDC_DEVICE_HANDLE hDev);

_declspec (dllimport) DWORD PLX_IntEnable(WDC_DEVICE_HANDLE hDev, PLX_INT_HANDLER funcIntHandler);
_declspec (dllimport) DWORD PLX_IntDisable(WDC_DEVICE_HANDLE hDev);
_declspec (dllimport) BOOL PLX_IntIsEnabled(WDC_DEVICE_HANDLE hDev);

_declspec (dllimport) DWORD PLX_EventRegister(WDC_DEVICE_HANDLE hDev, PLX_EVENT_HANDLER funcEventHandler);
_declspec (dllimport) DWORD PLX_EventUnregister(WDC_DEVICE_HANDLE hDev);
_declspec (dllimport) BOOL PLX_EventIsRegistered(WDC_DEVICE_HANDLE hDev);

_declspec (dllimport) DWORD PLX_GetNumAddrSpaces(WDC_DEVICE_HANDLE hDev);
_declspec (dllimport) BOOL PLX_GetAddrSpaceInfo(WDC_DEVICE_HANDLE hDev, PLX_ADDR_SPACE_INFO *pAddrSpaceInfo);

_declspec (dllimport) const char *PLX_GetLastErr(void);

#endif