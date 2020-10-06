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
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <io.h>


#define wpdk_calloc __real_calloc


int
wpdk_mkstemp(char *template)
{
	size_t len, pathlen;
	char buf[PATH_MAX];
	char *path;
	int i, fd;

	wpdk_set_invalid_handler();

	len = strnlen_s(template, PATH_MAX);

	if (len < 6 || len >= PATH_MAX || strcmp(&template[len-6], "XXXXXX") != 0)
		return wpdk_posix_error(EINVAL);

	/*
	 *  Another context could create the file between the return from
	 *  _mktemp and the call to _open. Fail the open if the file exists
	 *  and obtain a new pathname. Try this up to 26 times, to cover all
	 *  of the possible temporary files that can be returned.
	 */
	for (i = 0; i < 26; i++) {
		if ((path = wpdk_copy_path(buf, sizeof(buf), template)) == NULL)
			return wpdk_posix_error(EINVAL);

		if (_mktemp(path) == NULL)
			return -1;

		if ((fd = _open(path, O_CREAT|O_EXCL|O_RDWR|_O_BINARY, S_IREAD|S_IWRITE)) != -1) {
			pathlen = strnlen_s(path, sizeof(buf));

			if (pathlen >= 6 && pathlen < sizeof(buf) &&
					strcpy_s(&template[len-6], 7, &path[pathlen-6]) == 0)
				return fd;

			_close(fd);
			_unlink(path);
			return wpdk_posix_error(EINVAL);
		}

		if (errno != EEXIST)
			return -1;
	}

	return wpdk_posix_error(EEXIST);
}


int
wpdk_posix_memalign(void **memptr, size_t alignment, size_t size)
{
	// HACK - not aligned - just basic malloc for now
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(alignment);

	if (!memptr || !size)
		return EINVAL;

	*memptr = malloc(size);

	if (!*memptr)
		return ENOMEM;
	return 0;
}


void
wpdk_srand(unsigned seed)
{
	srand(seed);
}


int
wpdk_rand(void)
{
	/*
	 *  On Windows, RAND_MAX is defined as a 15-bit value, but Linux
	 *  defines it as 31-bits. Return 31-bits for compatibility.
	 */
	return (int)wpdk_random();
}


int
wpdk_rand_r(unsigned int *seedp)
{
	UNREFERENCED_PARAMETER(seedp);

	/*
	 *  POSIX: rand_r should return a reproducible sequence if the
	 *  seed is not modified between calls. There doesn't seem to
	 *  be an exact counterpart to this, so just use wpdk_random().
	 */
	return (int)wpdk_random();
}


void
wpdk_srandom(unsigned int seed)
{
	srand(seed);
}


long
int wpdk_random(void)
{
	/* Return a 31-bit random number */
	int r1 = rand() & 0x7fff;
	int r2 = rand() & 0x7fff;
	int r3 = rand() & 1;
	return (r1 << 16) | (r2 << 1) | r3;
}


void *
wpdk_calloc(size_t nelem, size_t elsize)
{
	return calloc(nelem, elsize);
}
