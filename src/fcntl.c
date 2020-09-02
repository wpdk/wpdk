#include <sys/platform.h>
#include <sys/socket.h>
#include <fcntl.h>

// HACK - very rough sketch

void wpdk_seterrno_wsa(int wsaerr);


int fcntl(int fildes, int cmd, ...)
{
	u_long mode;
	int rc, arg;
	va_list ap;

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

			// HACK - maybe use WSAIoctl
			// HACK - how to tell if this is a socket

			mode = (arg & O_NONBLOCK) != 0;
			rc = ioctlsocket((SOCKET)fildes, FIONBIO, &mode);

			if (rc == SOCKET_ERROR) {
				wpdk_seterrno_wsa(WSAGetLastError());
				return -1;
			}

			return 0;

		case F_GETLK:
		case F_SETLK:
			break;
	}

	// HACK - fcntl is used on sockets to set nonblock
	return -1;
}