#ifndef _WPDK_SYS_UIO_H_
#define	_WPDK_SYS_UIO_H_

#include <sys/platform.h>
#include <sys/_iovec.h>

_WPDK_BEGIN_C_HEADER

ssize_t readv(int fildes, const struct iovec *iov, int iovcnt);

ssize_t __wrap_writev(int fildes, const struct iovec *iov, int iovcnt);

#ifndef _WPDK_BUILD_LIB_
static inline ssize_t writev(int fildes, const struct iovec *iov, int iovcnt) {
	return __wrap_writev(fildes, iov, iovcnt);
}
#endif

_WPDK_END_C_HEADER

#endif /* !_WPDK_SYS_UIO_H_ */