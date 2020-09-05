#include <wpdklib.h>
#include <sys/socket.h>
#include <fcntl.h>


const char *wpdk_get_path(const char *path)
{
	if (!path) return path;

	if (!strncmp(path, "/tmp/", 5))
		return path + 5;

	if (!strncmp(path, "/var/tmp/", 9))
		return path + 9;

	if (strrchr(path, '/') == path)
		return path + 1;

	return path;
}


int wpdk_open(const char *pathname, int flags, ...)
{
	mode_t mode = 0;
	va_list ap;

	if (flags & O_CREAT) {
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
	}

	return _open(wpdk_get_path(pathname), flags, mode);
}


int wpdk_fcntl(int fildes, int cmd, ...)
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
			break;

		case F_GETLK:
		case F_SETLK:
			break;
	}

	// HACK - fcntl is used on sockets to set nonblock
	return -1;
}
