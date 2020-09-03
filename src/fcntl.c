#include <sys/platform.h>
#include <sys/socket.h>
#include <fcntl.h>


int wpdk_is_socket(int fd);
int wpdk_socket_error();
SOCKET wpdk_get_socket(int fd);


int fcntl(int fildes, int cmd, ...)
{
	u_long mode;
	int rc, arg;
	va_list ap;
	SOCKET s;

	switch (cmd) {
		case F_GETFD:
		case F_SETFD:
			break;

		case F_GETFL:
			return 0;

		case F_SETFL:
			va_start(ap, cmd);
			arg = va_arg(ap, int);
			va_end(ap);

			if (wpdk_is_socket(fildes)) {
				mode = (arg & O_NONBLOCK) != 0;
				s = wpdk_get_socket(fildes);

				if (s == INVALID_SOCKET)
					return -1;

				rc = ioctlsocket(s, FIONBIO, &mode);

				if (rc == SOCKET_ERROR)
					return wpdk_socket_error();

				return 0;
			}

			// HACK - maybe use WSAIoctl
			// HACK - how to tell if this is a socket
			break;

		case F_GETLK:
		case F_SETLK:
			break;
	}

	// HACK - fcntl is used on sockets to set nonblock
	return -1;
}