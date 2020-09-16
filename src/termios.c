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

#include <wpdklib.h>
#include <termios.h>


int wpdk_tcsetattr(int fildes, int optional_actions, const struct termios *termios_p)
{
	// HACK - implementation
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(fildes);
	UNREFERENCED_PARAMETER(optional_actions);
	UNREFERENCED_PARAMETER(termios_p);
	return -1;
}


int wpdk_tcgetattr(int fildes, struct termios *termios_p)
{
	// HACK - implementation
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(fildes);
	UNREFERENCED_PARAMETER(termios_p);
	return -1;
}
