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

#include <inaddr.h>

#include <CUnit/Basic.h>

#ifdef __cplusplus
#define include_c_inaddr_h include_cpp_inaddr_h
#endif


void
include_c_inaddr_h(void)
{
	struct in_addr in;

	CU_ASSERT(sizeof(struct in_addr) != 0);
	CU_ASSERT(sizeof(in.s_addr) != 0);
}
