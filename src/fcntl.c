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
#include <sys/locking.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>
#include <io.h>


// HACK - hard-coded local path
static char msys[] = "c:\\tools\\msys64";


const char *
wpdk_get_path(const char *path, char *buffer, size_t len)
{
	size_t pathlen;

	if (!path || !*path) return path;

	wpdk_set_invalid_handler();

	pathlen = strnlen(path, PATH_MAX);

	if (pathlen == 0 || pathlen >= PATH_MAX)
		return NULL;

	if (!strncmp(path, "/tmp/", 5) || !strncmp(path, "/var/tmp/", 9))
		return (strcpy_s(buffer, len, msys) == 0
			&& strcat_s(buffer, len, path) == 0) ? buffer : NULL;

	if (strrchr(path, '/') == path)
		return (strcpy_s(buffer, len, msys) == 0
			&& strcat_s(buffer, len, "/tmp") == 0
			&& strcat_s(buffer, len, path) == 0) ? buffer : NULL;

	return path;
}


char *
wpdk_copy_path(char *buffer, size_t len, const char *path)
{
	const char *cp = wpdk_get_path(path, buffer, len);

	if (cp && cp == path)
		if (strcpy_s(buffer, len, path) != 0)
			return NULL;

	return buffer;
}


int
wpdk_open(const char *pathname, int flags, ...)
{
	char buf[MAX_PATH];
	mode_t m, mode = 0;
	const char *path;
	va_list ap;

	wpdk_set_invalid_handler();

	if (flags & O_CREAT) {
		va_start(ap, flags);
		m = (mode_t)va_arg(ap, int);
		if (m & (S_IRUSR|S_IRGRP|S_IROTH))
			mode |= S_IREAD;
		if (m & (S_IWUSR|S_IWGRP|S_IWOTH))
			mode |= S_IWRITE;
		va_end(ap);
	}

	flags &= (O_RDONLY|O_WRONLY|O_RDWR|O_APPEND|O_CREAT|O_TRUNC|O_EXCL);
	flags |= _O_BINARY;

	if ((path = wpdk_get_path(pathname, buf, sizeof(buf))) == NULL)
		return wpdk_posix_error(EINVAL);

	return _open(path, flags, mode);
}


static int
wpdk_fcntl_lockfile(int fildes, int cmd, struct flock *flockp)
{
	int rc, type = flockp->l_type;
	off_t start, nbytes;

	wpdk_set_invalid_handler();

	if (!flockp)
		return wpdk_posix_error(EINVAL);

	if (type != F_RDLCK && type != F_WRLCK && type != F_UNLCK)
		return wpdk_posix_error(EINVAL);

	/*
	 *  Calculate the required range
	 */
	if (wpdk_lockfile_get_range(fildes, flockp->l_whence,
			flockp->l_start, flockp->l_len, &start, &nbytes) == -1)
		return -1;

	/*
	 *  Issue locking request.
	 */
	switch (cmd) {
		case F_GETLK:
			if (type == F_UNLCK)
				return wpdk_posix_error(EINVAL);

			if ((rc = wpdk_lockfile(type, fildes, start, nbytes, true)) != -1) {
				wpdk_lockfile(F_UNLCK, fildes, start, nbytes, false);
				flockp->l_type = F_UNLCK;
			}

			flockp->l_pid = -1;
			return 0;

		case F_SETLK:
			return wpdk_lockfile(type, fildes, start, nbytes, true);

		case F_SETLKW:
			return wpdk_lockfile(type, fildes, start, nbytes, false);
	}

	return wpdk_posix_error(EINVAL);
}


int
wpdk_fcntl(int fildes, int cmd, ...)
{
	struct flock *flockp;
	va_list ap;
	int arg;

	wpdk_set_invalid_handler();

	switch (cmd) {
		/*
		 *  File descriptor flags. FD_CLOEXEC is the only valid
		 *  flag and it is unimplemented on Windows.
		 */
		case F_GETFD:
		case F_SETFD:

			if ((HANDLE)_get_osfhandle(fildes) == INVALID_HANDLE_VALUE)
				return wpdk_posix_error(EBADF);

			WPDK_UNIMPLEMENTED();
			return 0;

		/*
		 *  File status flags.
		 */
		case F_GETFL:

			if (wpdk_is_socket(fildes))
				return wpdk_socket_fcntl(fildes, cmd, 0);

			if ((HANDLE)_get_osfhandle(fildes) == INVALID_HANDLE_VALUE)
				return wpdk_posix_error(EBADF);

			WPDK_UNIMPLEMENTED();
			return 0;

		case F_SETFL:

			va_start(ap, cmd);
			arg = va_arg(ap, int);
			va_end(ap);

			if (wpdk_is_socket(fildes))
				return wpdk_socket_fcntl(fildes, cmd, arg);

			if ((HANDLE)_get_osfhandle(fildes) == INVALID_HANDLE_VALUE)
				return wpdk_posix_error(EBADF);

			WPDK_UNIMPLEMENTED();
			return 0;

		/*
		 *  File lock request
		 */
		case F_GETLK:
		case F_SETLK:
		case F_SETLKW:

			va_start(ap, cmd);
			flockp = va_arg(ap, struct flock *);
			va_end(ap);

			return wpdk_fcntl_lockfile(fildes, cmd, flockp);
	}

	WPDK_UNIMPLEMENTED();
	return wpdk_posix_error(EINVAL);
}


int
wpdk_lockfile_get_range(int fildes, int whence,
		off_t offset, off_t size, off_t *pStart, off_t *pBytes)
{
	LARGE_INTEGER end;
	off_t posn = 0;
	HANDLE h;

	/*
	 *  Adjust starting position
	 */
	switch (whence) {
		case SEEK_SET:
			break;

		case SEEK_CUR:
			if ((posn = _telli64(fildes)) == -1)
				return -1;
			break;

		case SEEK_END:
			if ((h = (HANDLE)_get_osfhandle(fildes)) == INVALID_HANDLE_VALUE)
				return -1;

			if (GetFileSizeEx(h, &end) == 0)
				return wpdk_last_error();

			posn = end.QuadPart;
			break;

		default:
			return wpdk_posix_error(EINVAL);
	}

	if (posn >= LOCKFILE_MAX)
		return wpdk_posix_error(EINVAL);

	if ((offset < 0 && posn < (-offset)) || offset >= LOCKFILE_MAX)
			return wpdk_posix_error(EINVAL);

	posn += offset;

	/*
	 *  Calculate the required range
	 */
	if (size < 0) {
		if (posn < (-size) || (-size) > LOCKFILE_MAX)
			return wpdk_posix_error(EINVAL);

		*pStart = posn + size;
		*pBytes = (-size);
	} else {
		if (size > LOCKFILE_MAX || posn + size > LOCKFILE_MAX)
			return wpdk_posix_error(EINVAL);

		*pStart = posn;
		*pBytes = (size == 0) ? LOCKFILE_MAX - posn : size;
	}

	return 0;
}


#define LOCKFILE_GUARD		(INT64_MAX - LOCKFILE_MAX)

int
wpdk_lockfile(int op, int fildes, off_t start, ssize_t nbytes, bool nowait)
{
	DWORD error, flags = 0;
	OVERLAPPED io = {0};
	LARGE_INTEGER v;
	HANDLE h;
	BOOL rc;

	if ((h = (HANDLE)_get_osfhandle(fildes)) == INVALID_HANDLE_VALUE)
		return -1;

	/*
	 *  POSIX locks are advisory, but Windows file locks are not,
	 *  so offset into unallocated file space to avoid blocking I/O
	 *  requests.
	 */
	v.QuadPart = LOCKFILE_MAX + start;
	io.Offset = v.LowPart;
	io.OffsetHigh = v.HighPart;
	v.QuadPart = nbytes;

	if (nowait) {
		flags |= LOCKFILE_FAIL_IMMEDIATELY;

		/*
		 *  Checking if a region is locked involves changing the state,
		 *  so take a guard lock to protect the operation.
		 */
		if (wpdk_lockfile(F_WRLCK, fildes, LOCKFILE_GUARD, 1, false) == -1)
			return -1;
	}

	switch (op) {
		case F_WRLCK:
			rc = LockFileEx(h, LOCKFILE_EXCLUSIVE_LOCK | flags, 0,
					v.LowPart, v.HighPart, &io);
			break;

		case F_RDLCK:
			rc = LockFileEx(h, flags, 0, v.LowPart, v.HighPart, &io);
			break;

		case F_UNLCK:
			rc = UnlockFileEx(h, 0, v.LowPart, v.HighPart, &io);
			break;

		default:
			return wpdk_posix_error(EINVAL);
	}

	/*
	 *  Preserve any error and release the guard lock
	 */
	error = GetLastError();

	if (nowait)
		wpdk_lockfile(F_UNLCK, fildes, LOCKFILE_GUARD, 1, false);

	return (rc == FALSE) ? wpdk_windows_error(error) : 0;
}
