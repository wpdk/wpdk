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
#include <sys/resource.h>


int
wpdk_getrlimit(int resource, struct rlimit *rlp)
{
	if (!rlp)
		return wpdk_posix_error(EINVAL);

	memset(rlp, 0, sizeof(struct rlimit));

	switch (resource) {
		case RLIMIT_CORE:
			return 0;

		case RLIMIT_NOFILE:
			rlp->rlim_cur = 8192;
			rlp->rlim_max = 8192;
			return 0;
	}

	WPDK_UNIMPLEMENTED();
	return wpdk_posix_error(EINVAL);
}


int
wpdk_setrlimit(int resource, const struct rlimit *rlp)
{
	if (!rlp || rlp->rlim_cur > rlp->rlim_max)
		return wpdk_posix_error(EINVAL);

	switch (resource) {
		case RLIMIT_CORE:
			return 0;

		case RLIMIT_NOFILE:
			if (rlp->rlim_max > 8192)
				return wpdk_posix_error(EPERM);

			return 0;
	}

	WPDK_UNIMPLEMENTED();
	return wpdk_posix_error(EINVAL);
}


int wpdk_getrusage(int who, struct rusage *usage)
{
	FILETIME creation, exit, system, user;
	LARGE_INTEGER v;

	if (!usage)
		return wpdk_posix_error(EINVAL);

	memset(usage, 0, sizeof(struct rusage));

	switch (who) {
		case RUSAGE_THREAD:
			if (GetThreadTimes(GetCurrentThread(), &creation,
					&exit, &system, &user) == 0)
				return wpdk_last_error();

			v.HighPart = user.dwHighDateTime;
			v.LowPart = user.dwLowDateTime;
			usage->ru_utime.tv_sec = (long)(v.QuadPart / 10000000);
			usage->ru_utime.tv_usec = (v.QuadPart / 10) % 1000000;

			v.HighPart = system.dwHighDateTime;
			v.LowPart = system.dwLowDateTime;
			usage->ru_stime.tv_sec = (long)(v.QuadPart / 10000000);
			usage->ru_stime.tv_usec = (v.QuadPart / 10) % 1000000;

			/*
			 *  POSIX: Context switch counts should be reported,
			 *  but are currently left as zero.
			 */
			return 0;
	}

	WPDK_UNIMPLEMENTED();
	return wpdk_posix_error(EINVAL);
}
