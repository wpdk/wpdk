#include <wpdklib.h>
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


int wpdk_get_epoll(int fd)
{
	int id = -1;

	if (wpdk_epoll_fds && wpdk_is_epoll(fd))
		id = fd - epollbase;
		
	if (id == -1 || wpdk_epoll_fds[id] == NULL) {
		_set_errno(EBADF);
		return -1;
	}

	return id;
}


int wpdk_allocate_epoll()
{
	struct epoll **fds, *ep;
	int i;

	if (!wpdk_epoll_fds) {
		fds = calloc(maxepolls, sizeof(struct epoll *));

		if (fds == NULL) {
			_set_errno(ENOMEM);
			return -1;
		}

		if (InterlockedCompareExchangePointer((void **)&wpdk_epoll_fds, fds, NULL) != NULL)
			free(fds);
	}

	ep = calloc(1, sizeof(struct epoll));

	if (ep == NULL) {
		_set_errno(ENOMEM);
		return -1;
	}

	for (i = 0; i < FD_SETSIZE; i++)
		ep->socket[i] = INVALID_SOCKET;

	for (i = 0; i < maxepolls; i++)
		if (wpdk_epoll_fds[i] == NULL)
			if (InterlockedCompareExchangePointer((void **)&wpdk_epoll_fds[i], ep, NULL) == NULL)
				return epollbase + i;

	free(ep);
	_set_errno(EMFILE);
	return -1;
}


int epoll_create1(int flags)
{
	int id = wpdk_allocate_epoll();

	if (id == -1)
		return -1;

	wpdk_epoll_fds[id - epollbase]->flags = flags;
	return id;
}


int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
	SOCKET socket = wpdk_get_socket(fd); 
	int i, id = wpdk_get_epoll(epfd);
	struct epoll *ep;

	if (id == -1 || socket == INVALID_SOCKET)
		return -1;

	// HACK - race with close
	ep = wpdk_epoll_fds[id];

	switch (op) {
		case EPOLL_CTL_ADD:
			if (event == NULL) {
				_set_errno(EINVAL);
				return -1;
			}

			for (i = 0; i < ep->maxcount; i++)
				if (ep->socket[i] == socket) {
					_set_errno(EEXIST);
					return -1;
				}

			for (i = 0; i < FD_SETSIZE; i++)
				if (ep->socket[i] == INVALID_SOCKET)
					break;

			if (i >= FD_SETSIZE) {
				_set_errno(ENOSPC);
				return -1;
			}

			if (i >= ep->maxcount)
				ep->maxcount = i + 1;

			ep->socket[i] = socket;
			ep->event[i] = *event;
			return 0;

		case EPOLL_CTL_DEL:
			for (i = 0; i < ep->maxcount; i++)
				if (ep->socket[i] == socket)
					break;

			if (i >= ep->maxcount) {
				_set_errno(ENOENT);
				return -1;
			}

			if (i == ep->maxcount - 1)
				ep->maxcount--;

			ep->socket[i] = INVALID_SOCKET;
			return 0;		
	}	

	_set_errno(EINVAL);
	return -1;
}


int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
	struct timeval delay = { 0, 0 };
	int id = wpdk_get_epoll(epfd);
	fd_set readfds, exceptfds;
	int i, n, rc, rd, ex;
	struct epoll *ep;
	int worktodo = 0;

	if (id == -1)
		return -1;

	// HACK - timeout not supported
	if (timeout || !events || maxevents < 1) {
		_set_errno(EINVAL);
		return -1;
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
		return -1;

	ep = (struct epoll *)InterlockedExchangePointer((void **)&wpdk_epoll_fds[id], NULL);
	
	if (ep == NULL) {
		_set_errno(EBADF);
		return -1;
	}

	free(ep);
	return 0;
}
