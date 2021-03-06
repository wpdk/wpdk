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

#include <wpdk/internal.h>
#include <stdio.h>
#include <stdlib.h>


void
wpdk_warning(const char *file,
		int line, const char *function, const char *fmt, ...)
{
	WPDK_UNIMPLEMENTED();
	fprintf(stderr, "%s:%d: %s(): %s\n", file, line, function, fmt);
}
