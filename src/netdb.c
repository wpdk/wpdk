#include <sys/platform.h>
#include <netdb.h>


extern int _startup_socket();


int _getaddrinfo(const char *node, const char *service,
        const struct addrinfo *hints, struct addrinfo **res)
{
    if (!_socket_startup()) return EINVAL;
    return getaddrinfo(node, service, hints, res);
}

