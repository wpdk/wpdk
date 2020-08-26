#ifndef	_SYS_IOCTL_H_
#define	_SYS_IOCTL_H_

#include <sys/platform.h>

_CRT_BEGIN_C_HEADER

#define	SIOCSIFFLAGS    1
#define	SIOCGIFFLAGS    2

int ioctl(int fd, unsigned long request, ...);

#include "../src/ioctl.c"

_CRT_END_C_HEADER

#endif /* !__SYS_IOCTL_H_ */