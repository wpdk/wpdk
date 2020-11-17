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
#include <stdlib.h>
#include <string.h>

#undef strerror_s
#undef _strdup


char *
wpdk_strerror_r_gnu(int errnum, char *buf, size_t buflen)
{
	return wpdk_strerror_r(errnum, buf, buflen) ? NULL : buf;
}


int
wpdk_strerror_r(int errnum, char *buf, size_t buflen)
{
	if (!buf) {
		wpdk_posix_error(EINVAL);
		return EINVAL;
	}

	wpdk_set_invalid_handler();

	/*
	 *  POSIX: ERANGE should be returned if the buffer is too small
	 *  to contain the message. The message will be truncated instead.
	 */
	switch (errnum) {
		case ESHUTDOWN:
			strncpy(buf, "The socket has been shut down", buflen);
			return 0;

		case ETOOMANYREFS:
			strncpy(buf, "Too many references to kernel object", buflen);
			return 0;

		case EHOSTDOWN:
			strncpy(buf, "Host is down", buflen);
			return 0;

		case EUSERS:
			strncpy(buf, "Too many users", buflen);
			return 0;

		case EDQUOT:
			strncpy(buf, "Disk quota exceeded", buflen);
			return 0;

		case ESTALE:
			strncpy(buf, "Stale file handle", buflen);
			return 0;

		case EREMOTE:
			strncpy(buf, "Item is not available locally", buflen);
			return 0;

		case EPROCLIM:
			strncpy(buf, "Too many processes", buflen);
			return 0;
	}

	return strerror_s(buf, buflen, errnum);
}


char *
wpdk_strdup(const char *s)
{
	size_t len;
	char *cp;

	len = strlen(s) + 1;
	cp = wpdk_malloc(len);

	if (cp) memcpy(cp, s, len);
	return cp;
}
