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


inline static int _getsockopt(int socket, int level, int option_name,
       void *option_value, socklen_t *option_len)
{
    return getsockopt(socket, level, option_name, (char *)option_value, option_len);
}


inline static int _setsockopt(int socket, int level, int option_name,
       const void *option_value, socklen_t option_len)
{
    return setsockopt(socket, level, option_name, (const char *)option_value, option_len);
}

