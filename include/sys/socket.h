#ifndef _SYS_SOCKET_H_
#define _SYS_SOCKET_H_

#include <sys/platform.h>
#include <sys/_iovec.h>
#include <winsock2.h>
#include <ws2tcpip.h>

//HACK - only works on build 17063 onwards
#include <afunix.h>

_CRT_BEGIN_C_HEADER

struct msghdr {
    void        *msg_name;
    socklen_t   msg_namelen;
    struct iovec *msg_iov;
    int         msg_iovlen;
    void        *msg_control;
    socklen_t   msg_controllen;
    int         msg_flags;
};

ssize_t recvmsg(int socket, struct msghdr *message, int flags);
ssize_t sendmsg(int socket, const struct msghdr *message, int flags);

#include "../src/socket.c"

_CRT_END_C_HEADER

#endif /* !_SYS_SOCKET_H_ */