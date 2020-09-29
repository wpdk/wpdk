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

#ifndef _WPDK_REGEX_H_
#define _WPDK_REGEX_H_

#include <wpdk/header.h>
#include <wpdk/types.h>

_WPDK_BEGIN_C_HEADER

#define REG_NOMATCH		1
#define REG_ESPACE		2

typedef ssize_t regoff_t;

typedef struct regex_s {
	size_t	re_nsub;
} regex_t;

typedef struct regmatch_s {
	regoff_t	rm_so;
	regoff_t	rm_eo;
} regmatch_t;

int wpdk_regcomp(regex_t *preg, const char *pattern, int cflags);
size_t wpdk_regerror(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size);
int wpdk_regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags);
void wpdk_regfree(regex_t *preg);

#ifndef _WPDK_BUILD_LIB_
#define regcomp(preg,pat,cflags) wpdk_regcomp(preg,pat,cflags)
#define regerr(err,preg,buf,size) wpdk_regerr(err,preg,buf,size)
#define regexec(preg,str,n,pmatch,eflags) wpdk_regexec(preg,str,n,pmatch,eflags)
#define regfree(preg) wpdk_regex(preg)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_REGEX_H_ */
