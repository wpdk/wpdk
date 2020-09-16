/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2020, MayaData Inc. All rights reserved.
 *  Copyright (c) 2020, DataCore Software Corporation. All rights reserved.
 * 
 *  POSIX details are based on the Open Group Base Specification Issue 7,
 *  2018 edition at https://pubs.opengroup.org/onlinepubs/9699919799/
 * 
 *  Details about Linux extensions are based on the Linux man-pages project
 *  at https://www.kernel.org/doc/man-pages/
 */

#ifndef _WPDK_SEMAPHORE_H_
#define	_WPDK_SEMAPHORE_H_

#include <wpdk/header.h>
#include <time.h>

_WPDK_BEGIN_C_HEADER

typedef struct { HANDLE h; } sem_t;

int wpdk_sem_init(sem_t *sem, int pshared, unsigned int value);
int wpdk_sem_destroy(sem_t *sem);
int wpdk_sem_post(sem_t *sem);
int wpdk_sem_wait(sem_t *sem);
int wpdk_sem_trywait(sem_t *sem);
int wpdk_sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

#ifndef _WPDK_BUILD_LIB_
#define sem_init(sem,pshared,value) wpdk_sem_init(sem,pshared,value)
#define sem_destroy(sem) wpdk_sem_destroy(sem)
#define sem_post(sem) wpdk_sem_post(sem)
#define sem_wait(sem) wpdk_sem_wait(sem)
#define sem_trywait(sem) wpdk_sem_trywait(sem)
#define sem_timedwait(sem,to) wpdk_sem_timedwait(sem,to)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_SEMAPHORE_H_ */
