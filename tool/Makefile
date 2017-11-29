MODULE_AFTER=build_sh
CLEAN_AFTER=clear_sh
include $(BE_ROOT)/mk/rule.mk
build_sh:
	gcc $(cflags) -o accsh $(objects) 
clear_sh:
	-rm -rf  accsh
