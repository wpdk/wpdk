#include <sys/platform.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <pthread.h>
#include <stdlib.h>


void *__real_calloc(size_t nelem, size_t elsize);
ssize_t __real_recvmsg(int socket, struct msghdr *message, int flags);
ssize_t __real_sendmsg(int socket, const struct msghdr *message, int flags);
ssize_t __real_writev(int fildes, const struct iovec *iov, int iovcnt);
int __real_pthread_mutexattr_init(pthread_mutexattr_t *attr);
int __real_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);
int __real_unlink(const char *path);


void *__wrap_calloc(size_t nelem, size_t elsize)
{
    return __real_calloc(nelem, elsize);
}


ssize_t __wrap_recvmsg(int socket, struct msghdr *message, int flags)
{
    return __real_recvmsg(socket, message, flags);
}


ssize_t __wrap_sendmsg(int socket, const struct msghdr *message, int flags)
{
    return __real_sendmsg(socket, message, flags);
}


ssize_t __wrap_writev(int fildes, const struct iovec *iov, int iovcnt)
{
    return __real_writev(fildes, iov, iovcnt);
}


int __wrap_pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    return __real_pthread_mutexattr_init(attr);
}


int __wrap_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr)
{
    return __real_pthread_mutex_init(mutex, mutexattr);
}


int __wrap_unlink(const char *path) {
    return __real_unlink(path);
}