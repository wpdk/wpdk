#ifndef _WPDK_WDPKLIB_H_
#define _WPDK_WDPKLIB_H_

#define _WPDK_BUILD_LIB_

#include <sys/platform.h>
#include <sys/socket.h>

int wpdk_socket_startup();

int wpdk_is_socket(int fd);
int wpdk_is_epoll(int fd);

int wpdk_socket_error();
void wpdk_socket_seterrno();
int wpdk_socket_rc(int rc);
SOCKET wpdk_get_socket(int fd);

int wpdk_close_epoll(int fd);
int wpdk_close_socket(int socket);

ssize_t wpdk_socket_readv(int fildes, const struct iovec *iov, int iovcnt);
ssize_t wpdk_socket_writev(int fildes, const struct iovec *iov, int iovcnt);

const char *wpdk_get_path(const char *path);

#endif /* !_WPDK_WPDKLIB_H_ */