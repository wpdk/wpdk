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
_WPDK_INCLUDE_NEXT_
#include_next <setjmp.h>
#else
#include <../include/setjmp.h>
#endif

#ifndef _WPDK_SETJMP_H_
#define _WPDK_SETJMP_H_

#include <sys/types.h>

_WPDK_BEGIN_C_HEADER

typedef jmp_buf sigjmp_buf;

int sigsetjmp(sigjmp_buf env, int savemask);
void siglongjmp(sigjmp_buf env, int val); 

#define sigsetjmp(buf,...) setjmp(buf)
#define siglongjmp(env,val) longjmp(env,val)

_WPDK_END_C_HEADER

#endif /* _WPDK_SETJMP_H_ */
