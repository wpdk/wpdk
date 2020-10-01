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
#include <sys/time.h>


int wpdk_clock_gettime(clockid_t clk_id, struct timespec *tp)
{
	if (clk_id != CLOCK_REALTIME) {
		WPDK_UNIMPLEMENTED();
		return wpdk_posix_error(EINVAL);
	}

#ifndef __MINGW32__
	if (timespec_get(tp, TIME_UTC) != TIME_UTC)
		return wpdk_posix_error(EINVAL);
#else
	memset(tp, 0, sizeof(*tp));
#endif

	// HACK - clock is msec granularity
	WPDK_UNIMPLEMENTED();
	return 0;
}


int
wpdk_gettimeofday(struct timeval *tp, void *tzp)
{
	struct timespec t;
	int rc;

	if (!tp || tzp != NULL)
		return wpdk_posix_error(EINVAL);

	rc = wpdk_clock_gettime(CLOCK_REALTIME, &t);

	tp->tv_sec = (long)t.tv_sec;
	tp->tv_usec = (long)(t.tv_nsec / 1000);
	return rc;
}
