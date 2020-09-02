#include <sys/platform.h>
#include <netdb.h>


int _socket_startup();


int wpdk_getaddrinfo(const char *node, const char *service,
					 const struct addrinfo *hints, struct addrinfo **res)
{
	if (!_socket_startup())
		return EINVAL;
		
	return getaddrinfo(node, service, hints, res);
}


const char *wpdk_gai_strerror(int ecode)
{
	// HACK - not thread safe
	return gai_strerror(ecode);
}