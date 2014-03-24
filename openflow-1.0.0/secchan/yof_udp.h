/*
 * yof_udp.h
 *
 *  Created on: Mar 18, 2014
 *      Author: yiming
 */

#ifndef YOF_UDP_H_
#define YOF_UDP_H_


#include "srv_common.h"

#define MAX_UDP_LEN     		1500
#define MAX_UDP_SEND_NUM		1000
#define MAX_UDP_RECV_NUM		1000

void* udprecv_entry(void* arg);

void* udpsend_entry(void* arg);

void udp_recv_msg();



#endif /* YOF_UDP_H_ */
