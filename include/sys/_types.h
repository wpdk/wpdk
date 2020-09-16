/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2020, MayaData Inc. All rights reserved.
 *  Copyright (c) 2020, DataCore Software Corporation. All rights reserved.
 * 
 *  POSIX details are based on the Open Group Base Specification Issue 7,
 *  2018 edition at https://pubs.opengroup.org/onlinepubs/9699919799/
 * 
 *  Details about Linux extensions are based on the Linux man-pages project
 *  at https://www.kernel.org/doc/man-pages/
 */

#ifndef _WPDK_SYS__TYPES_H_
#define _WPDK_SYS__TYPES_H_

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdint.h>

_WPDK_BEGIN_C_HEADER

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

_WPDK_END_C_HEADER

#endif /* _WPDK_SYS__TYPES_H_ */
