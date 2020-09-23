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
#include <sys/uio.h>
#include <limits.h>
#include <io.h>


#define wpdk_writev __real_writev


static int
wpdk_uio_check_iovec(const struct iovec *iov, int iovcnt)
{
	ssize_t length = 0;
	int i;

	if (!iov || iovcnt < 0 || iovcnt > IOV_MAX)
		return 0;

	for (i = 0; i < iovcnt; i++) {
		/* Too long for a single read/write */
		if (iov[i].iov_len > UINT_MAX)
			return 0;

		/* Total length would overflow */
		if (iov[i].iov_len > (size_t)(SSIZE_MAX - length))
			return 0;

		length += iov[i].iov_len;
	}

	return 1;
}


ssize_t
wpdk_writev(int fildes, const struct iovec *iov, int iovcnt)
{
	ssize_t rc, len, nbytes = 0;
	int i;

	wpdk_set_invalid_handler();

	if (!wpdk_uio_check_iovec(iov, iovcnt))
		return wpdk_posix_error(EINVAL);

	if (wpdk_is_socket(fildes))
		return wpdk_socket_writev(fildes, iov, iovcnt);

	if (!wpdk_is_fd(fildes))
		return wpdk_posix_error(EBADF);
	
	/*
	 *  POSIX: The transfer is expected to be atomic which Windows doesn't
	 *  support unless the buffers are 4k aligned, so perform the I/O in
	 *  multiple segments. The alternative would be to copy the data through
	 *  a temporary buffer which is probably an overkill.
	 */
	for (i = 0; i < iovcnt; i++) {
		len = iov[i].iov_len;

		rc = _write(fildes, iov[i].iov_base, (unsigned int)len);
		if (rc < 0) return -1;

		nbytes += rc;
		if (rc != len) break;
	}

	return nbytes;
}


ssize_t
wpdk_readv(int fildes, const struct iovec *iov, int iovcnt)
{
	ssize_t rc, len, nbytes = 0;
	int i;

	wpdk_set_invalid_handler();

	if (!wpdk_uio_check_iovec(iov, iovcnt))
		return wpdk_posix_error(EINVAL);

	if (wpdk_is_socket(fildes))
		return wpdk_socket_readv(fildes, iov, iovcnt);

	if (!wpdk_is_fd(fildes))
		return wpdk_posix_error(EBADF);

	/*
	 *  POSIX: The transfer is expected to be atomic which Windows doesn't
	 *  support unless the buffers are 4k aligned, so perform the I/O in
	 *  multiple segments. The alternative would be to copy the data through
	 *  a temporary buffer which is probably an overkill.
	 */
	for (i = 0; i < iovcnt; i++) {
		len = iov[i].iov_len;

		rc = _read(fildes, iov[i].iov_base, (unsigned int)len);
		if (rc < 0) return -1;

		nbytes += rc;
		if (rc != len) break;
	}

	return nbytes;
}
