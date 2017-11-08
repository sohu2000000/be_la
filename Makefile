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

_OBJ = be_la_main.o be_la_log.o be_la_comm.o be_la_thd.o be_la_cmd.o be_la_vendor.o be_la_plugin.o be_la_accproc.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

be_la: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
#	strip $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core* $(INCDIR)/*~ 
	rm -f be_la
