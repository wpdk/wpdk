#ifndef _WPDK_SYS_WAIT_H_
#define _WPDK_SYS_WAIT_H_

#include <sys/platform.h>

_WPDK_BEGIN_C_HEADER

pid_t wait(int *stat_loc);
pid_t waitpid(pid_t pid, int *stat_loc, int options);

_WPDK_END_C_HEADER

#endif /* _WPDK_SYS_WAIT_H_ */
