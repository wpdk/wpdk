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
#include <sys/resource.h>


int wpdk_getrlimit(int resource, struct rlimit *rlp)
{
	// HACK - not implemented
	WPDK_UNIMPLEMENTED();

	if (!rlp || resource != RLIMIT_CORE)
		return wpdk_posix_error(EINVAL);

	memset(rlp, 0, sizeof(struct rlimit));
	return 0;
}


int wpdk_setrlimit(int resource, const struct rlimit *rlp)
{
	// HACK - not implemented
	WPDK_UNIMPLEMENTED();

	if (!rlp || resource != RLIMIT_CORE)
		return wpdk_posix_error(EINVAL);

	return 0;
}


int wpdk_getrusage(int who, struct rusage *usage)
{
	// HACK - not implemented
	WPDK_UNIMPLEMENTED();

	if (!usage || who != RUSAGE_THREAD)
		return wpdk_posix_error(EINVAL);

	memset(usage, 0, sizeof(struct rusage));
	return 0;
}
