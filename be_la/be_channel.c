/*
 * be_channel.c
 *
 *  Created on: Nov 22, 2017
 *      Author: anlang
 */
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>
#include <stdio.h>

#include "be_la_log.h"
#include "event2/event.h"
#include "be_acc_msg.h"
#include "be_acc_card.h"
#include "be_channel.h"

int be_vm_channel_init(char * socket_path) {
	int sockfd = 0, ret = 0;
	struct sockaddr_un address;

	/*UNIX SOCKET*/
	if (access(socket_path, W_OK) < 0) {
		perror("access failed: ");
		return -1;
	}

	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (-1 == sockfd) {
		perror("socket failed: ");
		return -1;
	}

	bzero((void *) &address, sizeof(struct sockaddr_un));
	address.sun_family = AF_UNIX;
	strncpy((char*) address.sun_path, socket_path,
			sizeof(address.sun_path) - 1);

	ret = connect(sockfd, (struct sockaddr *) &address, sizeof(address));
	if (ret == -1) {
		perror("connect failed: ");
		return -1;
	}

	BE_LA_LOG("open vm channel success, sockfd = %d\n", sockfd);
	return sockfd;
}

void do_vm_request_read(evutil_socket_t fd, short events, void*arg) {
	//struct acc_context*context;
	be_card_inner_t* card;
	struct acc_msg_header hdr;
	struct acc_msg_header reply_hdr;
	char buf[2048];
	char out_buf[2048];
	ssize_t n_size;
	int out_n_size = 0;

	card = (be_card_inner_t*) arg;
	//state = (struct acc_context*) arg;

	if (acc_unix_recv_message((void*) ((intptr_t) fd), &hdr, buf, &n_size)) {
		ACC_ERROR("recv message fail!\n");
		//XXX
		//free_fd_state(state);
		return;
	}

	ACC_DEBUG("recv message len:%d,fun_type:%d,msg_type:%d,code:%d\n",
			hdr.total_len, hdr.acc_type, hdr.msg_type, hdr.code);

	reply_hdr.total_len = sizeof(reply_hdr);
	reply_hdr.acc_type = hdr.acc_type;
	reply_hdr.msg_type = hdr.msg_type;
	reply_hdr.code = ACC_UNIX_SUCCESS;

#if 1
	if (be_acc_message_process(card, hdr.msg_type, buf, n_size, out_buf,
			&out_n_size)) {
		ACC_ERROR("process message fail!\n");
		reply_hdr.code = ACC_UNIX_FAIL;
	}
#else
	if(acc_netronome_proccess(state,hdr,n_size, out_buf,
					&out_n_size))
	{
		ACC_ERROR("process message fail!\n");
		reply_hdr.code = ACC_UNIX_FAIL;
	}
#endif

	reply_hdr.total_len += out_n_size;
	ACC_DEBUG("reply message len:%d,type:%d,code:%d\n", reply_hdr.total_len,
			reply_hdr.msg_type, reply_hdr.code);
	if (!acc_unix_send_message((void*) ((intptr_t) fd), &reply_hdr, out_buf,
			out_n_size)) {
		ACC_ERROR("send message fail!\n");
		return;
	}
}

int unix_socket_clnt_init(char * socket_path){
    int sockfd  = 0, ret = 0;
    struct sockaddr_un address;

    /*UNIX SOCKET*/
    if(access(socket_path, W_OK) < 0)
    {
        perror("access failed: ");
        return -1;
    }

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sockfd)
    {
        perror("socket failed: ");
        return -1;
    }

    bzero((void *)&address, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    strncpy((char*)address.sun_path, socket_path, sizeof(struct sockaddr_un) - 1);

    ret = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    if(ret == -1){
        perror("connect failed: ");
        return -1;
    }


    BE_LA_LOG("init_unix_socket sockfd = %d\n", sockfd);
    return sockfd;
}

int vm_channel_open(acc_card_t* card, void* arg) {
	be_card_inner_t* be_card = (be_card_inner_t*) card;
	int fd = unix_socket_clnt_init(card->vmpath);
	if (be_card->fd < 0) {
		return -1;
	}
	assert(be_card->context== NULL);

	struct event_base *base = arg;

	if (evutil_make_socket_nonblocking(fd)) {
		ACC_ERROR("set client socket nonblocking!\n");
		close(fd);
		return -1;
	}

	//read vm request
	be_card->read_event = event_new(base, fd, EV_READ | EV_PERSIST,
			do_vm_request_read, be_card);
	if (!be_card->read_event) {
		ACC_ERROR("alloc read event fail!\n");
		close(fd);
		return -1;
	}
	event_add(be_card->read_event, NULL);
	return 0;
}

int vm_channel_close(acc_card_t*card) {
	*(int*) 0 = 0;
	return -1;
}

