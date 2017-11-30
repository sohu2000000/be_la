extern-objects=$(BE_ROOT)/common/be_la_log.o $(BE_ROOT)/message/be_acc_msg.o $(BE_ROOT)/plugins/common/be_plugin.o \
	$(BE_ROOT)/rule/acc_rule_be.o $(BE_ROOT)/rule/openvswitch/acc_ovs_rule.o $(BE_ROOT)/message/be_acc_context.o \
	$(BE_ROOT)/plugins/netronome/be_netronome_plugin.o
MODULE_AFTER=build_be
CLEAN_AFTER=clear_be
include $(BE_ROOT)/mk/rule.mk
build_be:
	gcc $(cflags) -o be_main $(objects) $(extern-objects) -L$(BE_ROOT)/lib/ -levent
clear_be:
	-rm -rf be_main
