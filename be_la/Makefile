extern-objects=$(BE_ROOT)/common/be_la_log.o $(BE_ROOT)/message/be_acc_msg.o $(BE_ROOT)/plugins/common/be_plugin.o \
	$(BE_ROOT)/rule/acc_rule_be.o $(BE_ROOT)/rule/openvswitch/acc_ovs_rule.o $(BE_ROOT)/message/be_acc_context.o \
	$(BE_ROOT)/plugins/netronome/be_netronome_plugin.o $(BE_ROOT)/plugins/intel_qat/be_intel_qat_plugin.o \
	$(BE_ROOT)/plugins/cavium/be_liquidio_plugin.o $(BE_ROOT)/common/ssh_remote_exec.o
MODULE_AFTER=build_be
CLEAN_AFTER=clear_be
include $(BE_ROOT)/mk/rule.mk
build_be:
	#yum install libssh-devel;
	gcc $(cflags) -o be_main $(objects) $(extern-objects) -L$(BE_ROOT)/lib/ -levent -lssh
clear_be:
	-rm -rf be_main
