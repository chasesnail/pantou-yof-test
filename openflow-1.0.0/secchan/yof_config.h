/*
 * yof_config.h
 *
 *  Created on: Mar 19, 2014
 *      Author: yiming
 */

#ifndef YOF_CONFIG_H_
#define YOF_CONFIG_H_

#include "srv_common.h"
#include "srv_buffer.h"
#include "srv_task.h"
#include "srv_trace.h"
#include "srv_socket.h"
#include "yof_udp.h"

enum YOF_MSG_TYPE_
{
	YOF_MSG_TYPE_INTERNAL,
	YOF_MSG_TYPE_UDP
};

typedef struct UDP_HDR_
{
	u32 ulIP;
	u16 usPort;
	s32 lSkt;
}UDP_HDR;

typedef struct YOF_MSG_HDR_
{
	u32 ulType;
	u32 ulLen;
	union
	{
		UDP_HDR stUdpHdr;
	}hdr;
	void *pMsg;
}YOF_MSG_HDR;

#define YOF_MSG_HDR_LEN  sizeof(YOF_MSG_HDR)

enum YOF_TID
{
	TID_KERNEL,
	TID_UDPSEND,
	TID_UDPRECV,
	TID_MAX
};

enum YOF_TRACE_MID
{
	MID_UDPSEND,
	MID_UDPRECV,
	MID_MAX
};

enum YOF_TRACE_LID
{
	LID_INFO,
	LID_WARNING,
	LID_ERROR,
	LID_MAX
};


s32 yof_init();

void* yof_alloc(u32 ulMsgLen);

void yof_free(void* pMsg);

s32 yof_snd(s32 lStid, s32 lDtid, s32 lMsgPrio, void *pMsg);

s32 yof_rcv(s32 lTid,void** ppMsg);


#endif /* YOF_CONFIG_H_ */
