#include <sys/platform.h>
#include <sys/socket.h>


int _socket_startup()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    return (err != 0) ? 0 : 1;
}


ssize_t __real_recvmsg(int socket, struct msghdr *message, int flags)
{
    // HACK - not implemented
    return -1;
}


ssize_t __real_sendmsg(int socket, const struct msghdr *message, int flags)
{
    // HACK - not implemented
    return -1;
}


int _getsockopt(int socket, int level, int option_name,
       void *option_value, socklen_t *option_len)
{
    return getsockopt(socket, level, option_name, (char *)option_value, option_len);
}


int _setsockopt(int socket, int level, int option_name,
       const void *option_value, socklen_t option_len)
{
    return setsockopt(socket, level, option_name, (const char *)option_value, option_len);
}
