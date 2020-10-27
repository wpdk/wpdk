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

#ifndef _WPDK_POLL_H_
#define _WPDK_POLL_H_

#include <wpdk/header.h>

_WPDK_BEGIN_C_HEADER

#define POLLRDNORM  0x0100
#define POLLRDBAND  0x0200
#define POLLIN      (POLLRDNORM | POLLRDBAND)
#define POLLPRI     0x0400

#define POLLWRNORM  0x0010
#define POLLOUT     (POLLWRNORM)
#define POLLWRBAND  0x0020

#define POLLERR     0x0001
#define POLLHUP     0x0002
#define POLLNVAL    0x0004

struct pollfd {
	int		fd;
	short	events;
	short	revents;
};

typedef	unsigned int nfds_t;

int wpdk_poll(struct pollfd fds[], nfds_t nfds, int timeout);

#ifndef _WPDK_BUILD_LIB_
static inline int poll(struct pollfd fds[], nfds_t nfds, int timeout) {
	return wpdk_poll(fds, nfds, timeout);
}
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_POLL_H_ */
