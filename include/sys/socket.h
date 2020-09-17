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

/*
 * 	ws2tcpip.h contains inline definitions for several functions that are
 *  not required, but cannot be disabled. Redefine their dependencies to
 *  make sure that any use will result in a link error.
 */
#ifndef _WPDK_BUILD_LIB_
#define INCL_WINSOCK_API_PROTOTYPES 0
int wpdk_undefined_function(int,...);
#define WSASetLastError(a) wpdk_undefined_function(0,a)
#define WSAIoctl(a,b,c,d,e,f,g,h,i) wpdk_undefined_function(0,a,b,c,d,e,f,g,h,i)
#define getsockopt(a,b,c,d,e) wpdk_undefined_function(0,a,b,c,d,e)
#define setsockopt(a,b,c,d,e) wpdk_undefined_function(0,a,b,c,d,e)
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <afunix.h>

_WPDK_BEGIN_C_HEADER

struct msghdr {
	void *msg_name;
	socklen_t msg_namelen;
	struct iovec *msg_iov;
	int msg_iovlen;
	void *msg_control;
	socklen_t msg_controllen;
	int msg_flags;
};

#ifndef _WPDK_BUILD_LIB_
#define wpdk_recvmsg __wrap_recvmsg
#define wpdk_sendmsg __wrap_sendmsg
#endif

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
