#include <sys/platform.h>
#include <sys/uio.h>


ssize_t writev(int fildes, const struct iovec *iov, int iovcnt)
{
    // HACK - not implemented
    return EINVAL;
}


ssize_t readv(int fildes, const struct iovec *iov, int iovcnt)
{
    // HACK - not implemented
    return EINVAL;
}


ssize_t __real_writev(int fildes, const struct iovec *iov, int iovcnt)
{
    return writev(fildes, iov, iovcnt);
}