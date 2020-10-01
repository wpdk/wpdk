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
wpdk_mknod(const char *path, mode_t mode, dev_t dev)
{
	mode_t omode = 0;
	int fd;

	wpdk_set_invalid_handler();

	if (!S_ISREG(mode) || dev != 0) {
		WPDK_UNIMPLEMENTED();
		return wpdk_posix_error(ENOSYS);
	}

	if (!path)
		return wpdk_posix_error(EINVAL);

	if (mode & (S_IRUSR|S_IRGRP|S_IROTH))
		omode |= S_IREAD;

	if (mode & (S_IWUSR|S_IWGRP|S_IWOTH))
		omode |= S_IWRITE;

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
	int rc;
	
	wpdk_set_invalid_handler();

	if (!buf)
		return wpdk_posix_error(EINVAL);

	rc = _fstat64(fildes, buf);
	return (rc == 0) ? 0 : -1;
}


int
wpdk_chmod(const char *filename, int pmode)
{
	char buf[MAX_PATH];
	int mode = 0;
	
	wpdk_set_invalid_handler();

	if (!filename)
		return wpdk_posix_error(EINVAL);

	if (pmode & S_IREAD) mode |= _S_IREAD;
	if (pmode & S_IWRITE) mode |= _S_IWRITE;

	return _chmod(wpdk_get_path(filename, buf, sizeof(buf)), mode);
}


int wpdk_mkdir(const char *path, mode_t mode)
{
	char buf[MAX_PATH];

	wpdk_set_invalid_handler();

	// mode is ignored
	UNREFERENCED_PARAMETER(mode);

	if (!path)
		return wpdk_posix_error(EINVAL);

	return _mkdir(wpdk_get_path(path, buf, sizeof(buf)));	
}
