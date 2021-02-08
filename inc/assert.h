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
#include_next <assert.h>
#else
#include <../ucrt/assert.h>
#endif

#ifndef _WPDK_ASSERT_H_
#define _WPDK_ASSERT_H_

#if !defined(static_assert) && !defined(__cplusplus)

#if defined(__MINGW32__)
#define static_assert _Static_assert
#endif

#if defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wkeyword-macro"
#define static_assert _Static_assert
#pragma GCC diagnostic pop
#endif

#endif /* !static_assert && !_cplusplus */

#endif /* _WPDK_ASSERT_H_ */
