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
#include <fcntl.h>


const char *wpdk_get_path(const char *path, char *buffer, size_t len)
{
	// HACK - hard-coded local path
	static char *msys = "c:\\tools\\msys64";

	if (!path) return path;

	// HACK - wpdk_get_path fix pathnames and handle length
	UNREFERENCED_PARAMETER(len);

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
		mode = (mode_t)va_arg(ap, int);
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
					return wpdk_last_wsa_error();

				return 0;
			}

			// HACK - maybe use WSAIoctl
			break;

		case F_GETLK:
		case F_SETLK:
			break;
	}

	WPDK_UNIMPLEMENTED();
	return -1;
}
