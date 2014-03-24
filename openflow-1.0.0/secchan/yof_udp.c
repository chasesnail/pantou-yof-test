/*
 * yof_udp.c
 *
 *  Created on: Mar 18, 2014
 *      Author: yiming
 */

#include <stdio.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include "yof_udp.h"
#include "yof_config.h"

s32 g_EpollFd;


s32 create_bind_socket(u32 ulIp, u16 usPort)
{
	s32 lSock;
	struct sockaddr_in addr;

	lSock = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == lSock)
	{
		return SRV_INVALID;
	}

	addr.sin_family = AF_INET;
	if(0 == ulIp)
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		addr.sin_addr.s_addr = htonl(ulIp);
	}
	addr.sin_port = htons(usPort);
	if (-1 == bind(lSock,(struct sockaddr*)&addr,sizeof(addr)))
	{
		close(lSock);
		return SRV_INVALID;
	}

	return lSock;
}

void udp_recv_msg() {
	s8* pBuf = NULL;
	struct epoll_event events[MAX_UDP_RECV_NUM];
	struct sockaddr_in peerAddr;
	socklen_t peerAddrLen = sizeof(peerAddr);
	s32 num, i, size;

	while (1) {
		if (NULL != g_EpollFd) {
			printf("get in wait %d \n", g_EpollFd);
			num = epoll_wait(g_EpollFd, events, MAX_UDP_RECV_NUM, -1);
			if (num <= 0) {
				continue;
			}
			for (i = 0; i < num; i++) {
				if (events[i].events & EPOLLIN) {
					pBuf = yof_alloc(MAX_UDP_LEN);
					if (NULL == pBuf) {
						printf("%s-%u:yof_alloc failed\r\n", __FILE__,
								__LINE__);
						continue;
					}

					size = recvfrom(events[i].data.fd, pBuf + YOF_MSG_HDR_LEN,
							MAX_UDP_LEN, 0, (struct sockaddr*) &peerAddr,
							&peerAddrLen);
					printf("waiting size is %d \n", size);
					if (size > 0) {
						((YOF_MSG_HDR *) pBuf)->ulLen = size;
						((YOF_MSG_HDR *) pBuf)->ulType = YOF_MSG_TYPE_UDP;
						((YOF_MSG_HDR *) pBuf)->hdr.stUdpHdr.ulIP = ntohl(
								peerAddr.sin_addr.s_addr);
						((YOF_MSG_HDR *) pBuf)->hdr.stUdpHdr.usPort = ntohs(
								peerAddr.sin_port);
						((YOF_MSG_HDR *) pBuf)->hdr.stUdpHdr.lSkt =
								events[i].data.fd;

						if (SRV_OK
								!= yof_snd(TID_UDPRECV, TID_KERNEL,
										SRV_MSG_NORMAL_PRIO, pBuf)) {
							printf("%s-%u:yof_snd failed\r\n", __FILE__,
									__LINE__);
							yof_free(pBuf);
						}
					} else {
						yof_free(pBuf);
					}
				}
			}
		}
	}
}

void* udprecv_entry(void* arg) {
	struct epoll_event ev;
	s32 lSkt;
	printf("%s-%u:Enter udp_entry\r\n", __FILE__, __LINE__);
	lSkt = create_bind_socket(0,65500);

	if (0 == g_EpollFd || -1 == g_EpollFd) {
		g_EpollFd = epoll_create(MAX_UDP_RECV_NUM);
		if (-1 == g_EpollFd) {
			printf("%s-%u:epoll create failed.\r\n", __FILE__, __LINE__);
			return SRV_ERR;
		}

		ev.data.fd = lSkt;
		if (epoll_ctl(g_EpollFd, EPOLL_CTL_ADD, lSkt, &ev) == -1) {
			printf("%s-%u:epoll add failed\r\n", __FILE__, __LINE__);
			return SRV_ERR;
		}
	}
	udp_recv_msg();
	return NULL;
}

void* udpsend_entry(void* arg) {
	printf("%s-%u:Enter udpsend_entry\r\n", __FILE__, __LINE__);
	s32 lMsgId;
	void *pMsg;
	s32 lMsgSize;
	while (1) {
		lMsgSize = srv_msg_rcv(TID_UDPSEND, &lMsgId, &pMsg);
		printf("%s-%u:Enter udpsend_entry\r\n");
	}
}

