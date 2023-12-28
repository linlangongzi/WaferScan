////////////////////////////////////////////////////////////////
// File - PCI_DIAG_LIB.C
//
// Utility functions for printing card information,
// detecting PCI cards, and accessing PCI configuration
// registers.
// 
////////////////////////////////////////////////////////////////

#include "windrvr.h"
#ifdef _USE_SPECIFIC_KERNEL_DRIVER_
    #undef WD_Open
    #define WD_Open WD_OpenKernelHandle
    #if defined(UNIX)
        #undef WD_FUNCTION
        #define WD_FUNCTION(wFuncNum,h,pParam,dwSize,fWait) ((ULONG) ioctl((int)(h), wFuncNum, pParam))
    #endif
#endif
#include "pci_diag_lib.h"
#include "print_struct.h"
#include <stdio.h>

// input of command from user
static char line[256];

BOOL PCI_Get_WD_handle(HANDLE *phWD)
{
    WD_VERSION ver;

    *phWD = WD_Open();

    // Check whether handle is valid and version OK
    if (*phWD==INVALID_HANDLE_VALUE) 
    {
      //        printf("Failed opening  WD_PROD_NAME  device\n");
        return FALSE;
    }

    BZERO(ver);
    WD_Version(*phWD,&ver);
    if (ver.dwVer<WD_VER) 
    {
      //        printf("Incorrect  WD_PROD_NAME  version\n");
        printf("Incorrect WinDriver version\n");
        WD_Close (*phWD);
        *phWD = INVALID_HANDLE_VALUE;
        return FALSE;
    }

    return TRUE;
}

void PCI_Print_card_info(WD_PCI_SLOT pciSlot)
{
    HANDLE hWD;
    WD_PCI_CARD_INFO pciCardInfo;

    if (!PCI_Get_WD_handle (&hWD)) return;

    BZERO(pciCardInfo);
    pciCardInfo.pciSlot = pciSlot;
    WD_PciGetCardInfo (hWD, &pciCardInfo);

    WD_CARD_print(&pciCardInfo.Card, "   ");

    WD_Close (hWD);
}

void PCI_Print_all_cards_info() 
{
    HANDLE hWD;
    int i;
    WD_PCI_SCAN_CARDS pciScan;
    WD_PCI_SLOT pciSlot;
    WD_PCI_ID   pciId;

    if (!PCI_Get_WD_handle (&hWD)) return;

    BZERO(pciScan);
    pciScan.searchId.dwVendorId = 0;
    pciScan.searchId.dwDeviceId = 0;

    printf ("Pci bus scan:\n\n");
    WD_PciScanCards (hWD,&pciScan);

    for (i=0; i<(int)pciScan.dwCards; i++)
    {
        CHAR tmp[100];
        pciId = pciScan.cardId[i];
        pciSlot = pciScan.cardSlot[i];
        printf("Bus %d Slot %d Function %d, VendorID %04x DeviceID %04x\n",
            pciSlot.dwBus, pciSlot.dwSlot, pciSlot.dwFunction, pciId.dwVendorId, pciId.dwDeviceId);

        PCI_Print_card_info(pciSlot);
        printf("Press Enter to continue to next slot\n");
        gets (tmp);
    }
    WD_Close (hWD);
}

DWORD PCI_ReadBytes(HANDLE hWD, WD_PCI_SLOT pciSlot, DWORD dwOffset, DWORD dwBytes)
{
    WD_PCI_CONFIG_DUMP pciCnf;
    DWORD dwVal = 0;
    DWORD dwMask;
    DWORD dwDwordOffset = dwOffset % 4;

    BZERO(pciCnf);
    pciCnf.pciSlot = pciSlot;
    pciCnf.pBuffer = &dwVal;
    pciCnf.dwOffset = dwOffset;
    pciCnf.dwOffset -= dwDwordOffset;
    pciCnf.dwBytes = 4;
    pciCnf.fIsRead = TRUE;
    WD_PciConfigDump(hWD,&pciCnf);
    
    dwVal = dwVal >> (dwDwordOffset*8);
    switch (dwBytes)
    {
    case 1:
        dwMask = 0xff;
        break;
    case 2:
        dwMask = 0xffff;
        break;
    case 3:
        dwMask = 0xffffff;
        break;
    case 4:
        dwMask = 0xffffffff;
        break;
    }
    dwVal &= dwMask;
    return dwVal;
}

void PCI_WriteBytes(HANDLE hWD, WD_PCI_SLOT pciSlot, DWORD dwOffset, DWORD dwBytes, DWORD dwData)
{
    WD_PCI_CONFIG_DUMP pciCnf;
    DWORD dwVal = 0;
    DWORD dwMask;
    DWORD dwDwordOffset = dwOffset % 4;

    BZERO(pciCnf);
    pciCnf.pciSlot = pciSlot;
    pciCnf.pBuffer = &dwVal;
    pciCnf.dwOffset = dwOffset;
    pciCnf.dwOffset -= dwDwordOffset;
    pciCnf.dwBytes = 4;
    pciCnf.fIsRead = TRUE;
    WD_PciConfigDump(hWD,&pciCnf);
    
    switch (dwBytes)
    {
    case 1:
        dwMask = 0xff;
        break;
    case 2:
        dwMask = 0xffff;
        break;
    case 3:
        dwMask = 0xffffff;
        break;
    case 4:
        dwMask = 0xffffffff;
        break;
    }
    dwVal &= ~(dwMask << (dwDwordOffset*8));
    dwVal |= (dwMask & dwData) << (dwDwordOffset*8);

    pciCnf.fIsRead = FALSE;
    WD_PciConfigDump(hWD,&pciCnf);
}

void PCI_EditConfigReg(WD_PCI_SLOT pciSlot)
{
    HANDLE hWD;
    struct 
    {
        CHAR *name;
        DWORD dwOffset;
        DWORD dwBytes;
        DWORD dwVal;
    } fields[30];

    int cmd;
    int i;

    if (!PCI_Get_WD_handle (&hWD)) return;

    i = 0;
    fields[i].name = "VID"; fields[i].dwOffset = 0x0; fields[i++].dwBytes = 2;
    fields[i].name = "DID"; fields[i].dwOffset = 0x2; fields[i++].dwBytes = 2;
    fields[i].name = "CMD"; fields[i].dwOffset = 0x4; fields[i++].dwBytes = 2;
    fields[i].name = "STS"; fields[i].dwOffset = 0x6; fields[i++].dwBytes = 2;
    fields[i].name = "RID"; fields[i].dwOffset = 0x8; fields[i++].dwBytes = 1;
    fields[i].name = "CLCD"; fields[i].dwOffset = 0x9; fields[i++].dwBytes = 3;
    fields[i].name = "CALN"; fields[i].dwOffset = 0xc; fields[i++].dwBytes = 1;
    fields[i].name = "LAT"; fields[i].dwOffset = 0xd; fields[i++].dwBytes = 1;
    fields[i].name = "HDR"; fields[i].dwOffset = 0xe; fields[i++].dwBytes = 1;
    fields[i].name = "BIST"; fields[i].dwOffset = 0xf; fields[i++].dwBytes = 1;
    fields[i].name = "BADDR0"; fields[i].dwOffset = 0x10; fields[i++].dwBytes = 4;
    fields[i].name = "BADDR1"; fields[i].dwOffset = 0x14; fields[i++].dwBytes = 4;
    fields[i].name = "BADDR2"; fields[i].dwOffset = 0x18; fields[i++].dwBytes = 4;
    fields[i].name = "BADDR3"; fields[i].dwOffset = 0x1c; fields[i++].dwBytes = 4;
    fields[i].name = "BADDR4"; fields[i].dwOffset = 0x20; fields[i++].dwBytes = 4;
    fields[i].name = "BADDR5"; fields[i].dwOffset = 0x24; fields[i++].dwBytes = 4;
    fields[i].name = "EXROM"; fields[i].dwOffset = 0x30; fields[i++].dwBytes = 4;
    fields[i].name = "INTLN"; fields[i].dwOffset = 0x3c; fields[i++].dwBytes = 1;
    fields[i].name = "INTPIN"; fields[i].dwOffset = 0x3d; fields[i++].dwBytes = 1;
    fields[i].name = "MINGNT"; fields[i].dwOffset = 0x3e; fields[i++].dwBytes = 1;
    fields[i].name = "MAXLAT"; fields[i].dwOffset = 0x3f; fields[i++].dwBytes = 1;
    fields[i].name = NULL; fields[i].dwOffset = 0; fields[i++].dwBytes = 0;
    fields[i].name = NULL; fields[i].dwOffset = 0; fields[i++].dwBytes = 0;
    do
    {
        int row;
        int col;

        printf ("\n");
        printf ("Edit PCI configuration registers\n");
        printf ("--------------------------------\n");
        for (row = 0; row<=10; row++)
        {
            for (col = 0; col <=1; col++)
            {
                if (col==0) i = row;
                else i = row + 10;

                if (row==10 && col==0)
                {
                    printf("%26s","");
                }
                else
                {
                    DWORD dwVal;
                    dwVal = PCI_ReadBytes(hWD, pciSlot, fields[i].dwOffset, fields[i].dwBytes);
                    fields[i].dwVal = dwVal;
                    printf ("%2d. %6s : %0*x %*s    ",i+1, fields[i].name, fields[i].dwBytes*2, fields[i].dwVal, 8-fields[i].dwBytes*2, "");
                }
                if (col==1) printf ("\n");
            }
        }

        printf ("99. Back to main menu\n");
        printf ("Choose register to read from / write to, or 99 to exit: ");
        cmd = 0;
        gets(line);
        sscanf(line, "%d",&cmd);
        if (cmd>=1 && cmd <=21)
        {
            i = cmd-1;
            printf ("Enter value to write to %s register (or 'X' to cancel): ",fields[i].name);
            gets (line);
            if (toupper (line[0])!='X')
            {
                DWORD dwVal;
                dwVal = 0;
                sscanf(line,"%x",&dwVal);
                if ((dwVal>0xff && fields[i].dwBytes==1)|| 
                    (dwVal>0xffff && fields[i].dwBytes==2)|| 
                    (dwVal>0xffffff && fields[i].dwBytes==3))
                {
                    printf ("Error: value to big for register\n");
                }
                else
                {
                    PCI_WriteBytes(hWD, pciSlot, fields[i].dwOffset, fields[i].dwBytes, dwVal);
                }
            }
        }
    } while (cmd!=99);

    WD_Close (hWD);
}

BOOL PCI_ChooseCard(WD_PCI_SLOT *ppciSlot)
{
    BOOL fHasCard;
    WD_PCI_SCAN_CARDS pciScan;
    DWORD dwVendorID, dwDeviceID;
    HANDLE hWD;
    DWORD i;

    if (!PCI_Get_WD_handle (&hWD)) return FALSE;

    fHasCard = FALSE;

    for (;!fHasCard;) 
    {
        dwVendorID = 0;
        printf ("Enter VendorID: ");
        gets (line);
        sscanf (line, "%x",&dwVendorID);
        if (dwVendorID==0) 
            break;

        printf ("Enter DeviceID: ");
        gets (line);
        sscanf (line, "%x",&dwDeviceID);

        BZERO(pciScan);
        pciScan.searchId.dwVendorId = dwVendorID;
        pciScan.searchId.dwDeviceId = dwDeviceID;
        WD_PciScanCards (hWD, &pciScan);
        if (pciScan.dwCards==0) // Find at least one card
        {
            printf("Could not find PCI card\n");
        }
        else if (pciScan.dwCards==1)
        {
            *ppciSlot = pciScan.cardSlot[0];
            fHasCard = TRUE;
        }
        else
        {
            printf("Found %d matching PCI cards\n", pciScan.dwCards);
            printf("Select card (1-%d): ", pciScan.dwCards);
            i = 0;
            gets (line);
            sscanf (line, "%d",&i);
            if (i>=1 && i <=pciScan.dwCards)
            {
                *ppciSlot = pciScan.cardSlot[i-1];
                fHasCard = TRUE;
            }
            else printf ("Choice out of range\n");
        }
        if (!fHasCard)
        {
            printf ("Do you want to try a different VendorID/DeviceID? ");
            gets (line);
            if (toupper(line[0])!='Y')
                break;
        }
    }

    WD_Close (hWD);

    return fHasCard;
}
