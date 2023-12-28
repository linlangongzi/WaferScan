#include "windrvr.h"
#include "print_struct.h"
#include <stdio.h>

void WD_CARD_print(WD_CARD *pCard, char *pcPrefix)
{
    DWORD i;

    for (i=0; i<pCard->dwItems; i++)
    {
        WD_ITEMS item = pCard->Item[i];
        printf ("%sItem ", pcPrefix);
        switch (item.item)
        {
        case ITEM_MEMORY:
            printf ("Memory: range 0x%x-0x%x", 
                item.I.Mem.dwPhysicalAddr, item.I.Mem.dwPhysicalAddr+item.I.Mem.dwBytes-1);
            break;
        case ITEM_IO:
            printf ("IO: range 0x%x-0x%x", 
                item.I.IO.dwAddr, item.I.IO.dwAddr+item.I.IO.dwBytes-1);
            break;
        case ITEM_INTERRUPT:
            printf ("Interrupt: irq %d", item.I.Int.dwInterrupt);
            break;
        case ITEM_BUS:
            printf ("Bus: type %d, bus number %d, slot/func 0x%x", 
                item.I.Bus.dwBusType, item.I.Bus.dwBusNum, item.I.Bus.dwSlotFunc);
            break;
        default:
            printf ("Invalid item type");
            break;
        }
        printf ("\n");
    }
 }
