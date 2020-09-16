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

#ifndef _WPDK_TERMIOS_H_
#define _WPDK_TERMIOS_H_

#include <sys/cdefs.h>

_WPDK_BEGIN_C_HEADER

typedef unsigned char cc_t;
typedef unsigned int speed_t;
typedef unsigned int tcflag_t;

#define NCCS 16

struct termios {
	tcflag_t c_iflag;
	tcflag_t c_oflag;
	tcflag_t c_cflag;
	tcflag_t c_lflag;
	cc_t c_cc[NCCS];
};

#define ECHO		1
#define ICANON		2

#define TCSANOW		1
#define TCSADRAIN	2
#define TCSAFLUSH	4

int wpdk_tcsetattr(int fildes, int optional_actions, const struct termios *termios_p);
int wpdk_tcgetattr(int fildes, struct termios *termios_p);

#ifndef _WPDK_BUILD_LIB_
#define tcsetattr(fd,opt,term) wpdk_tcsetattr(fd,opt,term)
#define tcgetattr(fd,term) wpdk_tcgetattr(fd,term)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_TERMIOS_H_ */
