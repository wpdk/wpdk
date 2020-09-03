#include <sys/platform.h>
#include <sys/socket.h>
#include <stdlib.h>


#define wpdk_sendmsg __real_sendmsg
#define wpdk_recvmsg __real_recvmsg


int wpdk_socket_rc(int rc);
int wpdk_socket_error();
void wpdk_socket_seterrno();

long wpdk_socket_ready;
SOCKET *wpdk_socket_fds;

static const int maxsockets = 512;
static const int socketbase = 0x10000;


int wpdk_socket_startup()
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int wsaerr, i;
	SOCKET *fds;

	fds = (SOCKET *)calloc(maxsockets, sizeof(SOCKET));

	if (fds == NULL) {
		_set_errno(ENOMEM);
		return 0;
	}

	wsaerr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaerr != 0) {
		wpdk_socket_seterrno(wsaerr);
		free(fds);
		return 0;
	}

	for (i = 0; i < maxsockets; i++)
		fds[i] = INVALID_SOCKET;

	if (InterlockedCompareExchangePointer(&wpdk_socket_fds, fds, NULL) != NULL)
		free(fds);

	if (InterlockedCompareExchange(&wpdk_socket_ready, 1, 0) != 0)
		WSACleanup();

	return 1;
}


// HACK - not sure when to call!
int wpdk_socket_cleanup()
{
	if (WSACleanup() == SOCKET_ERROR)
		return wpdk_socket_error();

	return 0;
}


int wpdk_allocate_socket(SOCKET s)
{
	int i;
	
	// HACK - how to select exchange size?
	for (i = 0; i < maxsockets; i++)
		if (wpdk_socket_fds[i] == INVALID_SOCKET)
			if (InterlockedCompareExchange64(&wpdk_socket_fds[i], s, INVALID_SOCKET) == INVALID_SOCKET)
				return socketbase + i;

	closesocket(s);
	_set_errno(EMFILE);
	return -1;
}


int wpdk_is_socket(int fd)
{
	return (socketbase <= fd && fd < socketbase + maxsockets);
}


SOCKET wpdk_get_socket(int fd)
{
	SOCKET s = INVALID_SOCKET;

	if (wpdk_socket_fds && wpdk_is_socket(fd))
		s = wpdk_socket_fds[fd - socketbase];
		
	if (s == INVALID_SOCKET)
		_set_errno(EBADF);

	return s;
}


int wpdk_socket(int domain, int type, int protocol)
{
	SOCKET s;
	int i;

	if (!wpdk_socket_ready && !wpdk_socket_startup())
		return wpdk_socket_error();

	s = socket(domain, type, protocol);

	if (s == INVALID_SOCKET)
		return wpdk_socket_error();

	return wpdk_allocate_socket(s);
}


int wpdk_close_socket(int socket)
{
	SOCKET s = wpdk_get_socket(socket);

	if (s == INVALID_SOCKET)
		return -1;

	if (InterlockedCompareExchange64(&wpdk_socket_fds[socket - socketbase], INVALID_SOCKET, s) != s) {
		_set_errno(EBADF);
		return -1;
	}

	if (closesocket(s) == SOCKET_ERROR)
		return wpdk_socket_error();

	return 0;
}


int wpdk_accept(int socket, struct sockaddr *address, socklen_t *address_len)
{
	SOCKET s = wpdk_get_socket(socket);

	if (s == INVALID_SOCKET)
		return -1;
	
	s = accept(s, address, address_len);

	if (s == INVALID_SOCKET)
		return wpdk_socket_error();

	return wpdk_allocate_socket(s);
}


int wpdk_bind(int socket, const struct sockaddr *address, socklen_t address_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return -1;
	
	rc = bind(s, address, address_len);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return 0;
}


int wpdk_connect(int socket, const struct sockaddr *address, socklen_t address_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	rc = connect(s, address, address_len);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return 0;
}


int wpdk_getpeername(int socket, struct sockaddr *address, socklen_t *address_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	rc = getpeername(s, address, address_len);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return 0;
}


int wpdk_getsockname(int socket, struct sockaddr *address, socklen_t *address_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	rc = getsockname(s, address, address_len);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return 0;
}


int wpdk_getsockopt(int socket, int level, int option_name, void *option_value, socklen_t *option_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	rc = getsockopt(s, level, option_name, (char *)option_value, option_len);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return 0;	
}


int wpdk_listen(int socket, int backlog)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	rc = listen(s, backlog);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return 0;	
}


ssize_t wpdk_recv(int socket, void *buffer, size_t length, int flags)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	rc = recv(s, buffer, length, flags);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return rc;
}


ssize_t wpdk_recvfrom(int socket, void *buffer, size_t length,
							   int flags, struct sockaddr *address, socklen_t *address_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	rc = recvfrom(s, buffer, length, flags, address, address_len);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return rc;
}


WSABUF *wpdk_get_wsabuf(WSABUF *pBuffer, ULONG count, const struct iovec *iov, int iovlen)
{
	int i;

	if (iovlen > count)
		pBuffer = calloc(iovlen, sizeof(struct iovec));

	if (!pBuffer)
		return 0;

	for (i = 0; i < iovlen; i++) {
		pBuffer[i].buf = iov[i].iov_base;
		pBuffer[i].len = iov[i].iov_len;
	}

	return pBuffer;
}


ssize_t wpdk_socket_readv(int fildes, const struct iovec *iov, int iovcnt)
{
	SOCKET s = wpdk_get_socket(fildes);
	WSABUF *pBuffer, buf[10];
	DWORD flags = 0;
	DWORD nbytes;	
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	pBuffer = wpdk_get_wsabuf(buf, sizeof(buf) / sizeof(buf[0]), iov, iovcnt);

	if (!pBuffer) {
		_set_errno(ENOMEM);
		return -1;
	}

	rc = WSARecv(s, pBuffer, iovcnt, &nbytes, &flags, NULL, NULL);	

	if (pBuffer != buf) free(pBuffer);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return nbytes;
}


ssize_t wpdk_recvmsg(int socket, struct msghdr *message, int flags)
{
	SOCKET s = wpdk_get_socket(socket);
	WSABUF *pBuffer, buf[10];
	WSAMSG msg = { 0 };
	DWORD nbytes;
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	pBuffer = wpdk_get_wsabuf(buf, sizeof(buf) / sizeof(buf[0]),
							  message->msg_iov, message->msg_iovlen);
	if (!pBuffer) {
		_set_errno(ENOMEM);
		return -1;
	}

	msg.name = message->msg_name;
	msg.namelen = message->msg_namelen;
	msg.lpBuffers = pBuffer;
	msg.dwBufferCount = message->msg_iovlen;
	msg.dwFlags = message->msg_flags;
	msg.Control.buf = message->msg_control;
	msg.Control.len = message->msg_controllen;

	// HACK - use WSARecv for now
	rc = WSARecv(s, msg.lpBuffers, msg.dwBufferCount, &nbytes, &msg.dwFlags, NULL, NULL);
//	rc = WSARecvMsg(s, &msg, &nbytes, NULL, NULL);

	if (pBuffer != buf) free(pBuffer);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	message->msg_flags = msg.dwFlags;
	message->msg_control = msg.Control.buf;
	message->msg_controllen = msg.Control.len;
	return nbytes;
}


ssize_t wpdk_send(int socket, const void *buffer, size_t length, int flags)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	rc = send(s, buffer, length, flags);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return rc;
}


ssize_t wpdk_socket_writev(int fildes, const struct iovec *iov, int iovcnt)
{
	SOCKET s = wpdk_get_socket(fildes);
	WSABUF *pBuffer, buf[10];
	DWORD nbytes;
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	pBuffer = wpdk_get_wsabuf(buf, sizeof(buf) / sizeof(buf[0]), iov, iovcnt);

	if (!pBuffer) {
		_set_errno(ENOMEM);
		return -1;
	}

	rc = WSASend(s, pBuffer, iovcnt, &nbytes, 0, NULL, NULL);	

	if (pBuffer != buf) free(pBuffer);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return nbytes;
}


ssize_t wpdk_sendmsg(int socket, const struct msghdr *message, int flags)
{
	SOCKET s = wpdk_get_socket(socket);
	WSABUF *pBuffer, buf[10];
	WSAMSG msg = { 0 };
	DWORD nbytes;
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	pBuffer = wpdk_get_wsabuf(buf, sizeof(buf) / sizeof(buf[0]),
							  message->msg_iov, message->msg_iovlen);
	if (!pBuffer) {
		_set_errno(ENOMEM);
		return -1;
	}

	msg.name = message->msg_name;
	msg.namelen = message->msg_namelen;
	msg.lpBuffers = pBuffer;
	msg.dwBufferCount = message->msg_iovlen;
	msg.dwFlags = message->msg_flags;
	msg.Control.buf = message->msg_control;
	msg.Control.len = message->msg_controllen;

	// HACK - use WSASend instead
	rc = WSASend(s, msg.lpBuffers, msg.dwBufferCount, &nbytes, msg.dwFlags, NULL, NULL);
	// rc = WSASendMsg(s, &msg, flags, &nbytes, NULL, NULL);

	if (pBuffer != buf) free(pBuffer);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return nbytes;
}


ssize_t wpdk_sendto(int socket, const void *message, size_t length,
							 int flags, const struct sockaddr *dest_addr, socklen_t dest_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	rc = sendto(s, message, length, flags, dest_addr, dest_len);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return rc;
}


int wpdk_setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	rc = setsockopt(s, level, option_name, (const char *)option_value, option_len);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return 0;
}


int wpdk_shutdown(int socket, int how)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return -1;

	rc = shutdown(socket, how);

	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return 0;
}


int wpdk_sockatmark(int s)
{
	// HACK - TODO
	return -1;
	// return sockatmark(s);
}


int wpdk_socketpair(int domain, int type, int protocol, int socket_vector[2])
{
	// HACK - TODO
	return -1;
	// return socketpair(domain, type, protocol, socket_vector[2]);
}


int wpdk_socket_rc (int rc)
{
	if (rc == SOCKET_ERROR)
		return wpdk_socket_error();

	return 0;
}


int wpdk_socket_error ()
{
	wpdk_socket_seterrno(WSAGetLastError());
	return -1;
}


// HACK - more extensive list
void wpdk_socket_seterrno (int wsaerr)
{
	int error = EINVAL;

	switch (wsaerr) {
		case WSAEMFILE:
			error = EMFILE;
			break;

		case WSAENOBUFS:
			error = ENOBUFS;
			break;

		case WSAEINVAL:
			error = EINVAL;
			break;

		case WSAECONNRESET:
			error = ECONNABORTED;
			break;
		
		case WSAEFAULT:
			error = EFAULT;
			break;

		case WSATRY_AGAIN:
			error = EAGAIN;
			break;

		case WSANOTINITIALISED:
			error = EPROTO;
			break;

		case WSAEINTR:
			error = EINTR;
			break;

		case WSAEWOULDBLOCK:
			error = EWOULDBLOCK;
			break;

		case WSAEPROTONOSUPPORT:
			error = EPROTONOSUPPORT;
			break;

		case WSAEPROTOTYPE:
			error = EPROTOTYPE;
			break;
		
		case WSAEAFNOSUPPORT:
			error = EAFNOSUPPORT;
			break;

		case WSAEOPNOTSUPP:
			error = EOPNOTSUPP;
			break;

		case WSAEADDRINUSE:
			error = EADDRINUSE;
			break;

		case WSAEADDRNOTAVAIL:
			error = EADDRNOTAVAIL;
			break;

		case WSAEACCES:
			error = EACCES;
			break;

		case WSAEINPROGRESS:
			error = EINPROGRESS;
			break;

		case WSAENOTSOCK:
			error = ENOTSOCK;
			break;

		case WSAENETDOWN:
			error = ENETDOWN;
			break;

		case WSAEALREADY:
			error = EALREADY;
			break;
		
		case WSAECONNREFUSED:
			error = ECONNREFUSED;
			break;

		case WSAEISCONN:
			error = EISCONN;
			break;

		case WSAENETUNREACH:
			error = ENETUNREACH;
			break;

		case WSAEHOSTUNREACH:
			error = EHOSTUNREACH;
			break;

		case WSAETIMEDOUT:
			error = ETIMEDOUT;
			break;
	}

	_set_errno(error);
}