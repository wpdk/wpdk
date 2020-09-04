#ifndef _WPDK_POLL_H_
#define	_WPDK_POLL_H_

#include <sys/platform.h>
#include <sys/socket.h>

_WPDK_BEGIN_C_HEADER

typedef	unsigned int nfds_t;

int poll(struct pollfd fds[], nfds_t nfds, int timeout);

_WPDK_END_C_HEADER

#endif /* !_WPDK_POLL_H_ */