#ifndef _SYS_SELECT_H_
#define _SYS_SELECT_H_

#include <sys/platform.h>
#include <sys/socket.h>

_CRT_BEGIN_C_HEADER

int wpdk_select(int nfds, fd_set *readfds, fd_set *writefds,
		fd_set *exceptfds, struct timeval *timeout);

#ifndef _WPDK_BUILD_LIB_
#define select wpdk_select
#endif

_CRT_END_C_HEADER

#endif /* !_SYS_SELECT_H_ */