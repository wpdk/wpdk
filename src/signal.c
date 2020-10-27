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
#include <signal.h>


static struct sigaction wpdk_signals[NSIG];

static struct sigaction wpdk_sig_default = {
	.sa_handler = SIG_DFL
};

static CRITICAL_SECTION wpdk_sig_lock;
static INIT_ONCE wpdk_signal_once = INIT_ONCE_STATIC_INIT;


int
wpdk_sigemptyset(sigset_t *set)
{
	if (!set)
		return wpdk_posix_error(EINVAL);

	set->bits = 0;
	return 0;
}


int
wpdk_sigfillset(sigset_t *set)
{
	if (!set)
		return wpdk_posix_error(EINVAL);

	set->bits = (uint32_t)((1 << (NSIG - 1)) - 1);
	return 0;
}


int
wpdk_sigaddset(sigset_t *set, int signo)
{
	if (!set || signo < 1 || signo >= NSIG)
		return wpdk_posix_error(EINVAL);

	set->bits |= (1 << (signo - 1));
	return 0;
}


int
wpdk_sigdelset(sigset_t *set, int signo)
{
	if (!set || signo < 1 || signo >= NSIG)
		return wpdk_posix_error(EINVAL);

	set->bits &= ~(1 << (signo - 1));
	return 0;
}


int
wpdk_sigismember(const sigset_t *set, int signo)
{
	if (!set || signo < 1 || signo >= NSIG)
		return wpdk_posix_error(EINVAL);

	return ((set->bits & (1 << (signo - 1))) != 0);
}


sighandler_t
wpdk_signal(int sig, sighandler_t handler)
{
	struct sigaction oact, act = { 0 };

	act.sa_handler = handler;
	act.sa_flags = SA_RESETHAND;

	if (wpdk_sigaction(sig, &act, &oact) == -1)
		return SIG_ERR;

	return oact.sa_handler;
}


static BOOL CALLBACK
wpdk_signal_init(INIT_ONCE *pInit, void *arg, void **pContext)
{
	UNREFERENCED_PARAMETER(pInit);
	UNREFERENCED_PARAMETER(arg);

	InitializeCriticalSectionAndSpinCount(&wpdk_sig_lock, 4000);

	*pContext = NULL;
	return TRUE;
}


static void
wpdk_signal_handler(int sig)
{
	ucontext_t context = {0};
	struct sigaction act;
	siginfo_t info = {0};

	if (sig < 1 || sig >= NSIG) {
		WPDK_WARNING("Invalid signal (%d)", sig);
		return;
	}

	EnterCriticalSection(&wpdk_sig_lock);
	act = wpdk_signals[sig];

	if (act.sa_flags & SA_RESETHAND)
		wpdk_signals[sig] = wpdk_sig_default;

	else if (signal(sig, wpdk_signal_handler) == SIG_ERR)
		WPDK_WARNING("Failed to re-instate signal handler");

	LeaveCriticalSection(&wpdk_sig_lock);

	if ((act.sa_flags & SA_SIGINFO) == 0) {
		if (act.sa_handler != SIG_IGN) {
			if (act.sa_handler == SIG_DFL) exit(3);
			else (act.sa_handler)(sig);
		}
		return;
	}

	info.si_signo = sig;
	(act.sa_sigaction)(sig, &info, &context);
}


int
wpdk_sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
	sighandler_t handler = wpdk_signal_handler;
	void *context = NULL;
	int rc = 0;

	if (sig < 1 || sig >= NSIG)
		return wpdk_posix_error(EINVAL);

	wpdk_set_invalid_handler();

	/*
	 *  Initialise a critical section to protect the signal state.
	 *  Critical sections are recursive, so if a signal occurs on the
	 *  thread that is changing the state it won't deadlock.
	 *
	 *  NOTE: The documentation appears to incorrectly state the order
	 *  of the arguments as (..., Context, Parameter).  According to
	 *  the function prototype and the example code, the correct order
	 *  is (..., Parameter, Context).
	 */
	InitOnceExecuteOnce(&wpdk_signal_once, wpdk_signal_init,
			NULL, &context);

	/*
	 *  POSIX: The signal action includes a mask of signals to be
	 *  blocked when the handler is called. There is currently no
	 *  support for this and the mask is ignored.
	 */
	EnterCriticalSection(&wpdk_sig_lock);

	if (oact) *oact = wpdk_signals[sig];

	if (act) {
		wpdk_signals[sig] = *act;

		switch (sig) {
			case SIGABRT:
			case SIGFPE:
			case SIGILL:
			case SIGINT:
			case SIGSEGV:
			case SIGTERM:
				if (act->sa_handler == SIG_DFL || act->sa_handler == SIG_IGN)
					handler = act->sa_handler;

				if (signal(sig, handler) == SIG_ERR) rc = -1;
		}
	}

	LeaveCriticalSection(&wpdk_sig_lock);
	return rc;
}


int
wpdk_pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
{
	UNREFERENCED_PARAMETER(how);
	UNREFERENCED_PARAMETER(set);

	if (oset) wpdk_sigemptyset(oset);

	WPDK_UNIMPLEMENTED();
	return 0;
}


int
wpdk_kill(pid_t pid, int sig)
{
	wpdk_set_invalid_handler();

	if ((pid > 0 && pid != GetCurrentProcessId()) ||
			(pid < -1 && (-pid) != GetCurrentProcessId())) {
		WPDK_UNIMPLEMENTED();
		return wpdk_posix_error(ENOSYS);
	}

	return (sig != 0) ? raise(sig) : 0;
}
