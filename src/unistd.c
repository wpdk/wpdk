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
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <io.h>


pid_t
wpdk_getpid()
{
	return GetCurrentProcessId();
}


int
wpdk_truncate(const char *pathname, off_t length)
{
	char buf[MAX_PATH];
	const char *path;
	int fd, rc;

	wpdk_set_invalid_handler();

	if ((path = wpdk_get_path(pathname, buf, sizeof(buf))) == NULL)
		return wpdk_posix_error(EINVAL);

	if ((fd = _open(path, O_RDWR)) == -1)
		return -1;

	if (_chsize_s(fd, length) != 0) {
		rc = errno;
		_close(fd);
		return wpdk_posix_error(rc);		
	}

	_close(fd);
	return 0;
}


int
wpdk_ftruncate(int fd, off_t length)
{
	int rc;
	
	wpdk_set_invalid_handler();
	
	rc = _chsize_s(fd, length);
	
	if (rc != 0 && errno == EACCES)
		return wpdk_posix_error(EBADF);
	
	return (rc == 0) ? 0 : -1;
}


int
wpdk_usleep(useconds_t useconds)
{
	static LARGE_INTEGER freq;
	LARGE_INTEGER now, end;
	
	/*
	 *  If the sleep is 10 msec or over then the resolution is
	 *  close to the system clock, so call SleepEx.
	 */
	if (useconds >= 10000) {
		SleepEx((DWORD)(useconds / 1000), TRUE);
		return 0;
	}

	if (freq.QuadPart == 0)
		QueryPerformanceFrequency(&freq);
	
	/*
	 *  For short sleep intervals, loop yielding the CPU until the
	 *  high-resolution timer indicates enough time has elapsed. This
	 *  avoids short sleeps being extended to match the system clock
	 *  interval.
	 */
	QueryPerformanceCounter(&now);
	end.QuadPart = now.QuadPart + (useconds * freq.QuadPart) / 1000000;

	while (now.QuadPart < end.QuadPart) {
		if (SwitchToThread() == 0)
			YieldProcessor();

		QueryPerformanceCounter(&now);
	}

	return 0;
}


char *
wpdk_ttyname(int fildes)
{
	static char tty[] = "/dev/tty";

	if (wpdk_isatty(fildes) == 0)
		return NULL;

	return tty;
}


int
wpdk_isatty(int fildes)
{
	wpdk_set_invalid_handler();
	
	if (!wpdk_is_fd(fildes)) {
		wpdk_posix_error(ENOTTY);
		return 0;
	}

	_set_errno(0);

	if (_isatty(fildes) == 0) {
		if (errno == 0 || errno != EBADF)
			wpdk_posix_error(ENOTTY);

		return 0;
	}

	return 1;
}


unsigned int
wpdk_sleep(unsigned int seconds)
{
	ULONGLONG start, elapsed;
	
	if (seconds == 0) return 0;

	/* Reduce the sleep time to avoid overflow */
	if (seconds > LONG_MAX / 1000)
		seconds = LONG_MAX / 1000;

	start = GetTickCount64();
	SleepEx(seconds * 1000, TRUE);

	elapsed = (GetTickCount64() - start + 500) / 1000;
	return (elapsed < seconds) ? (unsigned int)(seconds - elapsed) : 0;
}


static int
wpdk_get_processor_count(DWORD *pConfig, DWORD *pOnline)
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX pBuffer = NULL;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX ptr;
	DWORD length = 0;
	DWORD offset = 0;
	WORD i;

	while (GetLogicalProcessorInformationEx(RelationGroup,
				pBuffer, &length) != TRUE) {
		if (pBuffer) free(pBuffer);
		if ((pBuffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)
				malloc(length)) == NULL)
			return FALSE;
	}

	*pConfig = 0;
	*pOnline = 0;

	for (offset = 0; offset < length; offset += ptr->Size) {
		ptr = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)((char *)pBuffer + offset);
		if (ptr->Relationship != RelationGroup) continue;

		for (i = 0; i < ptr->Group.ActiveGroupCount; i++) {
			*pConfig += ptr->Group.GroupInfo[i].MaximumProcessorCount;
			*pOnline += ptr->Group.GroupInfo[i].ActiveProcessorCount;
		}
	}

	free(pBuffer);
	return TRUE;
}


long
wpdk_sysconf(int name)
{
	DWORD config, online;
	SYSTEM_INFO info;

	switch (name) {
		case _SC_NPROCESSORS_CONF:
			if (wpdk_get_processor_count(&config, &online) == TRUE)
				return config;

			WPDK_WARNING("Unable to obtain the number of configured processors");

			/*
			 *  If wpdk_get_processor_count fails, then return the
			 *  CPU count for the current processor group.
			 */
			GetSystemInfo(&info);
			return info.dwNumberOfProcessors;

		case _SC_NPROCESSORS_ONLN:
			if (wpdk_get_processor_count(&config, &online) == TRUE)
				return online;

			WPDK_WARNING("Unable to obtain the number of active processors");

			/*
			 *  If wpdk_get_processor_count fails, then return the
			 *  CPU count for the current processor group.
			 */
			GetSystemInfo(&info);
			return info.dwNumberOfProcessors;

		case _SC_PAGESIZE:
			GetSystemInfo(&info);
			return info.dwPageSize;

		case _SC_IOV_MAX:
			return IOV_MAX;
	}

	WPDK_UNIMPLEMENTED();
	return wpdk_posix_error(EINVAL);
}


pid_t
wpdk_fork()
{
	WPDK_UNIMPLEMENTED();
	return (pid_t)wpdk_posix_error(ENOSYS);
}


int
wpdk_daemon(int nochdir, int noclose)
{
	WPDK_UNIMPLEMENTED();
	UNREFERENCED_PARAMETER(nochdir);
	UNREFERENCED_PARAMETER(noclose);
	return wpdk_posix_error(ENOSYS);
}


ssize_t
wpdk_read(int fildes, void *buf, size_t nbyte)
{
	wpdk_set_invalid_handler();

	if (nbyte > INT_MAX)
		return wpdk_posix_error(EINVAL);

	if (wpdk_is_epoll(fildes))
		return wpdk_posix_error(EBADF);

	if (wpdk_is_socket(fildes))
		return wpdk_socket_read(fildes, buf, nbyte);

	return _read(fildes, buf, (unsigned int)nbyte);
}


ssize_t
wpdk_write(int fildes, const void *buf, size_t nbyte)
{
	wpdk_set_invalid_handler();

	if (nbyte > INT_MAX)
		return wpdk_posix_error(EINVAL);

	if (wpdk_is_epoll(fildes))
		return wpdk_posix_error(EBADF);

	if (wpdk_is_socket(fildes))
		return wpdk_socket_write(fildes, buf, nbyte);

	return _write(fildes, buf, (unsigned int)nbyte);
}


off_t
wpdk_lseek(int fildes, off_t offset, int whence)
{
	wpdk_set_invalid_handler();

	if (!wpdk_is_fd(fildes))
		return wpdk_posix_error(EBADF);

	return _lseeki64(fildes, offset, whence);
}


int
wpdk_unlink(const char *pathname)
{
	char buf[MAX_PATH];
	const char *path;

	wpdk_set_invalid_handler();

	if ((path = wpdk_get_path(pathname, buf, sizeof(buf))) == NULL)
		return wpdk_posix_error(EINVAL);

	/*
	 *  Windows fails with EACCESS if the file is read-only,
	 *  so try and make the file writable first.
	 */
	_chmod(path, S_IREAD|S_IWRITE);
	return _unlink(path);
}


int
wpdk_access(const char *pathname, int mode)
{
	char buf[MAX_PATH];
	const char *path;

	if ((path = wpdk_get_path(pathname, buf, sizeof(buf))) == NULL)
		return wpdk_posix_error(EINVAL);
		
	return _access(path, mode);
}


int
wpdk_is_fd(int fildes)
{
	return !wpdk_is_epoll(fildes) && !wpdk_is_socket(fildes);
}


int
wpdk_close(int fildes)
{
	wpdk_set_invalid_handler();

	if (wpdk_is_epoll(fildes))
		return wpdk_close_epoll(fildes);

	if (wpdk_is_socket(fildes))
		return wpdk_close_socket(fildes);

	return _close(fildes);
}


int
wpdk_fsync(int fildes)
{
	intptr_t h;
	DWORD rc;

	if (!wpdk_is_fd(fildes))
		return wpdk_posix_error(EBADF);

	h = _get_osfhandle(fildes);

	if ((HANDLE)h == INVALID_HANDLE_VALUE)
		return wpdk_posix_error(EBADF);

	if (FlushFileBuffers((HANDLE)h) == 0) {
		rc = GetLastError();

		/* If the handle is unbuffered, then return success */
		if (rc == ERROR_INVALID_HANDLE)
			return 0;

		return wpdk_windows_error(rc);
	}

	return 0;
}


int
wpdk_lockf(int fildes, int function, off_t size)
{
	off_t start, nbytes;
	int rc;

	wpdk_set_invalid_handler();

	if (wpdk_lockfile_get_range(fildes,
			SEEK_CUR, 0, size, &start, &nbytes) == -1)
		return -1;

	/*
	 *  Issue locking request.
	 */
	switch (function) {
		case F_LOCK:
			return wpdk_lockfile(F_WRLCK, fildes, start, nbytes, false);

		case F_TLOCK:
			return wpdk_lockfile(F_WRLCK, fildes, start, nbytes, true);

		case F_ULOCK:
			return wpdk_lockfile(F_UNLCK, fildes, start, nbytes, false);

		case F_TEST:
			if ((rc = wpdk_lockfile(F_WRLCK, fildes, start, nbytes, true)) != -1)
				wpdk_lockfile(F_UNLCK, fildes, start, nbytes, false);
			return rc;
	}

	return wpdk_posix_error(EINVAL);
}


uid_t
wpdk_getuid()
{
	// HACK - not implemented
	WPDK_UNIMPLEMENTED();
	return 0;
}
