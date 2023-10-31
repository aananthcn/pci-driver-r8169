#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>


MODULE_LICENSE("GPL-2.0");


#define REALTEK_PCI_VENDOR_ID	(0x10ec)
#define REALTEK_TPLINK_1G_ETH	(0x8161)


static const struct pci_device_id mypci_ids[] = {
	{
		PCI_DEVICE(REALTEK_PCI_VENDOR_ID, REALTEK_TPLINK_1G_ETH)
	},
	{0}
};


static int pci_r8169_probe(struct pci_dev *dev, const struct pci_device_id *dev_id)
{
	printk(KERN_ALERT "pci-r8169: pci_probe() called!\n");
	return 0;
}

static void pci_r8169_remove(struct pci_dev *dev)
{
	printk(KERN_ALERT "pci-r8169: pci_remove() called!\n");
}


static struct pci_driver pci_driver = {
	.name = "pci_r8169",
	.id_table = mypci_ids,
	.probe = pci_r8169_probe,
	.remove = pci_r8169_remove,
};


static int __init pci_r8169_init(void)
{
	printk(KERN_ALERT "pci-r8169: Hello, Aananth! Welcome back to Linux Device Driver.\n");
	printk(KERN_ALERT "pci-r8169: Let us learn how to write a pci driver on Linux\n");

	return pci_register_driver(&pci_driver);
}


static void __exit pci_r8169_exit(void)
{
	printk(KERN_ALERT "pci-r8169: pci_r8169_exit() called!\n");
	pci_unregister_driver(&pci_driver);
}

module_init(pci_r8169_init);
module_exit(pci_r8169_exit);