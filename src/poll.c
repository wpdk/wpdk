#include <wpdklib.h>
#include <poll.h>


int poll(struct pollfd fds[], nfds_t nfds, int timeout)
{
	fd_set readfds, writefds, exceptfds;
	struct timeval delay = { 0, 0 };
	int worktodo = 0;
	SOCKET socket;
	int i, n, rc;

	if (!fds) {
		_set_errno(EINVAL);
		return -1;
	}

	delay.tv_sec = timeout / 1000;
	delay.tv_usec = (timeout % 1000) * 1000;

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);

	for (i = 0; i < nfds; i++) {
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

	rc = (worktodo) ? select(nfds, &readfds, &writefds, &exceptfds, &delay) : 0;

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	for (n = i = 0; i < nfds; i++) {
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