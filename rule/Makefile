objects :=$(patsubst %.c,%.o,$(wildcard *.c))

all:$(objects)
	make -C openvswitch/

clean:
	rm -rf $(objects)
	make -C openvswitch/ clean



%.o:%.c
	gcc -Wall -g -c -o $@ $< -I../flow -I../message/
%.o:%.c %.h
	gcc -Wall -g -c -o $@ $< 
