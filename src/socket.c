/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2020, MayaData Inc. All rights reserved.
 *  Copyright (c) 2020, DataCore Software Corporation. All rights reserved.
 * 
 *  POSIX details are based on the Open Group Base Specification Issue 7,
 *  2018 edition at https://pubs.opengroup.org/onlinepubs/9699919799/
 * 
 *  Details about Linux extensions are based on the Linux man-pages project
 *  at https://www.kernel.org/doc/man-pages/
 */

#include <wpdk/internal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>


#define wpdk_sendmsg __real_sendmsg
#define wpdk_recvmsg __real_recvmsg

long wpdk_socket_ready;
SOCKET *wpdk_socket_fds;
int *wpdk_socket_domains;

static const int maxsockets = 512;
static const int socketbase = 0x10000;


int wpdk_socket_startup()
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int wsaerr, i;
	int *domains;
	SOCKET *fds;

	fds = (SOCKET *)calloc(maxsockets, sizeof(SOCKET));

	if (fds == NULL) {
		WSASetLastError(WSA_NOT_ENOUGH_MEMORY);
		return 0;
	}

	domains = (int *)calloc(maxsockets, sizeof(int));

	if (domains == NULL) {
		free(fds);
		WSASetLastError(WSA_NOT_ENOUGH_MEMORY);
		return 0;
	}

	wsaerr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaerr != 0) {
		free(domains);
		free(fds);
		WSASetLastError(wsaerr);
		return 0;
	}

	for (i = 0; i < maxsockets; i++)
		fds[i] = INVALID_SOCKET;

	if (InterlockedCompareExchangePointer((void **)&wpdk_socket_domains, domains, NULL) != NULL)
		free(domains);

	if (InterlockedCompareExchangePointer((void **)&wpdk_socket_fds, fds, NULL) != NULL)
		free(fds);

	if (InterlockedCompareExchange(&wpdk_socket_ready, 1, 0) != 0)
		WSACleanup();

	return 1;
}


// HACK - not sure when to call!
int wpdk_socket_cleanup()
{
	if (WSACleanup() == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	// HACK - need to free fd and domain lists
	return 0;
}


int wpdk_allocate_socket(SOCKET s, int domain)
{
	int i;
	
	// HACK - how to select exchange size?
	for (i = 0; i < maxsockets; i++)
		if (wpdk_socket_fds[i] == INVALID_SOCKET)
			if ((SOCKET)InterlockedCompareExchangePointer((void **)&wpdk_socket_fds[i],
					(void *)s, (void *)INVALID_SOCKET) == INVALID_SOCKET) {
				wpdk_socket_domains[i] = domain;
				return socketbase + i;
			}

	closesocket(s);
	return wpdk_posix_error(EMFILE);
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

	if (!wpdk_socket_ready && !wpdk_socket_startup())
		return wpdk_last_wsa_error();

	s = socket(domain, type, protocol);

	if (s == INVALID_SOCKET)
		return wpdk_last_wsa_error();

	return wpdk_allocate_socket(s, domain);
}


int wpdk_close_socket(int socket)
{
	SOCKET s = wpdk_get_socket(socket);

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	if ((SOCKET)InterlockedCompareExchangePointer((void **)&wpdk_socket_fds[socket - socketbase],
				(void *)INVALID_SOCKET, (void *)s) != s)
		return wpdk_posix_error(EBADF);

	if (closesocket(s) == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return 0;
}


int wpdk_accept(int socket, struct sockaddr *address, socklen_t *address_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int domain;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);
	
	domain = wpdk_socket_domains[socket - socketbase];

	s = accept(s, address, address_len);

	if (s == INVALID_SOCKET)
		return wpdk_last_wsa_error();

	return wpdk_allocate_socket(s, domain);
}


int wpdk_bind(int socket, const struct sockaddr *address, socklen_t address_len)
{
	struct sockaddr_un un, *addr = (struct sockaddr_un *)address;
	SOCKET s = wpdk_get_socket(socket);
	socklen_t len = address_len;
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	if (address->sa_family == AF_UNIX) {
		if (address_len > (socklen_t)sizeof(un))
			return wpdk_posix_error(EINVAL);

		un.sun_family = addr->sun_family;

		if (wpdk_copy_path(un.sun_path, sizeof(un.sun_path), addr->sun_path) == NULL)
			return wpdk_posix_error(EINVAL);

		addr = &un;
		len = sizeof(un);
	}

	rc = bind(s, (struct sockaddr *)addr, len);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return 0;
}


int wpdk_connect(int socket, const struct sockaddr *address, socklen_t address_len)
{
	struct sockaddr_un un, *addr = (struct sockaddr_un *)address;
	SOCKET s = wpdk_get_socket(socket);
	socklen_t len = address_len;
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	if (address->sa_family == AF_UNIX) {
		if (address_len > (socklen_t)sizeof(un))
			return wpdk_posix_error(EINVAL);

		un.sun_family = addr->sun_family;

		if (wpdk_copy_path(un.sun_path, sizeof(un.sun_path), addr->sun_path) == NULL)
			return wpdk_posix_error(EINVAL);

		addr = &un;
		len = sizeof(un);
	}

	rc = connect(s, (struct sockaddr *)addr, len);

	if (rc == SOCKET_ERROR) {
		// HACK - connect - if WSAEWOULDBLOCK on non-blocking socket - ignore / retry?
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return wpdk_last_wsa_error();
	}

	return 0;
}


int wpdk_getpeername(int socket, struct sockaddr *address, socklen_t *address_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	rc = getpeername(s, address, address_len);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return 0;
}


int wpdk_getsockname(int socket, struct sockaddr *address, socklen_t *address_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	rc = getsockname(s, address, address_len);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return 0;
}


int wpdk_getsockopt(int socket, int level, int option_name, void *option_value, socklen_t *option_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	rc = getsockopt(s, level, option_name, (char *)option_value, option_len);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return 0;	
}


int wpdk_listen(int socket, int backlog)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	rc = listen(s, backlog);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return 0;	
}


ssize_t wpdk_recv(int socket, void *buffer, size_t length, int flags)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	// HACK - validate length is less that max int
	rc = recv(s, buffer, (int)length, flags);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return rc;
}


ssize_t wpdk_recvfrom(int socket, void *buffer, size_t length,
							   int flags, struct sockaddr *address, socklen_t *address_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	// HACK - validate length is less that max int
	rc = recvfrom(s, buffer, (int)length, flags, address, address_len);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return rc;
}


WSABUF *wpdk_get_wsabuf(WSABUF *pBuffer, int count, const struct iovec *iov, int iovlen)
{
	int i;

	if (iovlen > count)
		pBuffer = calloc(iovlen, sizeof(struct iovec));

	if (!pBuffer)
		return 0;

	// HACK - validate length is less than ULONG

	for (i = 0; i < iovlen; i++) {
		pBuffer[i].buf = iov[i].iov_base;
		pBuffer[i].len = (ULONG)iov[i].iov_len;
	}

	return pBuffer;
}


ssize_t
wpdk_socket_read(int fildes, void *buf, size_t nbyte)
{
	struct iovec iov;

	iov.iov_base = buf;
	iov.iov_len = nbyte;
	return wpdk_socket_readv(fildes, &iov, 1);
}


ssize_t wpdk_socket_readv(int fildes, const struct iovec *iov, int iovcnt)
{
	SOCKET s = wpdk_get_socket(fildes);
	WSABUF *pBuffer, buf[10];
	DWORD flags = 0;
	DWORD nbytes;	
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	pBuffer = wpdk_get_wsabuf(buf, sizeof(buf) / sizeof(buf[0]), iov, iovcnt);

	if (!pBuffer)
		return wpdk_posix_error(ENOMEM);

	rc = WSARecv(s, pBuffer, iovcnt, &nbytes, &flags, NULL, NULL);	

	if (pBuffer != buf) free(pBuffer);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return nbytes;
}


ssize_t wpdk_recvmsg(int socket, struct msghdr *message, int flags)
{
	SOCKET s = wpdk_get_socket(socket);
	WSABUF *pBuffer, buf[10];
	WSAMSG msg = { 0 };
	DWORD nbytes;
	int rc;

	// HACK - wpdk_recvmsg flags ignored
	UNREFERENCED_PARAMETER(flags);

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	pBuffer = wpdk_get_wsabuf(buf, sizeof(buf) / sizeof(buf[0]),
							  message->msg_iov, message->msg_iovlen);
	if (!pBuffer)
		return wpdk_posix_error(ENOMEM);

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
		return wpdk_last_wsa_error();

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
		return wpdk_posix_error(EBADF);

	// HACK - validate length is less that max int
	rc = send(s, buffer, (int)length, flags);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return rc;
}


ssize_t
wpdk_socket_write(int fildes, const void *buf, size_t nbyte)
{
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = nbyte;
	return wpdk_socket_writev(fildes, &iov, 1);
}


ssize_t wpdk_socket_writev(int fildes, const struct iovec *iov, int iovcnt)
{
	SOCKET s = wpdk_get_socket(fildes);
	WSABUF *pBuffer, buf[10];
	DWORD nbytes;
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	pBuffer = wpdk_get_wsabuf(buf, sizeof(buf) / sizeof(buf[0]), iov, iovcnt);

	if (!pBuffer)
		return wpdk_posix_error(ENOMEM);

	rc = WSASend(s, pBuffer, iovcnt, &nbytes, 0, NULL, NULL);	

	if (pBuffer != buf) free(pBuffer);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return nbytes;
}


ssize_t wpdk_sendmsg(int socket, const struct msghdr *message, int flags)
{
	SOCKET s = wpdk_get_socket(socket);
	WSABUF *pBuffer, buf[10];
	WSAMSG msg = { 0 };
	DWORD nbytes;
	int rc;

	UNREFERENCED_PARAMETER(flags);

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	pBuffer = wpdk_get_wsabuf(buf, sizeof(buf) / sizeof(buf[0]),
							  message->msg_iov, message->msg_iovlen);
	if (!pBuffer)
		return wpdk_posix_error(ENOMEM);

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
		return wpdk_last_wsa_error();

	return nbytes;
}


ssize_t wpdk_sendto(int socket, const void *message, size_t length,
							 int flags, const struct sockaddr *dest_addr, socklen_t dest_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	// HACK - validate length is less that max int
	rc = sendto(s, message, (int)length, flags, dest_addr, dest_len);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return rc;
}


int wpdk_setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	// Setting SO_REUSEADDR for AF_UNIX causes the bind to fail
	if (option_name == SO_REUSEADDR)
		if (wpdk_socket_domains[socket - socketbase] == AF_UNIX)
			return 0;

	if (option_name == SO_RCVLOWAT)
		return 0;

	rc = setsockopt(s, level, option_name, (const char *)option_value, option_len);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return 0;
}


int wpdk_shutdown(int socket, int how)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	rc = shutdown(socket, how);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return 0;
}


int wpdk_sockatmark(int s)
{
	// HACK - TODO
	WPDK_UNIMPLEMENTED();
	UNREFERENCED_PARAMETER(s);
	return wpdk_posix_error(ENOSYS);
	// return sockatmark(s);
}


int wpdk_socketpair(int domain, int type, int protocol, int socket_vector[2])
{
	// HACK - TODO
	UNREFERENCED_PARAMETER(domain);
	UNREFERENCED_PARAMETER(type);
	UNREFERENCED_PARAMETER(protocol);
	UNREFERENCED_PARAMETER(socket_vector);
	WPDK_UNIMPLEMENTED();
	return wpdk_posix_error(ENOSYS);
	// return socketpair(domain, type, protocol, socket_vector[2]);
}


int wpdk_socket_rc (int rc)
{
	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return 0;
}
