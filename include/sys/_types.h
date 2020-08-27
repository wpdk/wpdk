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
typedef _dev_t dev_t;
typedef _ino_t ino_t;
typedef int64_t off_t;
typedef int64_t time_t;
typedef short nlink_t;
typedef short uid_t;
typedef short gid_t;

_CRT_END_C_HEADER

#endif /* !_SYS__TYPES_H_ */