#ifndef _NETDB_H_
#define _NETDB_H_

#include <sys/platform.h>
#include <sys/socket.h>

_CRT_BEGIN_C_HEADER

int wpdk_getaddrinfo(const char *node, const char *service,
                       const struct addrinfo *hints,
                       struct addrinfo **res);

const char *wpdk_gai_strerror(int ecode);

#ifndef _WPDK_BUILD_LIB_
#define getaddrinfo wpdk_getaddrinfo
#define gai_strerror wpdk_gai_strerror
#endif

_CRT_END_C_HEADER

#endif /* _NETDB_H_ */