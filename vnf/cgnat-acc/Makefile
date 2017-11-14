
all: cgnapt_accelerate.o acc_client.o acc_vsocket.o acc_unix_vsocket.o acc_log.o
	gcc -Wall -g -o cgnat_accelerate  $^ $(INCLUDE_PATH)

%.o:%.c
	gcc -Wall -g -c -o $@ $< $(INCLUDE_PATH)
%.o:%.c %.h
	gcc -Wall -g -c -o $@ $< $(INCLUDE_PATH)



clean:
	-rm -rf *.o
