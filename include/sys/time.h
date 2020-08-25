#ifndef _SYS_TIME_H_
#define	_SYS_TIME_H_

#include <sys/platform.h>
#include <time.h>

_CRT_BEGIN_C_HEADER

#define	CLOCK_REALTIME	0

int clock_gettime(clockid_t, struct timespec *);

#include "../src/time.c"

_CRT_END_C_HEADER

#endif /* !_SYS_TIME_H_ */