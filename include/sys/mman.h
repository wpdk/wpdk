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

#ifndef _WPDK_SYS_MMAN_H_
#define _WPDK_SYS_MMAN_H_

#include <wpdk/header.h>
#include <wpdk/types.h>

_WPDK_BEGIN_C_HEADER

#define PROT_NONE	0
#define PROT_READ	1
#define PROT_EXEC	2
#define PROT_WRITE	4

#define MAP_SHARED		0x01
#define MAP_PRIVATE		0x02
#define MAP_FIXED		0x04
#define MAP_ANONYMOUS	0x08

#define MAP_FAILED ((void *) -1)

void *wpdk_mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
int wpdk_munmap(void *addr, size_t len);
int wpdk_shm_open(const char *name, int oflag, mode_t mode);
int wpdk_shm_unlink(const char *name);

#ifndef _WPDK_BUILD_LIB_
#define mmap(ad,len,prot,flags,fd,off) wpdk_mmap(ad,len,prot,flags,fd,off)
#define munmap(ad,len) wpdk_munmap(ad,len)
#define shm_open(name,oflag,mode) wpdk_shm_open(name,oflag,mode)
#define shm_unlink(name) wpdk_shm_unlink(name)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_SYS_MMAN_H_ */
