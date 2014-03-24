/*
 * yof_kernel.c
 *
 *  Created on: Mar 22, 2014
 *      Author: yiming
 */
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include "yof_kernel.h"
#include "yof_config.h"

s32          g_EpollFd;

void* kernel_entry(void* arg){
	printf("%s-%u:Enter kernel_entry\r\n",__FILE__,__LINE__);
	s32 lMsgId;
	void *pMsg;
	s32 lMsgSize;
	while(1)
	{
		lMsgSize = srv_msg_rcv(TID_KERNEL,&lMsgId,&pMsg);
	}
}
