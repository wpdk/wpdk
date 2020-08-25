#ifndef _SYS__SIGSET_H_
#define _SYS__SIGSET_H_

#include <sys/platform.h>
#include <sys/_types.h>

_CRT_BEGIN_C_HEADER

typedef struct sigset {
	uint32_t bits;
} sigset_t;

_CRT_END_C_HEADER

#endif /* !_SYS__SIGSET_H_ */