/*
 * srv_buffer.h
 *
 *  Created on: Mar 18, 2014
 *      Author: yiming
 */

#ifndef SRV_BUFFER_H_
#define SRV_BUFFER_H_

#include "srv_common.h"

typedef struct SRV_BUF_POOL_
{
    s32 blocksize; // size of block
    s32 blocknr;   // number of blocks
}SRV_BUF_POOL;

s32 srv_buf_init(const SRV_BUF_POOL* pstBufPool, s32 poolsize);

void* srv_buf_alloc(s32 size);
void srv_buf_free(void* buf);

#endif /* SRV_BUFFER_H_ */
