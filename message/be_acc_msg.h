/*
 * acc_unix.h
 *
 *  Created on: Oct 24, 2017
 *      Author: anlang
 */

#ifndef SRC_COMMON_ACC_UNIX_COMMON_H_
#define SRC_COMMON_ACC_UNIX_COMMON_H_

#include <inttypes.h>
#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <string.h>

#include "be_acc_context.h"
#include "be_la_vendor.h"
struct be_acc_card_;

struct acc_msg_header {
	int total_len;
	enum tag_acc_type acc_type;
#define ACC_UNIX_HELLO      1
#define ACC_UNIX_ADD_FLOWS  2
#define ACC_UNIX_DISCONNECT 3
#define ACC_UNIX_MAX_TYPE   10
	int msg_type;
#define ACC_UNIX_SUCCESS 0
#define ACC_UNIX_FAIL    1
	int code;
	char data[0];
};

static inline ssize_t _iovec_copy(struct iovec*array, ssize_t array_len,
		ssize_t index, ssize_t offset, struct iovec*remain_array) {
	ssize_t copy_number = 0;
	if (index < array_len) {
		remain_array[copy_number].iov_base = array[index].iov_base + offset;
		remain_array[copy_number].iov_len = array[index].iov_len - offset;
		//copy_number ++;//cppy_number == 1
		for (++index, copy_number++; index < array_len;
				++index, copy_number++) {
			remain_array[copy_number].iov_base = array[index].iov_base;
			remain_array[copy_number].iov_len = array[index].iov_len;
		}
	}

	return copy_number;
}




static inline int _readn(void*handle, char*msg, ssize_t len) {
	ssize_t ret;
	ssize_t remain = len;
	ssize_t readed = 0;

	assert(len>=0);

	int fd = (intptr_t) handle;
	while (remain) {
		ret = read(fd, msg + readed, remain);
		if (ret == -1) {
			if (errno == EINTR || errno == EAGAIN) {
				continue;
			}
			ACC_DEBUG("read message fail %s(msg=%p,readed=%d,offset=%p,remain=%d)!\n",strerror(errno),msg,readed,msg+readed,remain);
			*(int*)0=0;
			return -1;
		}
		if(ret == 0)
		{
			ACC_DEBUG("fd need close!\n");
			return -1;
		}
		remain -= ret;
		readed += ret;
	}

	return 0;
}

//尝试从handle中读取一条message消息
static inline int acc_unix_recv_message(void*handle,
		struct acc_msg_header*hdr, char*body, ssize_t *recv_n) {
	//int fd = (intptr_t) handle;

	if (_readn(handle, (char*)hdr, sizeof(*hdr))) {
		return -1;
	}

	int data_len = hdr->total_len - sizeof(*hdr);
	if (_readn(handle, body, data_len)) {
		return -1;
	}

	if (recv_n) {
		*recv_n = data_len;
	}
	return 0;
}

//等待handle的读事件
static inline int wait_read_event(void*handle) {
	int fd = (intptr_t) handle;

	struct pollfd fds;
	fds.fd = fd;
	fds.events = POLLIN;

	if (poll(&fds, 1, -1) <= 0) {
		return -1;
	}

	if (fds.revents) {
		return 0;
	}

	return -1;
}

static inline ssize_t _get_next_send_iovec(struct iovec*array,
		ssize_t array_len, ssize_t offset, struct iovec* remain_array) {
	ssize_t i;
	for (i = 0; i < array_len;) {
		if (offset >= array[i].iov_len) {
			offset -= array[i].iov_len;
			i++;
		} else {
			break;
		}
	}
	return _iovec_copy(array, array_len, i, offset, remain_array);
}

//如果发送成功，返回true,否则false
static inline int acc_unix_send_message(void*handle,
		struct acc_msg_header*hdr, const char*body, const ssize_t body_len) {

	int fd = (intptr_t) handle;
	ssize_t number = 2;
	struct iovec message[2] = { { .iov_base = hdr, .iov_len = sizeof(*hdr) }, {
			.iov_base = (void*)body, .iov_len = body_len } };

	struct iovec* need_send = message;

	ssize_t ret;
	while (number) {
		ret = writev(fd, need_send, number);
		if (ret == -1) {
			if ( errno == EINTR || errno == EAGAIN) {
				continue;
			}
			ACC_ERROR("write message fail, %s!\n",strerror(errno));
			break;
		}
		number = _get_next_send_iovec(need_send, number, ret, need_send);
	}

	return number == 0;
}

static inline int acc_unix_send_recv(void*handle,
		struct acc_msg_header*send_hdr, char*send_body,
		ssize_t send_body_len, struct acc_msg_header*recv_hdr,
		char*recv_body, ssize_t*recv_len) {
	ACC_DEBUG("send message :acc_type:%d,code:%d,message_type:%d,total_len:%d\n",send_hdr->acc_type,send_hdr->code,
			send_hdr->msg_type,send_hdr->total_len);
	if (!acc_unix_send_message(handle, send_hdr, send_body, send_body_len)) {
		//send fail
		ACC_ERROR("send message fail!\n");
		return -1;
	}

	if (wait_read_event(handle)) {
		ACC_ERROR("send message fail!\n");
		return -1;
	}

	if (acc_unix_recv_message(handle, recv_hdr, recv_body, recv_len)) {
		ACC_ERROR("recv message fail!\n");
		return -1;
	}

	ACC_DEBUG("rcv message :acc_type:%d,code:%d,message_type:%d,total_len:%d\n",recv_hdr->acc_type,recv_hdr->code,
			recv_hdr->msg_type,recv_hdr->total_len);
	return 0;
}

int be_acc_message_process(struct be_acc_card_ *state, int type, char*message,
		int len, char*out_msg, int* out_len);

void be_acc_flush_rules(be_card_inner_t*card);
#endif /* SRC_COMMON_ACC_UNIX_COMMON_H_ */
