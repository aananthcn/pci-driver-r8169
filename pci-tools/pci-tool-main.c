#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local file includes
#include "pci-tool-menu.h"
#include "pci-commands.h"

// Macros
#define PDEV_ADDR_LEN	(32)

// Globals
char PCIeDevAddr[PDEV_ADDR_LEN] = {0};


#undef PCI_MENU
#define PCI_MENU(a, b)	b,
char *PCIeToolMenu[] = {
	#include "pci-tool-menu.cfg"
	'\0'
};
#undef PCI_MENU



int print_cmd_menu(void) {
	int menu_cnt_max;
	int menu_cmd = -1;

	printf("\n\nMenu Command List:\n------------------\n\n");
	for (int i = 0; PCIeToolMenu[i]; i++) {
		printf("  [%2d] %s\n", i, PCIeToolMenu[i]);
		menu_cnt_max = i;
	}

	printf("\nPlease type the command number and press ENTER: ");
	scanf("%d", &menu_cmd);
	if ((menu_cmd < 0) || (menu_cmd > menu_cnt_max)) {
		printf("ERROR: There is no such menu with number %d!\n", menu_cmd);
		exit(1);
	}

	return menu_cmd;
}


void handle_cmd_menu(int cmd) {
	pci_cfg_t pci_config;

	switch (cmd) {
		case MENU_DEVICE_SELECT:
			cmd_select_device(PCIeDevAddr);
			printf("\nDevice selected is: %s\n", PCIeDevAddr);
			break;
		case MENU_PRINT_CFG_HDR:
			cmd_get_config_header(PCIeDevAddr, &pci_config);
			cmd_print_configs(stdout, &pci_config, PRNT_COL);
			break;
		case MENU_PRINT_CFG2FILE:
			cmd_get_all_configs_to_file();
			break;
		case MENU_PRINT_POWER_CAP:
			cmd_get_config_header(PCIeDevAddr, &pci_config);
			cmd_print_power_mgmt_caps(PCIeDevAddr, stdout, &pci_config, PRNT_COL);
			break;
		case MENU_PRINT_EXTENDED_CAP:
			cmd_get_config_header(PCIeDevAddr, &pci_config);
			cmd_print_extended_caps(PCIeDevAddr, stdout, &pci_config, PRNT_COL);
			break;
		default:
			printf("\nThe command selected (%d) is not supported yet!\n", cmd);
			break;
	}
}


int main()
{
	int menu_cmd;

	/* Select a PCIe device first */
	cmd_select_device(PCIeDevAddr);
	printf("\nDevice selected is: %s\n", PCIeDevAddr);

	while (1) {
		menu_cmd = print_cmd_menu();
		handle_cmd_menu(menu_cmd);
	}


	return 0;
}
