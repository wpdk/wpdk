#include <wpdklib.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <io.h>


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

		// HACK - check that len is within range for _write
		rc = _write(fildes, iov[i].iov_base, (unsigned int)len);
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

		// HACK - check that len is within range for _read
		rc = _read(fildes, iov[i].iov_base, (unsigned int)len);
		if (rc < 0) return -1;

		nbytes += rc;
		if (rc != len) break;
	}

	return nbytes;
}
