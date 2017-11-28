MODULE_AFTER=build_acc
CLEAN_AFTER=clear_acc
include $(BE_ROOT)/mk/rule.mk

build_acc:
	ar rcs libcgnatacc.a
	gcc -DFOR_TEST=1 $(cflags) -o cgnat_accelerate cgnapt_accelerate.c \
	$(filter-out cgnapt_accelerate.o,$(objects)) $(BE_ROOT)/common/be_la_log.o $(include_path)
clear_acc:
	-rm -rf libcgnatacc.a cgnat_accelerate
	
#include_path=-I ../../plugins/common -I ../../flow -I../../common -I../../message
#objects :=$(patsubst %.c,%.o,$(wildcard *.c))

#all: $(objects) ../../common/be_la_log.o
#	ar rcs libcgnatacc.a $^
#	gcc -Wall -g -o cgnat_accelerate  $^ 

#%.o:%.c
#	gcc -Wall -g -c -o $@ $<  $(include_path)
#%.o:%.c %.h
#	gcc -Wall -g -c -o $@ $<  $(include_path)
#
#clean:
#	-rm -rf $(objects) *.a cgnat_accelerate
