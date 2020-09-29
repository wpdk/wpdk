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
#include <regex.h>


int
wpdk_regcomp(regex_t *preg, const char *pattern, int cflags)
{
	UNREFERENCED_PARAMETER(preg);
	UNREFERENCED_PARAMETER(pattern);
	UNREFERENCED_PARAMETER(cflags);

	// HACK - not implemented
	WPDK_UNIMPLEMENTED();
	return REG_ESPACE;	
}


int
wpdk_regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags)
{
	UNREFERENCED_PARAMETER(preg);
	UNREFERENCED_PARAMETER(string);
	UNREFERENCED_PARAMETER(nmatch);
	UNREFERENCED_PARAMETER(pmatch);
	UNREFERENCED_PARAMETER(eflags);

	// HACK - not implemented
	WPDK_UNIMPLEMENTED();
	return REG_NOMATCH;	
}


void
wpdk_regfree(regex_t *preg)
{
	UNREFERENCED_PARAMETER(preg);

	// HACK - not implemented
	WPDK_UNIMPLEMENTED();
	return;	
}


size_t
wpdk_regerror(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size)
{
	UNREFERENCED_PARAMETER(errcode);
	UNREFERENCED_PARAMETER(preg);

	if (errbuf && errbuf_size > 0)
		*errbuf = 0;

	// HACK - not implemented
	WPDK_UNIMPLEMENTED();
	return 0;
}
