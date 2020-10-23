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
#include <sys/ioctl.h>
#include <net/if.h>


int
wpdk_ioctl(int fd, unsigned long request, ...)
{
	struct ifreq *ifr;
	va_list ap;

	switch (request) {
		case SIOCGIFFLAGS:
		case SIOCSIFFLAGS:

			if (!wpdk_is_socket(fd))
				return wpdk_posix_error(EBADF);

			va_start(ap, request);
			ifr = va_arg(ap, struct ifreq *);
			va_end(ap);

			if (!ifr)
				return wpdk_posix_error(EINVAL);

			if (request == SIOCSIFFLAGS) {
				WPDK_UNIMPLEMENTED();
				return 0;
			}

			return wpdk_getifflags(ifr);
	}

	return wpdk_posix_error(EINVAL);
}
