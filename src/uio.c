#include <sys/platform.h>
#include <sys/uio.h>


inline static ssize_t writev(int fildes, const struct iovec *iov, int iovcnt)
{
    // HACK - not implemented
    return EINVAL;
}


inline static ssize_t readv(int fildes, const struct iovec *iov, int iovcnt)
{
    // HACK - not implemented
    return EINVAL;
}