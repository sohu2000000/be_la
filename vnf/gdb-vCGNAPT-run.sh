#! /bin/bash
SAMPLEVNF_PATH="./samplevnf"
CGNAPT_PATH="$SAMPLEVNF_PATH/VNFs/vCGNAPT/build"
CGNAPT_CONFIG_PATH="$SAMPLEVNF_PATH/VNFs/vCGNAPT/config"

cfg_file="./acc_swlb_2port_2WT.cfg"
tc_file="./acc_swlb_2000_flow.tc"
./gen-flow-common.sh >  $tc_file
./gen-2000-flow.sh   >> $tc_file

#sudo gdb --args $CGNAPT_PATH/vCGNAPT -p 0x3 -f $CGNAPT_CONFIG_PATH/acc_swlb_2port_2WT.cfg  -s $CGNAPT_CONFIG_PATH/acc_swlb_2port_2WT.tc -l 2
sudo gdb --args $CGNAPT_PATH/vCGNAPT -p 0x3 -f $cfg_file  -s $tc_file -l 2

