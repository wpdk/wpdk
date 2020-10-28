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
#include <sys/socket.h>
#include <poll.h>


int
wpdk_poll(struct pollfd fds[], nfds_t nfds, int timeout)
{
	fd_set readfds, writefds, exceptfds;
	struct WSAtimeval delay = { 0, 0 };
	int worktodo = 0;
	SOCKET socket;
	int n, rc;
	nfds_t i;

	if (!fds || nfds >= FD_SETSIZE)
		return wpdk_posix_error(EINVAL);

	delay.tv_sec = timeout / 1000;
	delay.tv_usec = (timeout % 1000) * 1000;

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);

	for (i = 0; i < nfds && i < FD_SETSIZE; i++) {
		fds[i].revents = 0;
		if (fds[i].fd < 0) continue;

		socket = wpdk_get_socket(fds[i].fd);

		if (socket == INVALID_SOCKET) {
			fds[i].revents |= POLLNVAL;
			continue;
		}

		if (fds[i].events & POLLIN) {
			FD_SET(socket, &readfds);
			worktodo = 1;
		}

		if (fds[i].events & POLLOUT) {
			FD_SET(socket, &writefds);
			worktodo = 1;
		}

		if (fds[i].events & POLLPRI) {
			FD_SET(socket, &exceptfds);
			worktodo = 1;
		}
	}

	rc = (worktodo) ? select(nfds, &readfds, &writefds, &exceptfds, (timeout != -1) ? &delay : NULL) : 0;

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	for (n = i = 0; i < nfds && i < FD_SETSIZE; i++) {
		if (fds[i].fd < 0) continue;

		socket = wpdk_get_socket(fds[i].fd);

		if (socket != INVALID_SOCKET) {
			if (FD_ISSET(socket, &readfds))
				fds[i].revents |= POLLIN;

			if (FD_ISSET(socket, &writefds))
				fds[i].revents |= POLLOUT;

			if (FD_ISSET(socket, &exceptfds))
				fds[i].revents |= (POLLERR | POLLHUP | POLLPRI);
		}

		if (fds[i].revents) n++;
	}

	return n;
}
