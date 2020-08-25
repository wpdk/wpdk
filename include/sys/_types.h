#ifndef _SYS__TYPES_H_
#define _SYS__TYPES_H_

#include <sys/platform.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

_CRT_BEGIN_C_HEADER

typedef uint32_t clockid_t;
typedef uint16_t mode_t;
typedef uint32_t pid_t;
typedef long long ssize_t;
typedef uint64_t useconds_t;
typedef int64_t rlim_t;

_CRT_END_C_HEADER

#endif /* !_SYS__TYPES_H_ */