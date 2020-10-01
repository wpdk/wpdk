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

#include <limits.h>

#include <CUnit/Basic.h>

#ifdef __cplusplus
#define include_c_limits_h include_cpp_limits_h
#endif


void
include_c_limits_h(void)
{
	CU_ASSERT(PATH_MAX != 0);
}
