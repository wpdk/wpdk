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
#include <io.h>


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
	int rc, error;
	int newfd;
	FILE *fp;

	wpdk_set_invalid_handler();

	/*
	 *  This could be implemented using vsprintf but it is impossible
	 *  to know the required buffer size until the output is formatted.
	 *  Instead, duplicate the file handle, calling fdopen and vfprintf.
	 *  This is functionally correct and vdprintf is unlikely to be
	 *  performance critical.
	 */
	if ((newfd = _dup(fd)) == -1)
		return wpdk_posix_error(EBADF);

	if ((fp = _fdopen(newfd, "r+")) == NULL)
		return -1;

	rc = vfprintf(fp, format, ap);

	error = errno;
	fclose(fp);

	_set_errno(error);
	return rc;
}


int
wpdk_fileno(FILE *stream)
{
	wpdk_set_invalid_handler();

	if (stream == NULL)
		return wpdk_posix_error(EINVAL);

	return _fileno(stream);
}
