

all: cgnapt_accelerate.o acc_client.o acc_vsocket.o acc_channel_vsocket.o acc_log.o
	ar rcs libcgnatacc.a $^
	#gcc -Wall -g -o cgnat_accelerate  $^ 

%.o:%.c
	gcc -Wall -g -c -o $@ $< 
%.o:%.c %.h
	gcc -Wall -g -c -o $@ $< 



clean:
	-rm -rf *.o *.a cgnat_accelerate
