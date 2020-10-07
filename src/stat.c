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
#include <direct.h>
#include <unistd.h>
#include <fcntl.h>


int
wpdk_mknod(const char *pathname, mode_t mode, dev_t dev)
{
	char buf[MAX_PATH];
	const char *path;
	mode_t omode = 0;
	int fd;

	wpdk_set_invalid_handler();

	if (!S_ISREG(mode) || dev != 0) {
		WPDK_UNIMPLEMENTED();
		return wpdk_posix_error(ENOSYS);
	}

	if (mode & (S_IRUSR|S_IRGRP|S_IROTH))
		omode |= S_IREAD;

	if (mode & (S_IWUSR|S_IWGRP|S_IWOTH))
		omode |= S_IWRITE;

	if ((path = wpdk_get_path(pathname, buf, sizeof(buf))) == NULL)
		return wpdk_posix_error(EINVAL);

	fd = _open(path, _O_CREAT|_O_EXCL|O_RDONLY, omode);
	if (fd < 0) return -1;

	_close(fd);
	return 0;
}


int
wpdk_stat64(const char *path, struct _stat64 *buf)
{
	int rc;
	
	wpdk_set_invalid_handler();

	if (!path || !buf)
		return wpdk_posix_error(EINVAL);

	rc = _stat64(path, buf);
	return (rc == 0) ? 0 : -1;
}


int
wpdk_fstat64(int fildes, struct _stat64 *buf)
{
#ifdef __MINGW32__
	BY_HANDLE_FILE_INFORMATION info;
	intptr_t h;
#endif
	int rc;
	
	wpdk_set_invalid_handler();

	if (!buf)
		return wpdk_posix_error(EINVAL);

	rc = _fstat64(fildes, buf);

#ifdef __MINGW32__
	/*
	 *  Mingw doesn't seem to recognise directories correctly,
	 *  so adjust st_mode to correct it.
	 */
	if (rc == 0 && (HANDLE)(h = _get_osfhandle(fildes)) != INVALID_HANDLE_VALUE)
		if (GetFileInformationByHandle((HANDLE)h, &info) != 0)
			if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				buf->st_mode = (buf->st_mode & ~_S_IFMT) | _S_IFDIR;
#endif
	return (rc == 0) ? 0 : -1;
}


int
wpdk_chmod(const char *filename, int pmode)
{
	char buf[MAX_PATH];
	const char *path;
	int mode = 0;
	
	wpdk_set_invalid_handler();

	if (pmode & S_IREAD) mode |= _S_IREAD;
	if (pmode & S_IWRITE) mode |= _S_IWRITE;

	if ((path = wpdk_get_path(filename, buf, sizeof(buf))) == NULL)
		return wpdk_posix_error(EINVAL);

	return _chmod(path, mode);
}


int wpdk_mkdir(const char *pathname, mode_t mode)
{
	char buf[MAX_PATH];
	const char *path;

	wpdk_set_invalid_handler();

	// mode is ignored
	UNREFERENCED_PARAMETER(mode);

	if ((path = wpdk_get_path(pathname, buf, sizeof(buf))) == NULL)
		return wpdk_posix_error(EINVAL);

	return _mkdir(path);	
}
