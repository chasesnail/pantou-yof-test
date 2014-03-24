/*
 * yof_config.c
 *
 *  Created on: Mar 19, 2014
 *      Author: yiming
 */
#include <stdio.h>
#include "yof_config.h"
#include "yof_kernel.h"


static SRV_BUF_POOL g_astBufPool[] =
{
	{32,     500},
    {64,     500},
    {128,    500},
    {256,    500},
    {512,    500},
    {1024,   200},
    {2048,   200},
    {4096,   200},
    {8192,   200},
    {16384,  50},
    {32768,  50},
    {65536,  10},
    {131072, 10},
    {262144, 5}
};

static SRV_TASK g_astTaskTbl[] =
{
	{TID_KERNEL,       	"KERNEL",		kernel_entry,		NULL, SRV_TASK_NORMAL_PRIO, SRV_MSQ_ENABLE},
	{TID_UDPSEND,       "UDPSEND",		udpsend_entry,		NULL, SRV_TASK_NORMAL_PRIO, SRV_MSQ_ENABLE},
	{TID_UDPRECV, 		"UDPRECV",		udprecv_entry,		NULL, SRV_TASK_NORMAL_PRIO, SRV_MSQ_ENABLE}

//	{TID_BRCUMSGRECV,"BRCRECV",	 brc_umsg_entry, }
};

static SRV_TRACE_MODULE g_astTraceMdl[] =
{
	{MID_UDPSEND, "UDPSEND", 1},
	{MID_UDPRECV, "UDPRECV", 1}
};

static SRV_TRACE_LEVEL g_astTraceLvl[] =
{
	{LID_INFO, "INFO", 1},
	{LID_WARNING, "WARNING", 1},
	{LID_ERROR, "ERROR", 1}
};


s32 yof_init()
{
	s32 ret;

	ret = srv_buf_init(g_astBufPool, sizeof(g_astBufPool)/sizeof(SRV_BUF_POOL));
	if(SRV_OK != ret)
	{
		printf("Buffer init failed.\r\n");
		return SRV_ERR;
	}

	ret = srv_skt_init();
	if(SRV_OK != ret)
	{
		printf("Socket init failed.\r\n");
		return SRV_ERR;
	}

	ret = srv_trace_init(g_astTraceMdl, sizeof(g_astTraceMdl) / sizeof(SRV_TRACE_MODULE),
						   g_astTraceLvl, sizeof(g_astTraceLvl) / sizeof(SRV_TRACE_LEVEL));
	if(SRV_OK != ret)
	{
		printf("Trace init failed.\r\n");
		return SRV_ERR;
	}

	ret = srv_task_init(TID_MAX);
	if(SRV_OK != ret)
	{
		printf("Task init failed.\r\n");
		return SRV_ERR;
	}

	ret = srv_task_register_table(g_astTaskTbl, sizeof(g_astTaskTbl)/sizeof(SRV_TASK));
	if(SRV_OK != ret)
	{
		printf("Task register failed.\r\n");
		return SRV_ERR;
	}

	 ret = srv_task_start_table();
	 if(SRV_OK != ret)
	 {
		 printf("Task start failed.\r\n");
		 return SRV_ERR;
	 }

	return SRV_OK;
}


void* yof_alloc(u32 ulMsgLen)
{
	void *pMsg;
	YOF_MSG_HDR *pstCsptMsgHdr;

	pMsg = srv_msg_alloc(ulMsgLen + YOF_MSG_HDR_LEN);
	if(NULL == pMsg)
	{
		return NULL;
	}

	pstCsptMsgHdr = (YOF_MSG_HDR *)pMsg;
	pstCsptMsgHdr->ulType = YOF_MSG_TYPE_INTERNAL;
	pstCsptMsgHdr->ulLen = ulMsgLen;
	pstCsptMsgHdr->pMsg = (s8 *)pMsg + YOF_MSG_HDR_LEN;

	return pMsg;
}

void yof_free(void* pMsg)
{
	if(NULL == pMsg)
	{
		return;
	}
	srv_msg_free(pMsg);
}

s32 yof_snd(s32 lStid, s32 lDtid, s32 lMsgPrio, void *pMsg)
{
	if(NULL == pMsg)
	{
		return SRV_ERR;
	}

	if(SRV_OK == srv_msg_snd(lStid,lDtid,lMsgPrio,1,pMsg))
	{
		return SRV_OK;
	}
	else
	{
		return SRV_ERR;
	}
}

s32 yof_rcv(s32 lTid,void** ppMsg)
{
	s32 lMsgId;
	if(SRV_ERR == srv_msg_rcv(lTid,&lMsgId,ppMsg))//lMsgId no use
	{
		return SRV_ERR;
	}
	else
	{
		return SRV_OK;
	}
}
