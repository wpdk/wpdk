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
#include <sys/wait.h>


pid_t wpdk_wait(int *stat_loc)
{
	// HACK - implement
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(stat_loc);
	return (pid_t)-1;
}


pid_t wpdk_waitpid(pid_t pid, int *stat_loc, int options)
{
	// HACK - implement
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(pid);
	UNREFERENCED_PARAMETER(stat_loc);
	UNREFERENCED_PARAMETER(options);
	return (pid_t)-1;
}
