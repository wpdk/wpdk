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


int
wpdk_mknod(const char *path, mode_t mode, dev_t dev)
{
	// HACK - implementation
	WPDK_UNIMPLEMENTED();
	UNREFERENCED_PARAMETER(path);
	UNREFERENCED_PARAMETER(mode);
	UNREFERENCED_PARAMETER(dev);
	return EINVAL;
}


// HACK - check implementation
int
wpdk_stat64(const char *path, struct _stat64 *buf)
{
	int rc = _stat64(path, buf);
	if (rc < 0) return rc;
	return 0;
}


// HACK - check implementation
int
wpdk_fstat64(int fildes, struct _stat64 *buf)
{
	int rc = _fstat64(fildes, buf);
	if (rc < 0) return rc;
	return 0;
}


int
wpdk_chmod(const char *filename, int pmode)
{
	char buf[MAX_PATH];
	int mode = 0;

	if (pmode & S_IREAD) mode |= _S_IREAD;
	if (pmode & S_IWRITE) mode |= _S_IWRITE;

	return _chmod(wpdk_get_path(filename, buf, sizeof(buf)), mode);
}
