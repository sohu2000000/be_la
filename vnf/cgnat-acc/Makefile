MODULE_AFTER=build_acc
CLEAN_AFTER=clear_acc
include $(BE_ROOT)/mk/rule.mk

build_acc:
	ar rcs libcgnatacc.a $(objects)
	gcc -DFOR_TEST=1 $(cflags) -o cgnat_accelerate cgnapt_accelerate.c \
	$(filter-out cgnapt_accelerate.o,$(objects)) $(BE_ROOT)/common/be_la_log.o $(include_path)
clear_acc:
	-rm -rf libcgnatacc.a cgnat_accelerate
