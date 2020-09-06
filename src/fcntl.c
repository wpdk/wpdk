#include <wpdklib.h>
#include <sys/socket.h>
#include <fcntl.h>

static char *msys = "c:\\tools\\msys64";


const char *wpdk_get_path(const char *path, char *buffer, size_t len)
{
	if (!path) return path;

	// HACK - wpdk_get_path fix pathnames and handle length

	if (!strncmp(path, "/tmp/", 5) || !strncmp(path, "/var/tmp/", 9)) {
		strcpy(buffer, msys);
		strcat(buffer, path);
		return buffer;
	}

	if (strrchr(path, '/') == path)	{
		strcpy(buffer, msys);
		strcat(buffer, "/tmp");
		strcat(buffer, path);
		return buffer;
	}

	return path;
}


int wpdk_open(const char *pathname, int flags, ...)
{
	char buf[MAX_PATH];
	mode_t mode = 0;
	va_list ap;

	if (flags & O_CREAT) {
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
	}

	return _open(wpdk_get_path(pathname, buf, sizeof(buf)), flags, mode);
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

	return -1;
}
