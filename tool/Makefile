MODULE_AFTER=build_sh
CLEAN_AFTER=clear_sh
include $(BE_ROOT)/mk/rule.mk
build_sh:
	gcc $(cflags) -o accsh $(filter-out ssh_exec.o,$(objects))
	gcc $(cflags) -o ssh-exec ssh_exec.o -lssh
clear_sh:
	-rm -rf  accsh
