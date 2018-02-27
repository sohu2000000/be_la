/*
 * ssh_remote_exec.c
 *
 *  Created on: Jan 8, 2018
 *      Author: anlang
 */

#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>

#include "be_la_log.h"

void free_channel(ssh_channel channel) {
	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);
}

void free_session(ssh_session session) {
	ssh_disconnect(session);
	ssh_free(session);
}

void message_error(ssh_session session,char*message) {
	ACC_ERROR("%s,error=%s\n",message, ssh_get_error(session));
	free_session(session);
}

int ssh_remote_exec(char*remote_ip, char*username,int port, char*password,char*env, char*cmd) {

	ssh_session session;
	ssh_channel channel;
	int rc, ret = -1;
	char buffer[1024];
    char cmd_buf[4096];
	unsigned int nbytes;

	session = ssh_new();
	if (!session) {
		ACC_ERROR("create ssh session fail\n");
		return -1;
	}

	ssh_options_set(session, SSH_OPTIONS_HOST, remote_ip);
	ssh_options_set(session, SSH_OPTIONS_PORT, &port);
	ssh_options_set(session, SSH_OPTIONS_USER, username);

	rc = ssh_connect(session);
	if (rc != SSH_OK)
	{
		message_error(session,"connect to remote fail");
        goto FREE_SESSION;
	}

	rc = ssh_userauth_password(session, NULL,password);
	if (rc != SSH_AUTH_SUCCESS)
	{
		message_error(session,"Autentication fail");
        goto FREE_SESSION;
	}

	channel = ssh_channel_new(session);
	if (channel == NULL)
	{
		message_error(session,"create channel fail");
        goto FREE_SESSION;
	}

	rc = ssh_channel_open_session(channel);
	if (rc != SSH_OK)
	{
		message_error(session,"open channel fail");
        goto FREE_CHANNEL;
	}

    //if(env  && (rc = ssh_channel_request_exec(channel, env)) != SSH_OK)
    //{
	//	message_error(session,"executing remote enviroment fail");
    //   goto FREE_CHANNEL;
    //}

    snprintf(cmd_buf,4093,"%s %s",env,cmd);
	rc = ssh_channel_request_exec(channel, cmd_buf);
	if (rc != SSH_OK)
	{
		message_error(session,"executing remote command fail");
        goto FREE_CHANNEL;
	}

	//rc = ssh_channel_get_exit_status(channel);
	BE_LA_DEBUG("STDOUT Receiveed:");
	nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	while (nbytes > 0) {
		buffer[nbytes]='\0';
		BE_LA_LOG_INNER(BE_LA_LEVEL_DEBUG,"%s",buffer);
		//fwrite(buffer, 1, nbytes, stdout);
		nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	}

	BE_LA_DEBUG("STDERROR Receiveed:");
	nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 1);
	while (nbytes > 0) {
			buffer[nbytes]='\0';
			BE_LA_LOG_INNER(BE_LA_LEVEL_DEBUG,"%s",buffer);
			//fwrite(buffer, 1, nbytes, stdout);
			nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 1);
	}

    ret = 0;
FREE_CHANNEL:
	free_channel(channel);
FREE_SESSION:
	free_session(session);

	return ret;
}
