/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2020, MayaData Inc. All rights reserved.
 *  Copyright (c) 2020, DataCore Software Corporation. All rights reserved.
 */

#ifndef _WPDK_SYS__IOVEC_H_
#define _WPDK_SYS__IOVEC_H_

#include <sys/platform.h>

_WPDK_BEGIN_C_HEADER

struct iovec {
	void *iov_base;     /* Address */
	size_t iov_len;     /* Length */
};

_WPDK_END_C_HEADER

#endif /* _WPDK_SYS__IOVEC_H_ */
