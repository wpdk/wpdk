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
#include <io.h>


#define wpdk_calloc __real_calloc


int
wpdk_mkstemp(char *path)
{
	if (_mktemp(path) == NULL) return -1;

	// HACK - loop on failure
	return _open(path, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
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
