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
#include <sys/locking.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <stdbool.h>
#include <fcntl.h>


int
wpdk_flock(int fd, int operation)
{
	int rc, op = (operation & ~LOCK_NB);
	off_t start, nbytes;
	bool nowait;

	if (op != LOCK_SH && op != LOCK_EX && op != LOCK_UN)
		return wpdk_posix_error(EINVAL);

	/*
	 *  Calculate the required range and unlock
	 */
	if (wpdk_lockfile_get_range(fd, SEEK_SET,
			0, 0, &start, &nbytes) == -1)
		return -1;

	rc = wpdk_lockfile(F_UNLCK, fd, start, nbytes, false);
	
	/*
	 *  Issue locking request.
	 */
	nowait = ((operation & LOCK_NB) != 0);

	switch (op & ~LOCK_NB) {
		case LOCK_SH:
			rc = wpdk_lockfile(F_RDLCK, fd, start, nbytes, nowait);
			break;

		case LOCK_EX:
			rc = wpdk_lockfile(F_WRLCK, fd, start, nbytes, nowait);
			break;

		case LOCK_UN:
			return 0;
	}

	if (rc == -1 && errno == EACCES && nowait)
		return wpdk_posix_error(EWOULDBLOCK);

	return rc;
}
