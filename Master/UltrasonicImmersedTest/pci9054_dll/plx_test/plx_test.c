// plx_test.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "../plx_lib_x.h"

int main(int argc, char* argv[])
{
	DWORD dwData = 0x0A0A;
	WDC_DEVICE_HANDLE   hDev;
	WDC_PCI_SCAN_RESULT scanResult;

	PLX_LibInit();
	PLX_DeviceFind(0x10b5, 0x9054, &scanResult);
	PLX_DeviceOpenBySlot(&hDev, &scanResult.deviceSlot[0], TRUE);

	WDC_DIAG_ReadWriteAddr(hDev, WDC_WRITE, 2, 2, &dwData, WDC_MODE_16);
	WDC_DIAG_ReadWriteAddr(hDev, WDC_READ, 2, 2, &dwData, WDC_MODE_16);

	SwitchChannel(hDev, CHANNEL_16);

	PLX_DeviceClose(hDev);
	PLX_LibUninit();

	return 0;
}

