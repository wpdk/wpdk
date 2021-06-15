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

#include <wpdk/header.h>

#ifdef _WPDK_INCLUDE_NEXT_
_WPDK_INCLUDE_NEXT_
#include_next <signal.h>
#else
#include <../ucrt/signal.h>
#endif

#ifndef _WPDK_SIGNAL_H_
#define _WPDK_SIGNAL_H_

#include <sys/types.h>

_WPDK_BEGIN_C_HEADER

#define SIGHUP		1
#define SIGQUIT		3
#define SIGBUS		7
#define SIGKILL		9
#define SIGUSR1		10
#define SIGUSR2		12
#define SIGPIPE		13
#define SIGALRM		14
#define SIGCHLD		17

#define SIG_BLOCK		1
#define SIG_UNBLOCK		2
#define SIG_SETMASK		3

#define SA_SIGINFO		1
#define SA_RESETHAND	2

typedef struct sigset {
	uint32_t bits;
} sigset_t;

typedef struct siginfo {
	int		si_signo;
	int		si_code;
	void	*si_addr;
} siginfo_t;

typedef struct ucontext {
	int x;
} ucontext_t;

struct sigaction {
	union {
		void (*sighandler)(int);
		void (*sigaction)(int, siginfo_t *, void *);
	} u;
	sigset_t sa_mask;
	int     sa_flags;
};

#define sa_handler		u.sighandler
#define sa_sigaction	u.sigaction

typedef void (*sighandler_t)(int);

int wpdk_sigaction(int sig, const struct sigaction *act, struct sigaction *oact);
sighandler_t wpdk_signal(int sig, sighandler_t handler);
int wpdk_sigemptyset(sigset_t *set);
int wpdk_sigfillset(sigset_t *set);
int wpdk_sigaddset(sigset_t *set, int signo);
int wpdk_sigdelset(sigset_t *set, int signo);
int wpdk_sigismember(const sigset_t *set, int signo);
int wpdk_kill(pid_t pid, int sig);

#undef pthread_sigmask
int wpdk_pthread_sigmask(int how, const sigset_t *set, sigset_t *oset);

#ifndef _WPDK_BUILD_LIB_
#define sigaction(sig,act,oact) wpdk_sigaction(sig,act,oact)
#define signal(sig,handler) wpdk_signal(sig,handler)
#define sigemptyset(set) wpdk_sigemptyset(set)
#define sigfillset(set) wpdk_sigfillset(set)
#define sigaddset(set,sig) wpdk_sigaddset(set,sig)
#define sigdelset(set,sig) wpdk_sigdelset(set,sig)
#define sigismember(set,sig) wpdk_sigismember(set,sig)
#define kill(pid,sig) wpdk_kill(pid,sig)
#define pthread_sigmask(how,set,oset) wpdk_pthread_sigmask(how,set,oset)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_SIGNAL_H_ */
