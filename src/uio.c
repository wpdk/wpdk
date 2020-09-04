#include <sys/platform.h>
#include <sys/socket.h>
#include <sys/uio.h>


int wpdk_is_socket(int fd);
ssize_t wpdk_socket_readv(int fildes, const struct iovec *iov, int iovcnt);
ssize_t wpdk_socket_writev(int fildes, const struct iovec *iov, int iovcnt);


ssize_t __real_writev(int fildes, const struct iovec *iov, int iovcnt)
{
	ssize_t rc, len, nbytes = 0;
	int i;

	if (!iov || iovcnt < 0) {
		_set_errno(EINVAL);
		return -1;
	}

	if (wpdk_is_socket(fildes))
		return wpdk_socket_writev(fildes, iov, iovcnt);

	// HACK - non-atomic and does I/O individually
	for (i = 0; i < iovcnt; i++) {
		len = iov[i].iov_len;

		rc = _write(fildes, iov[i].iov_base, len);
		if (rc < 0) return -1;

		nbytes += rc;
		if (rc != len) break;
	}

	return nbytes;
}


ssize_t readv(int fildes, const struct iovec *iov, int iovcnt)
{
	ssize_t rc, len, nbytes = 0;
	int i;

	if (!iov || iovcnt < 0) {
		_set_errno(EINVAL);
		return -1;
	}

	if (wpdk_is_socket(fildes))
		return wpdk_socket_readv(fildes, iov, iovcnt);

	// HACK - non-atomic and does I/O individually
	for (i = 0; i < iovcnt; i++) {
		len = iov[i].iov_len;

		rc = _read(fildes, iov[i].iov_base, len);
		if (rc < 0) return -1;

		nbytes += rc;
		if (rc != len) break;
	}

	return nbytes;
}