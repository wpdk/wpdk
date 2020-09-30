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
#include <stdio.h>
#include <unistd.h>


ssize_t
wpdk_getdelim(char **lineptr, size_t *n, int delim, FILE *stream)
{
	size_t newlen, len = 0;
	int ch = 0;
	char *buf;

	wpdk_set_invalid_handler();

	if (!lineptr || !n || !stream)
		return wpdk_posix_error(EINVAL);

	for (buf = *lineptr; ch != delim; buf[len++] = (char)ch) {
		if (!buf || len + 2 >= *n) {
			newlen = (buf && *n) ? (*n) * 2 : BUFSIZ;
			if ((buf = realloc(buf, newlen)) == NULL)
				return -1;
			*lineptr = buf;
			*n = newlen;
		}

		if ((ch = getc(stream)) == EOF) {
			if (feof(stream) && len) break;
			return -1;
		}
	}

	buf[len] = 0;
	return len;
}


ssize_t
wpdk_getline(char **lineptr, size_t *n, FILE *stream)
{
	return wpdk_getdelim(lineptr, n, '\n', stream);
}


FILE *
wpdk_fopen(const char *filename, const char *mode)
{
	char buf[MAX_PATH];

	wpdk_set_invalid_handler();
	return fopen(wpdk_get_path(filename, buf, sizeof(buf)), mode);
}


FILE *
wpdk_fdopen(int fildes, const char *mode)
{
	wpdk_set_invalid_handler();
	return _fdopen(fildes, mode);
}


int
wpdk_vdprintf(int fd, const char *format, va_list ap)
{
	char buffer[4096];
	int rc;

	wpdk_set_invalid_handler();

	// HACK - temporary implementation
	rc = vsprintf_s(buffer, sizeof(buffer), format, ap);
	return (rc > 0) ? write(fd, buffer, rc) : rc;
}
