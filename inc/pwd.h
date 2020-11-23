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

#ifndef _WPDK_PWD_H_
#define _WPDK_PWD_H_

#include <wpdk/header.h>
#include <sys/types.h>

struct passwd {
	char	*pw_name;
	uid_t	pw_uid;
	gid_t	pw_gid;
	char	*pw_dir;
	char	*pw_shell;
};

struct passwd *wpdk_getpwuid(uid_t uid);

#ifndef _WPDK_BUILD_LIB_
#define getpwuid(uid) wpdk_getpwuid(uid)
#endif

#endif /* _WPDK_PWD_H_ */
