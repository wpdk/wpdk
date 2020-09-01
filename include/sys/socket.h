#ifndef _SYS_SOCKET_H_
#define _SYS_SOCKET_H_

#include <sys/platform.h>
#include <sys/_iovec.h>
#include <winsock2.h>
#include <ws2tcpip.h>
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

int _getsockopt(int socket, int level, int option_name,
       void *option_value, socklen_t *option_len);

int _setsockopt(int socket, int level, int option_name,
       const void *option_value, socklen_t option_len);

ssize_t __wrap_recvmsg(int socket, struct msghdr *message, int flags);
ssize_t __wrap_sendmsg(int socket, const struct msghdr *message, int flags);

#ifndef _WPDK_BUILD_LIB_
#define setsockopt _setsockopt
#define getsockopt _getsockopt

static inline ssize_t recvmsg(int socket, struct msghdr *message, int flags) {
       return __wrap_recvmsg(socket, message, flags);
}

static inline ssize_t sendmsg(int socket, const struct msghdr *message, int flags) {
       return __wrap_sendmsg(socket, message, flags);
}
#endif

_CRT_END_C_HEADER

#endif /* !_SYS_SOCKET_H_ */