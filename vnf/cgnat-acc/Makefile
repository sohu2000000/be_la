
include_path=-I ../../plugins/common
objects :=$(patsubst %.c,%.o,$(wildcard *.c))

all: $(objects)
	ar rcs libcgnatacc.a $^
	#gcc -Wall -g -o cgnat_accelerate  $^ 

%.o:%.c
	gcc -Wall -g -c -o $@ $<  $(include_path)
%.o:%.c %.h
	gcc -Wall -g -c -o $@ $<  $(include_path)



clean:
	-rm -rf $(objects) *.a cgnat_accelerate
