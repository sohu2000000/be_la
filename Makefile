IDIR =./
CC=gcc
#CFLAGS=-I$(IDIR) -g
#CFLAGS=-I$(IDIR) -O3 -DMSG_DEBUG_ON
CFLAGS=-I$(IDIR) -O3

ODIR=./
LDIR =./

LIBS=-lpthread

_DEPS = be_la_main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = be_la_main.o be_la_comm.o be_la_thd.o be_la_cmd.o be_la_vendor.o be_la_accproc.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: subdirs be_la

all_dirs=common message plugins rule #vnf
subdirs:
	for i in $(all_dirs);do make -C $$i ;done;

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -I plugins/common -I common/

BE_LA_DEPS_OBJ=common/be_la_log.o message/be_acc_msg.o plugins/common/be_plugin.o \
	rule/acc_rule_be.o rule/openvswitch/acc_ovs_rule.o message/be_acc_context.o

be_la: $(OBJ)
	gcc -o $@ $^ $(BE_LA_DEPS_OBJ) $(CFLAGS) $(LIBS) 
#	strip $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core* $(INCDIR)/*~ 
	rm -f be_la
	for i in $(all_dirs);do make -C $$i clean;done;
