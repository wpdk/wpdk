#include <sys/platform.h>
#include <sys/socket.h>


inline static ssize_t recvmsg(int socket, struct msghdr *message, int flags)
{
    // HACK - not implemented
    return -1;
}


inline static ssize_t sendmsg(int socket, const struct msghdr *message, int flags)
{
    // HACK - not implemented
    return -1;
}


inline static ssize_t __real_recvmsg(int socket, struct msghdr *message, int flags)
{
    return recvmsg(socket, message, flags);
}


inline static ssize_t __real_sendmsg(int socket, const struct msghdr *message, int flags)
{
    return sendmsg(socket, message, flags);
}

