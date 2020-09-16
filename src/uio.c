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

#include <wpdklib.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <io.h>


#define wpdk_writev __real_writev


ssize_t wpdk_writev(int fildes, const struct iovec *iov, int iovcnt)
{
	ssize_t rc, len, nbytes = 0;
	int i;

	if (!iov || iovcnt < 0)
		return wpdk_posix_error(EINVAL);

	if (wpdk_is_socket(fildes))
		return wpdk_socket_writev(fildes, iov, iovcnt);

	// HACK - non-atomic - does I/O individually
	for (i = 0; i < iovcnt; i++) {
		len = iov[i].iov_len;

		// HACK - check that len is within range for _write
		rc = _write(fildes, iov[i].iov_base, (unsigned int)len);
		if (rc < 0) return -1;

		nbytes += rc;
		if (rc != len) break;
	}

	return nbytes;
}


ssize_t wpdk_readv(int fildes, const struct iovec *iov, int iovcnt)
{
	ssize_t rc, len, nbytes = 0;
	int i;

	if (!iov || iovcnt < 0)
		return wpdk_posix_error(EINVAL);

	if (wpdk_is_socket(fildes))
		return wpdk_socket_readv(fildes, iov, iovcnt);

	// HACK - non-atomic and does I/O individually
	for (i = 0; i < iovcnt; i++) {
		len = iov[i].iov_len;

		// HACK - check that len is within range for _read
		rc = _read(fildes, iov[i].iov_base, (unsigned int)len);
		if (rc < 0) return -1;

		nbytes += rc;
		if (rc != len) break;
	}

	return nbytes;
}
