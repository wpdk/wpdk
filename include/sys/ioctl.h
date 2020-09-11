#ifndef	_WPDK_SYS_IOCTL_H_
#define	_WPDK_SYS_IOCTL_H_

#include <sys/platform.h>

_WPDK_BEGIN_C_HEADER

#define	SIOCSIFFLAGS	1
#define	SIOCGIFFLAGS	2

int ioctl(int fd, unsigned long request, ...);

_WPDK_END_C_HEADER

#endif /* !__WPDK_SYS_IOCTL_H_ */
