#include <sys/un.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <memory.h>

#define BUF_LEN 2048

enum tag_msg_type{
    MSG_TYPE_DATA = 1,
    MSG_TYPE_DATA_MORE,      
    MSG_TYPE_MAX
};

typedef struct tag_acc_proc_msg{
    unsigned int msg_type;
    unsigned int acc_type;
    char buffer[BUF_LEN];
}acc_proc_msg_t;

int main(){
	/*1. */
	int fd = open("/dev/virtio-ports/accla", O_RDWR);
	int ret = -1;
	char write_buf[256] = {"Hello World, I am VM !!!"}, feedback[256] = {'\0'};
	acc_proc_msg_t msg;
	if(fd < 0)
		perror("open failed:");

	/*2. */
	memset(&msg, 0x0, sizeof(msg));
	msg.msg_type = MSG_TYPE_DATA;
	msg.acc_type = 0x12;
	strcpy(msg.buffer, "Hello World, I am VM !!!" );	
	write(fd, &msg, sizeof(msg));

	/*3. */
	memset(&msg, 0x0, sizeof(msg));
	ret = read(fd, &msg, sizeof(msg));
	printf("read feedback: len = %d, msg.acc_type = %x, msg.buffer = %s\n", ret, msg.acc_type, msg.buffer);

	/*4. */
	//write(fd, "end\n", 5);

out:
	/*5. */
	sleep(3);
	close(fd);
	return 0;
}
