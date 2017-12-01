/*****************************************************************
 * Copyright (C) 2017 Lenovo Co.,Ltd.*
 ******************************************************************
 *
 * FILENAME:
 *   be_la_main.c
 *
 * DESCRIPTION:
 *
 * AUTHOR:
 *   Feng Liu 626987
 *
 * CREATED DATE:
 *   28/10/2017
 * REVISION:
 *   1.0
 *
 * MODIFICATION HISTORY
 * --------------------
 * $Log:$
 *   Feng Liu 28/10/2017  Creation
 *****************************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>

#include "be_la_path.h"
#include "be_la_log.h"
#include "be_acc_card.h"
#include "event2/event.h"
#include "event2/event_struct.h"
#include "be_plugin.h"

int be_cmd_srv_init() {
	int server_fd = 0;
	struct sockaddr_un server_addr;

	if (0 <= access(CMD_UNIX_PATH, W_OK)) {
		remove(CMD_UNIX_PATH);
	}

	server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (server_fd < 0) {
		ACC_PERROR("socket failed: ");
		return -1;
	}

	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, CMD_UNIX_PATH);

	if (bind(server_fd, (struct sockaddr *) &server_addr,
			sizeof(server_addr))) {
		ACC_PERROR("bind: ");
		return -1;
	}

	if (listen(server_fd, 10)) {
		ACC_PERROR("listen: ");  //server listen most 10 requests.
		return -1;
	}

	return server_fd;
}

void be_cmd_request_process(evutil_socket_t fd, short events, void*arg) {

#define CMD_REQUEST_MESSAGE_MAX_SIZE 2048

	char buf[CMD_REQUEST_MESSAGE_MAX_SIZE];

	int ret = read(fd, buf, CMD_REQUEST_MESSAGE_MAX_SIZE);

	struct event*myself = (struct event*)arg;
	if (ret == -1) {
		ACC_PERROR("read failed: ");
		close(fd);
		//need delete event from base
		event_del(myself);
		return;
	}

	if (ret == 0) {
		close(fd);
		//need delete event from base
		event_del(myself);
		return;
	}

	if (ret != sizeof(cmd_hdr_t)) {
		BE_LA_ERROR("command message length error\n");
		close(fd);
		//need delete event from base
		event_del(myself);
		return;
	}

	be_la_cmd_process((cmd_hdr_t *) buf,myself->ev_base);
	close(fd);
	//need delete event from base
	event_del(myself);
	return;
}

void be_cmd_server_process(evutil_socket_t listener, short event, void*arg) {
	struct event_base *base = arg;
	struct sockaddr_storage ss;
	socklen_t slen = sizeof(ss);
	int fd = accept(listener, (struct sockaddr*) &ss, &slen);
	if (fd < 0) {
		ACC_PERROR("accept: ");
	} else if (fd > FD_SETSIZE) {
		BE_LA_ERROR("fd more than %d\n", FD_SETSIZE);
		close(fd);
	} else {
		ACC_DEBUG("accept a new client %d\n", fd);

		if (evutil_make_socket_nonblocking(fd)) {
			BE_LA_ERROR("set client socket nonblocking!\n");
			close(fd);
			return;
		}

		//read openstack request
		struct event* read_event = event_new(base, fd, EV_READ | EV_PERSIST,
				be_cmd_request_process, base);

		if (!read_event) {
			BE_LA_ERROR("alloc read event fail!\n");
			close(fd);
		}

		read_event->ev_arg = read_event;//arg is myself
		event_add(read_event, NULL);
	}
}

int main(void) {

	evutil_socket_t cmd_srv;
	struct event*listener_event;
	struct event_base*base;

#if 0
	/* become a deamon, no change dir and not redirect io */
	if(daemon(1,1) < 0) {
		ACC_PERROR("deamon failed: ");
		return 1;
	}
#endif

	if (be_la_acc_cards_init()) {
		BE_LA_ERROR("init acc cards fail!\n");
		return 1;
	}

	cmd_srv = be_cmd_srv_init();
	if (cmd_srv < 0) {
		BE_LA_ERROR("init be 'command server' fail!\n");
		return 1;
	}

	base = event_base_new();
	if (!base) {
		BE_LA_ERROR("alloc event base fail!\n");
		return 1;
	}

	listener_event = event_new(base, cmd_srv, EV_READ | EV_PERSIST,
			be_cmd_server_process, (void*) base);
	if (!listener_event) {
		BE_LA_ERROR("alloc event fail!\n");
		return 1;
	}

	event_add(listener_event, NULL);
	event_base_dispatch(base);

	return 0;
}

