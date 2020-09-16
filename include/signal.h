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

#ifndef _WPDK_SIGNAL_H_
#define _WPDK_SIGNAL_H_

#include <sys/cdefs.h>
#include <sys/_types.h>
#include <../ucrt/signal.h>
#include <sys/_sigset.h>

_WPDK_BEGIN_C_HEADER

#define SIGHUP		1
#define SIGQUIT		3
#define SIGKILL		9
#define SIGBUS		10
#define SIGPIPE		13
#define SIGALRM		14
#define SIGCHLD		20
#define SIGUSR1		30
#define SIGUSR2		31

#define SIG_BLOCK		1
#define SIG_UNBLOCK		2
#define SIG_SETMASK		3

#define SA_SIGINFO		1

typedef struct siginfo {
	int x;
} siginfo_t;

struct sigaction {
	void (*sa_handler)(int);
	void (*sa_sigaction)(int, siginfo_t *, void *);
	sigset_t sa_mask;
	int     sa_flags;
};


int wpdk_sigaction(int sig, const struct sigaction *act, struct sigaction *oact);
int wpdk_sigemptyset(sigset_t *set);
int wpdk_sigfillset(sigset_t *set);
int wpdk_sigaddset(sigset_t *set, int signo);
int wpdk_sigdelset(sigset_t *set, int signo);
int wpdk_kill(pid_t pid, int sig);

int pthread_sigmask(int how, const sigset_t *set, sigset_t *oset);

#ifndef _WPDK_BUILD_LIB_
#define sigaction(sig,act,oact) wpdk_sigaction(sig,act,oact)
#define sigemptyset(set) wpdk_sigemptyset(set)
#define sigfillset(set) wpdk_sigfillset(set)
#define sigaddset(set,sig) wpdk_sigaddset(set,sig)
#define sigdelset(set,sig) wpdk_sigdelset(set,sig)
#define kill(pid,sig) wpdk_kill(pid,sig)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_SIGNAL_H_ */
