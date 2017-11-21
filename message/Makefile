objects :=$(patsubst %.c,%.o,$(wildcard *.c))

all:$(objects)

clean:
	rm -rf $(objects)



%.o:%.c
	gcc -Wall -g -c -o $@ $< -I../common/ -I../flow/ -I../rule/
%.o:%.c %.h
	gcc -Wall -g -c -o $@ $< -I../common/ -I../flow/ -I../rule/