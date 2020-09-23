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
#include <string.h>

#undef strerror_s
#undef _strdup


char *_wpdk_errlist[] = {
	/* 200 - ESHUTDOWN */		"The socket has been shut down",
	/* 201 - ETOOMANYREFS */	"Too many references to kernel object",
	/* 202 - EHOSTDOWN */		"Host is down",
	/* 203 - EUSERS */			"Too many users",
	/* 204 - EDQUOT */			"Disk quota exceeded",
	/* 205 - ESTALE	*/			"Stale file handle",
	/* 206 - EREMOTE */			"Item is not available locally",
	/* 207 - EPROCLIM */		"Too many processes"
};

int _wpdk_nerr = sizeof(_wpdk_errlist) / sizeof(_wpdk_errlist[0]);


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

	/*
	*  POSIX: ERANGE should be returned if the buffer is too small
	*  to contain the message. The message will be truncated instead.
	*/

	if (_WPDK_BASE_ERRNO <= errnum && errnum < _WPDK_BASE_ERRNO + _wpdk_nerr) {
		strncpy(buf, _wpdk_errlist[errnum-_WPDK_BASE_ERRNO], buflen);
		return 0;
	}

	return strerror_s(buf, buflen, errnum);
}


char *
wpdk_strdup(const char *s)
{
	return _strdup(s);
}
