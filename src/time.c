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


int
wpdk_clock_gettime(clockid_t clk_id, struct timespec *tp)
{
	static LARGE_INTEGER freq;
	LARGE_INTEGER stamp;
	ULARGE_INTEGER v;
	FILETIME now;

	if (!tp)
		return wpdk_posix_error(EINVAL);

	switch (clk_id) {
		case CLOCK_MONOTONIC:
		case CLOCK_MONOTONIC_RAW:
			if (freq.QuadPart == 0)
				QueryPerformanceFrequency(&freq);

			QueryPerformanceCounter(&stamp);
			tp->tv_sec = (long)(stamp.QuadPart / freq.QuadPart);
			tp->tv_nsec = (long)(((stamp.QuadPart % freq.QuadPart) * 1000000000) / freq.QuadPart);

			return 0;

		case CLOCK_REALTIME:
			GetSystemTimePreciseAsFileTime(&now);

			v.HighPart = now.dwHighDateTime;
			v.LowPart = now.dwLowDateTime;

			/* Adjust to Unix Epoch */
			v.QuadPart -= 116444736000000000LL;

			tp->tv_sec = (long)(v.QuadPart / 10000000);
			tp->tv_nsec = (v.QuadPart % 10000000) * 100;
			return 0;
	}

	WPDK_UNIMPLEMENTED();
	return wpdk_posix_error(EINVAL);
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
