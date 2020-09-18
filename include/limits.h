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

#include <wpdk/header.h>

#ifdef _WPDK_INCLUDE_NEXT_
__extension__
#include_next <limits.h>
#else
#include <../km/crt/limits.h>
#endif

#ifndef _WPDK_LIMITS_H_
#define _WPDK_LIMITS_H_

#include <stdlib.h>

#ifndef PATH_MAX
#define PATH_MAX _MAX_PATH
#endif

#ifndef SSIZE_MAX
#define SSIZE_MAX _I64_MAX
#endif

#endif /* _WPDK_LIMITS_H_ */
