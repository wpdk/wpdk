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
#include <signal.h>


int wpdk_sigemptyset(sigset_t *set)
{
	if (!set)
		return wpdk_posix_error(EINVAL);

	set->bits = 0;
	return 0;
}


int wpdk_sigfillset(sigset_t *set)
{
	if (!set)
		return wpdk_posix_error(EINVAL);

	set->bits = (uint32_t)~0;
	return 0;
}


int wpdk_sigaddset(sigset_t *set, int signo)
{
	if (!set || signo < 1 || signo >= NSIG)
		return wpdk_posix_error(EINVAL);

	set->bits |= (1 << (signo - 1));
	return 0;
}


int wpdk_sigdelset(sigset_t *set, int signo)
{
	if (!set || signo < 1 || signo >= NSIG)
		return wpdk_posix_error(EINVAL);

	set->bits &= ~(1 << (signo - 1));
	return 0;
}


int wpdk_sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
	// HACK - not implemented
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(sig);
	UNREFERENCED_PARAMETER(act);
	UNREFERENCED_PARAMETER(oact);
	return 0;
}


int wpdk_pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
{
	// HACK - not implemented
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(how);
	UNREFERENCED_PARAMETER(set);
	UNREFERENCED_PARAMETER(oset);
	return 0;
}


int wpdk_kill(pid_t pid, int sig)
{
	// HACK - implementation
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(pid);
	UNREFERENCED_PARAMETER(sig);

	return wpdk_posix_error(ENOSYS);
}
