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

#ifndef _WPDK_SYS_SOCKET_H_
#define _WPDK_SYS_SOCKET_H_

#include <wpdk/header.h>
#include <wpdk/windows.h>
#include <sys/uio.h>
#include <inaddr.h>

#ifndef _WPDK_BUILD_LIB_
#define INCL_WINSOCK_API_PROTOTYPES 0
#endif

#define pollfd WSApollfd
#include <winsock2.h>
#undef pollfd

/*
 * 	ws2tcpip.h contains inline definitions for several functions that are
 *  not required, but cannot be disabled. Stub out the dependencies to
 *  avoid compilation errors.
 */
#ifndef _WPDK_BUILD_LIB_
#define WSASetLastError(a) ((a),0)
#define WSAIoctl(a,b,c,d,e,f,g,h,i) ((a),(b),(c),(d),(e),(f),(g),(h),(i),0)
#define setsockopt(a,b,c,d,e) ((a),(b),(c),(d),(e),0)
#define getsockopt(a,b,c,d,e) ((a),(b),(c),(d),(e),0)
#endif

#include <ws2tcpip.h>
#ifdef __MINGW32__
#define _WSACMSGHDR cmsghdr
#include <mswsock.h>
#endif

#ifndef _WPDK_BUILD_LIB_
#undef WSASetLastError
#undef WSAIoctl
#undef setsockopt
#undef getsockopt
#endif

_WPDK_BEGIN_C_HEADER

struct msghdr {
	void *msg_name;
	socklen_t msg_namelen;
	struct iovec *msg_iov;
	int msg_iovlen;
	struct {
		void *buf;
		socklen_t len;
	} Control;
	int msg_flags;
};

#define msg_control Control.buf
#define msg_controllen Control.len

#ifndef CMSG_FIRSTHDR
#define CMSG_FIRSTHDR(msg) WSA_CMSG_FIRSTHDR(msg)
#define CMSG_NXTHDR(msg,cmsg) WSA_CMSG_NXTHDR(msg,cmsg)
#define CMSG_DATA(cmsg) WSA_CMSG_DATA(cmsg)
#define CMSG_SPACE(length) WSA_CMSG_SPACE(length)
#define CMSG_LEN(length) WSA_CMSG_LEN(length)
#endif

#define SCM_RIGHTS			1
#define SCM_CREDENTIALS		2

typedef unsigned short sa_family_t;

int wpdk_accept(int socket, struct sockaddr *address, socklen_t *address_len);
int wpdk_bind(int socket, const struct sockaddr *address, socklen_t address_len);
int wpdk_connect(int socket, const struct sockaddr *address, socklen_t address_len);
int wpdk_getpeername(int socket, struct sockaddr *address, socklen_t *address_len);
int wpdk_getsockname(int socket, struct sockaddr *address, socklen_t *address_len);
int wpdk_getsockopt(int socket, int level, int option_name, void *option_value, socklen_t *option_len);
int wpdk_listen(int socket, int backlog);
ssize_t wpdk_recv(int socket, void *buffer, size_t length, int flags);
ssize_t wpdk_recvfrom(int socket, void *buffer, size_t length,
       int flags, struct sockaddr *address, socklen_t *address_len);
ssize_t wpdk_recvmsg(int socket, struct msghdr *message, int flags);
ssize_t wpdk_send(int socket, const void *buffer, size_t length, int flags);
ssize_t wpdk_sendmsg(int socket, const struct msghdr *message, int flags);
ssize_t wpdk_sendto(int socket, const void *message, size_t length,
       int flags, const struct sockaddr *dest_addr, socklen_t dest_len);
int wpdk_setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
int wpdk_shutdown(int socket, int how);
int wpdk_sockatmark(int s);
int wpdk_socket(int domain, int type, int protocol);
int wpdk_socketpair(int domain, int type, int protocol, int socket_vector[2]);

#ifndef _WPDK_BUILD_LIB_
#define accept wpdk_accept
#define bind wpdk_bind
#define connect wpdk_connect
#define getpeername wpdk_getpeername
#define getsockname wpdk_getsockname
#undef getsockopt
#define getsockopt wpdk_getsockopt
#define listen wpdk_listen
#define recv wpdk_recv
#define recvfrom wpdk_recvfrom
static inline ssize_t recvmsg(int socket, struct msghdr *message, int flags) {
	return wpdk_recvmsg(socket, message, flags);
}
#define send wpdk_send
static inline ssize_t sendmsg(int socket, const struct msghdr *message, int flags) {
	return wpdk_sendmsg(socket, message, flags);
}
#define sendto wpdk_sendto
#undef setsockopt
#define setsockopt wpdk_setsockopt
#define shutdown wpdk_shutdown
#define sockatmark wpdk_sockatmark
#define socket wpdk_socket
#define socketpair wpdk_socketpair
#endif

_WPDK_END_C_HEADER

#ifdef _MSC_VER
#pragma comment(lib, "ws2_32")
#endif

#endif /* _WPDK_SYS_SOCKET_H_ */
