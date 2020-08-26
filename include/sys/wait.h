#ifndef _SYS_WAIT_H_
#define _SYS_WAIT_H_

#include <sys/platform.h>

_CRT_BEGIN_C_HEADER

pid_t wait(int *stat_loc);
pid_t waitpid(pid_t pid, int *stat_loc, int options);

#include "../src/wait.c"

_CRT_END_C_HEADER

#endif /* !_SYS_WAIT_H_ */