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
#include <pwd.h>


static struct passwd root = {
	.pw_name = "root",
	.pw_uid = 0,
	.pw_gid = 0,
	.pw_dir = "/root",
	.pw_shell = "/bin/bash"
};

static struct passwd guest = {
	.pw_name = "guest",
	.pw_uid = 1000,
	.pw_gid = 1000,
	.pw_dir = "/home/guest",
	.pw_shell = "/bin/bash"
};


struct passwd *
wpdk_getpwuid(uid_t uid)
{
	if (uid == 0)
		return &root;

	guest.pw_uid = uid;

	WPDK_UNIMPLEMENTED();
	return &guest;
}
