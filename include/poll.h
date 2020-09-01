#ifndef _POLL_H_
#define	_POLL_H_

#include <sys/platform.h>
#include <sys/socket.h>

_CRT_BEGIN_C_HEADER

typedef	unsigned int nfds_t;

int poll(struct pollfd fds[], nfds_t nfds, int timeout);

_CRT_END_C_HEADER

#endif /* !_POLL_H_ */