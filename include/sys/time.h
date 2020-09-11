#ifndef _WPDK_SYS_TIME_H_
#define	_WPDK_SYS_TIME_H_

#include <sys/platform.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>

_WPDK_BEGIN_C_HEADER

#define	CLOCK_REALTIME	0

int clock_gettime(clockid_t, struct timespec *);

_WPDK_END_C_HEADER

#endif /* _WPDK_SYS_TIME_H_ */
