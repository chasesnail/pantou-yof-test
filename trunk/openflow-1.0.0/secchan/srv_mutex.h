/*
 * srv_mutex.h
 *
 *  Created on: Mar 18, 2014
 *      Author: yiming
 */

#ifndef SRV_MUTEX_H_
#define SRV_MUTEX_H_

#include "srv_common.h"

SRV_HANDLE srv_mutex_create();
s32 srv_mutex_destroy(SRV_HANDLE hMutex);

s32 srv_mutex_lock(SRV_HANDLE hMutex);
s32 srv_mutex_unlock(SRV_HANDLE hMutex);

#endif /* SRV_MUTEX_H_ */
