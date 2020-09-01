#ifndef _NETDB_H_
#define _NETDB_H_

#include <sys/platform.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <afunix.h>

_CRT_BEGIN_C_HEADER

int _getaddrinfo(const char *node, const char *service,
                       const struct addrinfo *hints,
                       struct addrinfo **res);

#ifndef _WPDK_BUILD_LIB_
#define getaddrinfo _getaddrinfo
#endif

_CRT_END_C_HEADER

#endif /* _NETDB_H_ */