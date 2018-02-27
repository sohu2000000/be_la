/*
 * ssh_remote_exec.h
 *
 *  Created on: Jan 8, 2018
 *      Author: anlang
 */

#ifndef COMMON_SSH_REMOTE_EXEC_H_
#define COMMON_SSH_REMOTE_EXEC_H_

int ssh_remote_exec(char*remote_ip, char*username,int port, char*password,char*env, char*cmd);

#endif /* COMMON_SSH_REMOTE_EXEC_H_ */
