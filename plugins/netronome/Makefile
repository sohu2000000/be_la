objects :=$(patsubst %.c,%.o,$(wildcard *.c))

all:$(objects)

clean:
	rm -rf $(objects)



%.o:%.c
	gcc -Wall -g -c -o $@ $< -I../common -I../../common -I../../ -I../../message
%.o:%.c %.h
	gcc -Wall -g -c -o $@ $< 
