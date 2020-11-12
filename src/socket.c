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
#include <fcntl.h>


long wpdk_socket_ready;

struct socket_fd {
	SOCKET socket;
	int domain;
	int flags;
	SRWLOCK lock;
} *wpdk_socket_fds;

static const int maxsockets = 512;
static const int socketbase = 0x10000;


int
wpdk_socket_startup()
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	struct socket_fd *fds;
	WSADATA wsaData;
	int wsaerr, i;

	fds = (struct socket_fd *)calloc(maxsockets, sizeof(struct socket_fd));

	if (fds == NULL) {
		WSASetLastError(WSA_NOT_ENOUGH_MEMORY);
		return 0;
	}

	wsaerr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaerr != 0) {
		free(fds);
		WSASetLastError(wsaerr);
		return 0;
	}

	for (i = 0; i < maxsockets; i++) {
		fds[i].socket = INVALID_SOCKET;
		InitializeSRWLock(&fds[i].lock);
	}

	if (InterlockedCompareExchangePointer((void **)&wpdk_socket_fds, fds, NULL) != NULL)
		free(fds);

	if (InterlockedCompareExchange(&wpdk_socket_ready, 1, 0) != 0)
		WSACleanup();

	return 1;
}


int
wpdk_socket_cleanup()
{
	if (WSACleanup() == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	free(wpdk_socket_fds);

	wpdk_socket_fds = NULL;
	wpdk_socket_ready = 0;
	return 0;
}


static int
wpdk_allocate_socket(SOCKET s, int domain)
{
	int i;
	
	for (i = 0; i < maxsockets; i++)
		if (wpdk_socket_fds[i].socket == INVALID_SOCKET)
			if ((SOCKET)InterlockedCompareExchangePointer(
					(void **)&wpdk_socket_fds[i].socket,
					(void *)s, (void *)INVALID_SOCKET) == INVALID_SOCKET) {
				wpdk_socket_fds[i].domain = domain;
				wpdk_socket_fds[i].flags = 0;
				return socketbase + i;
			}

	closesocket(s);
	return wpdk_posix_error(EMFILE);
}


int
wpdk_is_socket(int fd)
{
	return (socketbase <= fd && fd < socketbase + maxsockets);
}


SOCKET
wpdk_get_socket(int fd)
{
	SOCKET s = INVALID_SOCKET;

	if (wpdk_socket_fds && wpdk_is_socket(fd))
		s = wpdk_socket_fds[fd - socketbase].socket;
		
	if (s == INVALID_SOCKET)
		_set_errno(EBADF);

	return s;
}


int
wpdk_socket(int domain, int type, int protocol)
{
	SOCKET s;

	if (!wpdk_socket_ready && !wpdk_socket_startup())
		return wpdk_last_wsa_error();

	s = socket(domain, type, protocol);

	if (s == INVALID_SOCKET)
		return wpdk_last_wsa_error();

	return wpdk_allocate_socket(s, domain);
}


int
wpdk_close_socket(int socket)
{
	SOCKET s = wpdk_get_socket(socket);

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	if ((SOCKET)InterlockedCompareExchangePointer(
				(void **)&wpdk_socket_fds[socket - socketbase].socket,
				(void *)INVALID_SOCKET, (void *)s) != s)
		return wpdk_posix_error(EBADF);

	if (closesocket(s) == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return 0;
}


int
wpdk_accept(int socket, struct sockaddr *address, socklen_t *address_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int domain;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);
	
	domain = wpdk_socket_fds[socket - socketbase].domain;

	s = accept(s, address, address_len);

	if (s == INVALID_SOCKET)
		return wpdk_last_wsa_error();

	return wpdk_allocate_socket(s, domain);
}


int
wpdk_bind(int socket, const struct sockaddr *address, socklen_t address_len)
{
	const struct sockaddr_un *addr = (const struct sockaddr_un *)address;
	SOCKET s = wpdk_get_socket(socket);
	socklen_t len = address_len;
	struct sockaddr_un un;
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


int
wpdk_connect(int socket, const struct sockaddr *address, socklen_t address_len)
{
	const struct sockaddr_un *addr = (const struct sockaddr_un *)address;
	SOCKET s = wpdk_get_socket(socket);
	socklen_t len = address_len;
	struct sockaddr_un un;
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
		// HACK - connect WSAEWOULDBLOCK on non-blocking socket - wait with select?
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return wpdk_last_wsa_error();
	}

	return 0;
}


int
wpdk_getpeername(int socket, struct sockaddr *address, socklen_t *address_len)
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


int
wpdk_getsockname(int socket, struct sockaddr *address, socklen_t *address_len)
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


int
wpdk_getsockopt(int socket, int level, int option_name, void *option_value, socklen_t *option_len)
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


int
wpdk_listen(int socket, int backlog)
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


ssize_t
wpdk_recv(int socket, void *buffer, size_t length, int flags)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	if (length > INT_MAX)
		return wpdk_posix_error(EINVAL);

	rc = recv(s, buffer, (int)length, flags);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return rc;
}


ssize_t
wpdk_recvfrom(int socket, void *buffer, size_t length,
							   int flags, struct sockaddr *address, socklen_t *address_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	if (length > INT_MAX)
		return wpdk_posix_error(EINVAL);

	rc = recvfrom(s, buffer, (int)length, flags, address, address_len);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return rc;
}



WSABUF *
wpdk_get_wsabuf(WSABUF *buf, int count, const struct iovec *iov, int iovlen)
{
	WSABUF *pBuffer = buf;
	ssize_t length = 0;
	int i;

	if (!iov || iovlen < 0 || iovlen > IOV_MAX) {
		wpdk_posix_error(EINVAL);
		return NULL;
	}

	if (iovlen > count)
		pBuffer = calloc(iovlen, sizeof(struct iovec));

	if (!pBuffer) {
		wpdk_posix_error(ENOMEM);
		return NULL;
	}

	for (i = 0; i < iovlen; i++) {
		/* Check for invalid length */
		if (iov[i].iov_len > ULONG_MAX
				|| iov[i].iov_len > (size_t)(SSIZE_MAX - length)) {
			wpdk_posix_error(EINVAL);
			if (pBuffer != buf)	free(pBuffer);
			return NULL;
		}

		pBuffer[i].buf = iov[i].iov_base;
		pBuffer[i].len = (ULONG)iov[i].iov_len;
		length += iov[i].iov_len;
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


ssize_t
wpdk_socket_readv(int fildes, const struct iovec *iov, int iovcnt)
{
	SOCKET s = wpdk_get_socket(fildes);
	WSABUF *pBuffer, buf[10];
	DWORD flags = 0;
	DWORD nbytes;	
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	pBuffer = wpdk_get_wsabuf(buf, sizeof(buf) / sizeof(buf[0]), iov, iovcnt);

	if (!pBuffer) return -1;

	rc = WSARecv(s, pBuffer, iovcnt, &nbytes, &flags, NULL, NULL);	

	if (pBuffer != buf) free(pBuffer);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return nbytes;
}


ssize_t
wpdk_recvmsg(int socket, struct msghdr *message, int flags)
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
	if (!pBuffer) return -1;

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


ssize_t
wpdk_send(int socket, const void *buffer, size_t length, int flags)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	if (length > INT_MAX)
		return wpdk_posix_error(EINVAL);

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


ssize_t
wpdk_socket_writev(int fildes, const struct iovec *iov, int iovcnt)
{
	SOCKET s = wpdk_get_socket(fildes);
	WSABUF *pBuffer, buf[10];
	DWORD nbytes;
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	pBuffer = wpdk_get_wsabuf(buf, sizeof(buf) / sizeof(buf[0]), iov, iovcnt);

	if (!pBuffer) return -1;

	rc = WSASend(s, pBuffer, iovcnt, &nbytes, 0, NULL, NULL);	

	if (pBuffer != buf) free(pBuffer);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return nbytes;
}


ssize_t
wpdk_sendmsg(int socket, const struct msghdr *message, int flags)
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
	if (!pBuffer) return -1;

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


ssize_t
wpdk_sendto(int socket, const void *message, size_t length,
							 int flags, const struct sockaddr *dest_addr, socklen_t dest_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	if (length > INT_MAX)
		return wpdk_posix_error(EINVAL);

	rc = sendto(s, message, (int)length, flags, dest_addr, dest_len);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return rc;
}


int
wpdk_setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len)
{
	SOCKET s = wpdk_get_socket(socket);
	int rc;

	if (s == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	// Setting SO_REUSEADDR for AF_UNIX causes the bind to fail
	if (option_name == SO_REUSEADDR)
		if (wpdk_socket_fds[socket - socketbase].domain == AF_UNIX)
			return 0;

	if (option_name == SO_RCVLOWAT)
		return 0;

	rc = setsockopt(s, level, option_name, (const char *)option_value, option_len);

	if (rc == SOCKET_ERROR)
		return wpdk_last_wsa_error();

	return 0;
}


int
wpdk_shutdown(int socket, int how)
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


int
wpdk_sockatmark(int s)
{
	UNREFERENCED_PARAMETER(s);
	
	WPDK_UNIMPLEMENTED();
	return wpdk_posix_error(ENOSYS);
}


int
wpdk_socketpair(int domain, int type, int protocol, int socket_vector[2])
{
	UNREFERENCED_PARAMETER(domain);
	UNREFERENCED_PARAMETER(type);
	UNREFERENCED_PARAMETER(protocol);
	UNREFERENCED_PARAMETER(socket_vector);

	WPDK_UNIMPLEMENTED();
	return wpdk_posix_error(ENOSYS);
}


int
wpdk_socket_fcntl(int fildes, int cmd, int arg)
{
	struct socket_fd *pfd;
	u_long mode;
	SOCKET s;
	int rc;

	if ((s = wpdk_get_socket(fildes)) == INVALID_SOCKET)
		return wpdk_posix_error(EBADF);

	pfd = &wpdk_socket_fds[fildes - socketbase];

	switch (cmd) {
		case F_GETFL:
			return pfd->flags;

		case F_SETFL:
			mode = (arg & O_NONBLOCK) != 0;

			AcquireSRWLockExclusive(&pfd->lock);
			rc = ioctlsocket(s, FIONBIO, &mode);

			if (rc == SOCKET_ERROR) {
				ReleaseSRWLockExclusive(&pfd->lock);
				return wpdk_last_wsa_error();
			}

			/*
			 * Update the local copy of the socket flags.
			 */
			if (mode)
				pfd->flags |= O_NONBLOCK;
			else
				pfd->flags &= ~O_NONBLOCK;

			ReleaseSRWLockExclusive(&pfd->lock);
			return 0;
	}

	return wpdk_posix_error(EINVAL);
}
