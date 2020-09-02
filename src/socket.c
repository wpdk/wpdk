#include <sys/platform.h>
#include <sys/socket.h>


#define wpdk_sendmsg __real_sendmsg
#define wpdk_recvmsg __real_recvmsg


int _socket_startup()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	return (err != 0) ? 0 : 1;
}


int wpdk_accept(int socket, struct sockaddr *address, socklen_t *address_len)
{
	// HACK - TODO
	return accept(socket, address, address_len);
}


int wpdk_bind(int socket, const struct sockaddr *address, socklen_t address_len)
{
	// HACK - TODO
	return bind(socket, address, address_len);
}


int wpdk_connect(int socket, const struct sockaddr *address, socklen_t address_len)
{
	// HACK - TODO
	return connect(socket, address, address_len);
}


int wpdk_getpeername(int socket, struct sockaddr *address, socklen_t *address_len)
{
	// HACK - TODO
	return getpeername(socket, address, address_len);
}


int wpdk_getsockname(int socket, struct sockaddr *address, socklen_t *address_len)
{
	// HACK - TODO
	return getsockname(socket, address, address_len);
}


int wpdk_getsockopt(int socket, int level, int option_name, void *option_value, socklen_t *option_len)
{
	// HACK - TODO
	return getsockopt(socket, level, option_name, (char *)option_value, option_len);
}


int wpdk_listen(int socket, int backlog)
{
	// HACK - TODO
	return listen(socket, backlog);
}


ssize_t wpdk_recv(int socket, void *buffer, size_t length, int flags)
{
	// HACK - TODO
	return recv(socket, buffer, length, flags);
}


ssize_t wpdk_recvfrom(int socket, void *buffer, size_t length,
							   int flags, struct sockaddr *address, socklen_t *address_len)
{
	// HACK - TODO
	return recvfrom(socket, buffer, length, flags, address, address_len);
}


ssize_t wpdk_recvmsg(int socket, struct msghdr *message, int flags)
{
	// HACK - TODO
	return -1;
//	return recvmsg(socket, message, flags);
}


ssize_t wpdk_send(int socket, const void *buffer, size_t length, int flags)
{
	// HACK - TODO
	return send(socket, buffer, length, flags);
}


ssize_t wpdk_sendmsg(int socket, const struct msghdr *message, int flags)
{
	// HACK - TODO
	return -1;
//	return sendmsg(socket, message, flags);
}


ssize_t wpdk_sendto(int socket, const void *message, size_t length,
							 int flags, const struct sockaddr *dest_addr, socklen_t dest_len)
{
	// HACK - TODO
	return sendto(socket, message, length, flags, dest_addr, dest_len);
}


int wpdk_setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len)
{
	// HACK - TODO
	return setsockopt(socket, level, option_name, (const char *)option_value, option_len);
}


int wpdk_shutdown(int socket, int how)
{
	// HACK - TODO
	return shutdown(socket, how);
}


int wpdk_sockatmark(int s)
{
	// HACK - TODO
	return -1;
	// return sockatmark(s);
}


int wpdk_socket(int domain, int type, int protocol)
{
	// HACK - TODO
	return socket(domain, type, protocol);
}


int wpdk_socketpair(int domain, int type, int protocol, int socket_vector[2])
{
	// HACK - TODO
	return -1;
	// return socketpair(domain, type, protocol, socket_vector[2]);
}