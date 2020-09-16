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

#ifndef _WPDK_SYS_EPOLL_H_
#define _WPDK_SYS_EPOLL_H_

#include <sys/cdefs.h>
#include <sys/_types.h>

_WPDK_BEGIN_C_HEADER

typedef union epoll_data {
	void		*ptr;
	int			fd;
	uint32_t	u32;
	uint64_t	u64;
} epoll_data_t;

struct epoll_event {
	uint32_t		events;
	epoll_data_t	data;
};

#define EPOLL_CTL_ADD	1
#define EPOLL_CTL_DEL	2

#define EPOLLIN			1
#define EPOLLERR		4

int wpdk_epoll_create1(int flags);
int wpdk_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int wpdk_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

#ifndef _WPDK_BUILD_LIB_
#define epoll_create1(flags) wpdk_epoll_create1(flags)
#define epoll_ctl(epfd,op,fd,ev) wpdk_epoll_ctl(epfd,op,fd,ev)
#define epoll_wait(epfd,ev,max,to) wpdk_epoll_wait(epfd,ev,max,to)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_SYS_EPOLL_H_ */
