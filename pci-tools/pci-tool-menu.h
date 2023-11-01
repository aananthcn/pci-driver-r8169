#ifndef PCI_TOOL_MENU_H
#define PCI_TOOL_MENU_H


//#define PCI_TOOL_MENU	\
//	"Select a PCIe device", \
//	"Print PCIe device configuration"


#undef PCI_MENU
#define PCI_MENU(a, b)	MENU_##a,
typedef enum {
	#include "pci-tool-menu.cfg"
	MENU_ITEM_MAX
} pci_tool_menu_t;
#undef PCI_MENU

#endif