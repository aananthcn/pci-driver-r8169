# drv-r8169
A repo to explore &amp; learn PCIe driver for PCIe Card TP-Link TG-3468 Gigabit PCI Express Network Adapter


# Getting Started
## Pre-requisite -- Hardware
* Setup an old PC having some PCIe slots, at home
* Buy a low cost PCIe card and insert it to the PCIe slot.
  * https://www.amazon.in/gp/product/B003CFATNI/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&th=1

## Pre-requisite -- Software
* Follow the steps in link below to build linux kernel and install it.
  * https://www.cyberciti.biz/tips/compiling-linux-kernel-26.html

## How to build this driver
* Just clone this repo and following the steps below:
  * `git clone https://github.com/aananthcn/drv-r8169.git`
  * `cd drv-r8169`
  * `make`
* search for *.ko file, that is our new driver!!

## Module uninstallation and installation
By default Linux will probe the PCIe card and load the driver. We first need to unload that driver and load our freshly built PCIe driver. To do that you need to perform following steps.
* Find out our device by typing the following command and search for "RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller"
  * `lspci`
* Find out the name of the driver loaded by the Linux kernel for the above device. (Hint: look product description matching the device and search for 'configuration: driver=' to locate the driver)
  * `sudo lshw -class network`
* Now it is time to unload the driver module from the kernel.
  * `sudo rmmod r8169`
* Insert the newly built kernel module
  * `sudo insmod my-r8169.ko`
* Verify if our driver module has claimed the hardware device by following the steps below:
  * `sudo lshw -class network`
  * Then search for text: "configuration: driver=pci_r8169"
