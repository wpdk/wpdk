#ifndef _WPDK_LIMITS_H_
#define _WPDK_LIMITS_H_

#include <sys/platform.h>
#include <../km/crt/limits.h>

_WPDK_BEGIN_C_HEADER

#ifndef SSIZE_MAX
#define SSIZE_MAX _I64_MAX
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_LIMITS_H_ */
