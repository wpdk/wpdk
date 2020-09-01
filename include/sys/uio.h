#ifndef _SYS_UIO_H_
#define	_SYS_UIO_H_

#include <sys/platform.h>
#include <sys/_iovec.h>

_CRT_BEGIN_C_HEADER

ssize_t readv(int fildes, const struct iovec *iov, int iovcnt);
ssize_t writev(int fildes, const struct iovec *iov, int iovcnt);

_CRT_END_C_HEADER

#endif /* !_SYS_UIO_H_ */