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

#ifndef _WPDK_SYS_WAIT_H_
#define _WPDK_SYS_WAIT_H_

#include <wpdk/header.h>
#include <wpdk/types.h>

_WPDK_BEGIN_C_HEADER

pid_t wpdk_wait(int *stat_loc);
pid_t wpdk_waitpid(pid_t pid, int *stat_loc, int options);

#ifndef _WPDK_BUILD_LIB_
#define wait(loc) wpdk_wait(loc)
#define waitpid(pid,loc,opt) wpdk_waitpid(pid,loc,opt)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_SYS_WAIT_H_ */
