INCLUDE_PATH=-I../../flow -I../ -I../../common -I../../message
all: acc_ovs_rule.o

%.o:%.c
	#gcc -E -o abc.ff $< $(INCLUDE_PATH)
	gcc -Wall -g -c -o $@ $< $(INCLUDE_PATH)
%.o:%.c %.h
	gcc -Wall -g -c -o $@ $< $(INCLUDE_PATH)



clean:
	-rm -rf *.o