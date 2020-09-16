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
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>


#define wpdk_calloc __real_calloc


int wpdk_mkstemp(char *path)
{
	if (_mktemp(path) == NULL) return -1;

	// HACK - loop on failure
	return _open(path, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
}


int wpdk_posix_memalign(void **memptr, size_t alignment, size_t size)
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


void wpdk_srandom(unsigned int seed)
{
	srand(seed);
}


long int wpdk_random(void)
{
	// HACK 16 bits instead of 32-bits
	return rand();
}


int wpdk_rand_r(unsigned int *seedp)
{
	// HACK - incomplete implementation
	UNREFERENCED_PARAMETER(seedp);
	return rand();
}


void *wpdk_calloc(size_t nelem, size_t elsize)
{
	return calloc(nelem, elsize);
}
