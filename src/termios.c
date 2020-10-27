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
#include <termios.h>
#include <unistd.h>


int wpdk_tcsetattr(int fildes, int optional_actions, const struct termios *termios_p)
{
	if (!termios_p)
		return wpdk_posix_error(EINVAL);

	if (!wpdk_isatty(fildes))
		return -1;

	switch (optional_actions) {
		case TCSANOW:
		case TCSADRAIN:
		case TCSAFLUSH:
			break;

		default:
			return wpdk_posix_error(EINVAL);
	}

	WPDK_UNIMPLEMENTED();
	return 0;
}


int wpdk_tcgetattr(int fildes, struct termios *termios_p)
{
	if (!termios_p)
		return wpdk_posix_error(EINVAL);

	if (!wpdk_isatty(fildes))
		return -1;

	memset(termios_p, 0, sizeof(struct termios));

	WPDK_UNIMPLEMENTED();
	return 0;
}
