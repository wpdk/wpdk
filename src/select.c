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

#include <wpdk/internal.h>
#include <sys/select.h>


// HACK - fd_set should contain SOCKETs

int
wpdk_select(int nfds, fd_set *readfds, fd_set *writefds,
		fd_set *exceptfds, struct timeval *timeout)
{
	struct WSAtimeval delay;
	int rc;

	if (timeout) {
		delay.tv_sec = (timeout->tv_sec > LONG_MAX) ?
						LONG_MAX : (long)timeout->tv_sec;
		delay.tv_usec = timeout->tv_usec;
	}

	rc = select(nfds, readfds, writefds, exceptfds,
		(timeout ? &delay : NULL));

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return rc;
}
