#ifndef _SYS__IOVEC_H_
#define _SYS__IOVEC_H_

#include <sys/platform.h>

_CRT_BEGIN_C_HEADER

struct iovec {
    void *iov_base;     /* Address */
    size_t iov_len;     /* Length */
};

_CRT_END_C_HEADER

#endif /* !_SYS__IOVEC_H_ */
