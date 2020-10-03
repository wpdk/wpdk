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

#ifndef _WPDK_SYSLOG_H_
#define _WPDK_SYSLOG_H_

#include <wpdk/header.h>

_WPDK_BEGIN_C_HEADER

#define LOG_EMERG	0
#define LOG_ALERT	1
#define LOG_CRIT	2
#define LOG_ERR		3
#define LOG_WARNING	4
#define LOG_NOTICE	5
#define LOG_INFO	6
#define LOG_DEBUG	7

#define LOG_LOCAL7	(1<<3)
#define LOG_PID		1

void wpdk_openlog(const char *ident, int option, int facility);
void wpdk_syslog(int priority, const char *format, ...);
void wpdk_closelog(void);

#ifndef _WPDK_BUILD_LIB_
#define openlog(ident,opt,facility) wpdk_openlog(ident,opt,facility)
#define syslog(prio,fmt,...) wpdk_syslog(prio,fmt,__VA_ARGS__)
#define closelog() wpdk_closelog()
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_SYSLOG_H_ */
