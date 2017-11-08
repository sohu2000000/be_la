IDIR =./
CC=gcc
#CFLAGS=-I$(IDIR) -g
CFLAGS=-I$(IDIR) -O3

ODIR=./
LDIR =./

LIBS=-lpthread

_DEPS = accsh.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = accsh.o accsh_comm.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

accsh: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
	strip $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core* $(INCDIR)/*~ 
	rm -f accsh
