export BE_ROOT=$(CURDIR)

OS_NAME=$(shell cat /etc/os-release | grep "^NAME=")

ifeq ($(findstring "UBUNTU",$(OS_NAME)),)
SUB_MODULE=lib common message plugins rule be_la tool
else
SUB_MODULE=lib common message plugins rule be_la tool vnf
endif

MODULE_BEFORE=prepare_env
include $(BE_ROOT)/mk/rule.mk

ifeq ($(findstring "UBUNTU",$(OS_NAME)),)
prepare_env:
	rpm -qa | grep -q "libssh-devel" || sudo yum -y install libssh-devel ;
else
prepare_env:
	dpkg -s libssh-dev || sudo apt install libssh-dev ; fi;
endif

