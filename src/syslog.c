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
#include <syslog.h>


void
wpdk_openlog(const char *ident, int option, int facility)
{
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(ident);
	UNREFERENCED_PARAMETER(option);
	UNREFERENCED_PARAMETER(facility);
}


void
wpdk_syslog(int priority, const char *format, ...)
{
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(priority);
	UNREFERENCED_PARAMETER(format);
}


void
wpdk_closelog()
{
	WPDK_UNIMPLEMENTED();
}
