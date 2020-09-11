#ifndef _WPDK_SYS__IOVEC_H_
#define _WPDK_SYS__IOVEC_H_

#include <sys/platform.h>

_WPDK_BEGIN_C_HEADER

struct iovec {
	void *iov_base;     /* Address */
	size_t iov_len;     /* Length */
};

_WPDK_END_C_HEADER

#endif /* _WPDK_SYS__IOVEC_H_ */
