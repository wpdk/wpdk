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
#include <sys/epoll.h>
#include <sys/time.h>
#include <stdlib.h>
#include <poll.h>


struct epoll {
	int		flags;
	int		start;
	int		maxcount;
	SOCKET	socket[FD_SETSIZE];
	struct epoll_event event[FD_SETSIZE];
};

struct epoll **wpdk_epoll_fds;

static const int maxepolls = 512;
static const int epollbase = 0x20000;


// HACK - race conditions if multiple changes can occur concurrently
// HACK - or if create / close can co-exist.

// HACK - need to stop polling on close socket


int wpdk_is_epoll(int fd)
{
	return (epollbase <= fd && fd < epollbase + maxepolls);
}


static int
wpdk_get_epoll(int fd)
{
	int id = -1;

	if (wpdk_epoll_fds && wpdk_is_epoll(fd))
		id = fd - epollbase;
		
	if (id == -1 || wpdk_epoll_fds[id] == NULL)
		return -1;

	return id;
}


static int
wpdk_allocate_epoll()
{
	struct epoll **fds, *ep;
	int i;

	if (!wpdk_epoll_fds) {
		fds = calloc(maxepolls, sizeof(struct epoll *));

		if (fds == NULL)
			return wpdk_posix_error(ENOMEM);

		if (InterlockedCompareExchangePointer((void **)&wpdk_epoll_fds, fds, NULL) != NULL)
			free(fds);
	}

	ep = calloc(1, sizeof(struct epoll));

	if (ep == NULL)
		return wpdk_posix_error(ENOMEM);

	for (i = 0; i < FD_SETSIZE; i++)
		ep->socket[i] = INVALID_SOCKET;

	for (i = 0; i < maxepolls; i++)
		if (wpdk_epoll_fds[i] == NULL)
			if (InterlockedCompareExchangePointer((void **)&wpdk_epoll_fds[i], ep, NULL) == NULL)
				return epollbase + i;

	free(ep);
	return wpdk_posix_error(EMFILE);
}


int wpdk_epoll_create1(int flags)
{
	int id = wpdk_allocate_epoll();

	if (id == -1)
		return -1;

	wpdk_epoll_fds[id - epollbase]->flags = flags;
	return id;
}


int wpdk_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
	SOCKET socket = wpdk_get_socket(fd); 
	int i, id = wpdk_get_epoll(epfd);
	struct epoll *ep;

	if (id == -1 || socket == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	// HACK - race with close
	ep = wpdk_epoll_fds[id];

	switch (op) {
		case EPOLL_CTL_ADD:
			if (event == NULL)
				return wpdk_posix_error(EINVAL);

			for (i = 0; i < ep->maxcount; i++)
				if (ep->socket[i] == socket)
					return wpdk_posix_error(EEXIST);

			for (i = 0; i < FD_SETSIZE; i++)
				if (ep->socket[i] == INVALID_SOCKET)
					break;

			if (i >= FD_SETSIZE)
				return wpdk_posix_error(ENOSPC);

			if (i >= ep->maxcount)
				ep->maxcount = i + 1;

			ep->socket[i] = socket;
			ep->event[i] = *event;
			return 0;

		case EPOLL_CTL_DEL:
			for (i = 0; i < ep->maxcount; i++)
				if (ep->socket[i] == socket)
					break;

			if (i >= ep->maxcount)
				return wpdk_posix_error(ENOENT);

			if (i == ep->maxcount - 1)
				ep->maxcount--;

			ep->socket[i] = INVALID_SOCKET;
			return 0;		
	}	

	return wpdk_posix_error(EINVAL);
}


int wpdk_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
	struct WSAtimeval delay = { 0, 0 };
	int id = wpdk_get_epoll(epfd);
	fd_set readfds, exceptfds;
	int i, n, rc, rd, ex;
	struct epoll *ep;
	int worktodo = 0;

	if (id == -1)
		return wpdk_posix_error(EBADF);

	// HACK - timeout not supported
	if (timeout || !events || maxevents < 1) {
		WPDK_UNIMPLEMENTED();
		return wpdk_posix_error(EINVAL);
	}

	// HACK - race with close
	ep = wpdk_epoll_fds[id];

	FD_ZERO(&readfds);
	FD_ZERO(&exceptfds);

	for (i = 0; i < ep->maxcount; i++) {
		if (ep->event[i].events & EPOLLIN) {
			FD_SET(ep->socket[i], &readfds);
			worktodo = 1;
		}

		if (ep->event[i].events & EPOLLERR) {
			FD_SET(ep->socket[i], &exceptfds);
			worktodo = 1;
		}
	}

	if (!worktodo) return 0;

	rc = select(ep->maxcount, &readfds, NULL, &exceptfds, &delay);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	for (i = ep->start, n = 0; i < ep->maxcount && n < maxevents && n < rc; i++) {
		if (ep->socket[i] == INVALID_SOCKET) continue;

		rd = FD_ISSET(ep->socket[i], &readfds);
		ex = FD_ISSET(ep->socket[i], &exceptfds);

		if (rd || ex) {
			events[n].data = ep->event[i].data;
			events[n++].events = (rd ? EPOLLIN : 0) | (ex ? EPOLLERR : 0);
		}
	}

	for (i = 0; i < ep->start && n < maxevents && n < rc; i++) {
		if (ep->socket[i] == INVALID_SOCKET) continue;

		rd = FD_ISSET(ep->socket[i], &readfds);
		ex = FD_ISSET(ep->socket[i], &exceptfds);

		if (rd || ex) {
			events[n].data = ep->event[i].data;
			events[n++].events = (rd ? EPOLLIN : 0) | (ex ? EPOLLERR : 0);
		}
	}

	if (rc > n) ep->start++;
	return n;
}


int wpdk_close_epoll(int fd)
{
	int id = wpdk_get_epoll(fd);
	struct epoll *ep;

	if (id == -1)
		return wpdk_posix_error(EBADF);

	ep = (struct epoll *)InterlockedExchangePointer((void **)&wpdk_epoll_fds[id], NULL);
	
	if (ep == NULL)
		return wpdk_posix_error(EBADF);

	free(ep);
	return 0;
}
