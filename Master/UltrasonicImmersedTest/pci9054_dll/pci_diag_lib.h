#ifndef _PCI_DIAG_LIB_H_
#define _PCI_DIAG_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

BOOL PCI_Get_WD_handle(HANDLE *phWD);
void PCI_Print_card_info(WD_PCI_SLOT pciSlot);
void PCI_Print_all_cards_info();
void PCI_EditConfigReg(WD_PCI_SLOT pciSlot);
BOOL PCI_ChooseCard(WD_PCI_SLOT *ppciSlot);

#ifdef __cplusplus
}
#endif

#endif
