#include <sys/platform.h>
#include <sys/socket.h>
#include <sys/uio.h>


int wpdk_is_socket(int fd);


ssize_t __real_writev(int fildes, const struct iovec *iov, int iovcnt)
{
	if (wpdk_is_socket(fildes) && iovcnt == 1)
		return wpdk_send(fildes, iov->iov_base, iov->iov_len, 0);

	// HACK - not implemented
	return EINVAL;
}


ssize_t readv(int fildes, const struct iovec *iov, int iovcnt)
{
	if (wpdk_is_socket(fildes) && iovcnt == 1)
		return wpdk_recv(fildes, iov->iov_base, iov->iov_len, 0);

	// HACK - not implemented
	return EINVAL;
}
