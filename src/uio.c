#include <sys/platform.h>
#include <sys/socket.h>
#include <sys/uio.h>


int wpdk_is_socket(int fd);
ssize_t wpdk_socket_readv(int fildes, const struct iovec *iov, int iovcnt);
ssize_t wpdk_socket_writev(int fildes, const struct iovec *iov, int iovcnt);


ssize_t __real_writev(int fildes, const struct iovec *iov, int iovcnt)
{
	if (wpdk_is_socket(fildes))
		return wpdk_socket_writev(fildes, iov, iovcnt);

	// HACK - not implemented
	return EINVAL;
}


ssize_t readv(int fildes, const struct iovec *iov, int iovcnt)
{
	if (wpdk_is_socket(fildes))
		return wpdk_socket_readv(fildes, iov, iovcnt);

	// HACK - not implemented
	return EINVAL;
}