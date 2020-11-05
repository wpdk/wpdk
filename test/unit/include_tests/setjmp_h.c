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

#include <setjmp.h>

#include <CUnit/Basic.h>

#ifdef __cplusplus
#define include_c_setjmp_h include_cpp_setjmp_h
#endif

#if defined(_MSC_VER) && defined(__cplusplus)
#pragma warning(disable : 4611)
#endif

void
include_c_setjmp_h(void)
{
	sigjmp_buf env;
	sigsetjmp(env, 0);

	CU_ASSERT(sizeof(sigjmp_buf) == sizeof(jmp_buf));
}

#include "include_ut.h"
