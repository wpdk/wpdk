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

#ifndef _WPDK_SYS_RESOURCE_H_
#define _WPDK_SYS_RESOURCE_H_

#include <wpdk/header.h>
#include <sys/time.h>

_WPDK_BEGIN_C_HEADER

struct rusage {
	struct timeval ru_utime;
	struct timeval ru_stime;
	long	ru_nvcsw;
	long	ru_nivcsw;
};

#define RUSAGE_THREAD	1

struct rlimit {
	rlim_t		rlim_cur; 
	rlim_t		rlim_max;
};

#define RLIMIT_CORE		1
#define RLIMIT_NOFILE	2

int wpdk_getrlimit(int resource, struct rlimit *rlp);
int wpdk_setrlimit(int resource, const struct rlimit *rlp);
int wpdk_getrusage(int who, struct rusage *usage);

#ifndef _WPDK_BUILD_LIB_
#define getrlimit(res,rlp) wpdk_getrlimit(res,rlp)
#define setrlimit(res,rlp) wpdk_setrlimit(res,rlp)
#define getrusage(who,usage) wpdk_getrusage(who,usage)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_SYS_RESOURCE_H_ */
