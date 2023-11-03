#ifndef PCI_TYPES_DOT_H
#define PCI_TYPES_DOT_H

typedef struct {
	unsigned short vendor_id;
	unsigned short device_id;
	unsigned short cmd_reg;
	unsigned short stat_reg;
	unsigned char  rev_id;
	unsigned int   class_code;
	unsigned char  cache_line;
	unsigned char  lat_timer;
	unsigned char  hdr_type;
	unsigned char  bist;
	unsigned int   bar0;
	unsigned int   bar1;
	unsigned int   bar2;
	unsigned int   bar3;
	unsigned int   bar4;
	unsigned int   bar5;
	unsigned int   card_bus_p;
	unsigned short subsys_vid;
	unsigned short subsys_did;
	unsigned int   exp_rom_ba;
	unsigned char  irq_line;
	unsigned char  irq_pin;
} pci_config_t;

#endif