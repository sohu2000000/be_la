MODULE_AFTER=build_acc
CLEAN_AFTER=clear_acc
include $(BE_ROOT)/mk/rule.mk

build_acc:
	ar rcs libcgnatacc.a $(objects) 
clear_acc:
	-rm -rf libcgnatacc.a cgnat_accelerate
