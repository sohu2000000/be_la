#! /bin/bash
DPDK_PATH=./samplevnf/dpdk
NIC_BIND=$DPDK_PATH/./tools/dpdk_nic_bind.py

if [ ! `which python` ] ;
then
	sudo apt-get update
	sudo apt-get -y install python gdb
fi;

if [ ! -f $DPDK_PATH/tools/dpdk_nic_bind.py ];
then
	NIC_BIND="$DPDK_PATH/./tools/dpdk-devbind.py"
fi;
pci_bus=`lspci | grep "Netronome" | cut -d ' ' -f 1 | tr '\n' ' '`
echo "sudo $NIC_BIND -b igb_uio $pci_bus"
sudo $NIC_BIND -b igb_uio $pci_bus
$NIC_BIND --status

