export BE_ROOT=$(CURDIR)

SUB_MODULE=lib common message plugins rule be_la tool vnf
#MODULE_BEFORE=
#MODULE_AFTER=
#CLEAN_AFTER=

include $(BE_ROOT)/mk/rule.mk
