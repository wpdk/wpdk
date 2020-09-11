#include <wpdklib.h>
#include <signal.h>


int sigemptyset(sigset_t *set)
{
	// HACK - set errno
	if (!set) return -1;

	set->bits = 0;
	return 0;
}


int sigfillset(sigset_t *set)
{
	// HACK - set errno
	if (!set) return -1;

	set->bits = (uint32_t)~0;
	return 0;
}


int sigaddset(sigset_t *set, int signo)
{
	// HACK - set errno
	if (!set || signo < 1 || signo >= NSIG)
		return -1;

	set->bits |= (1 << (signo - 1));
	return 0;
}


int sigdelset(sigset_t *set, int signo)
{
	// HACK - set errno
	if (!set || signo < 1 || signo >= NSIG)
		return -1;

	set->bits &= ~(1 << (signo - 1));
	return 0;
}


int sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
	UNREFERENCED_PARAMETER(sig);
	UNREFERENCED_PARAMETER(act);
	UNREFERENCED_PARAMETER(oact);

	// HACK - not implemented
	return 0;
}


int pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
{
	UNREFERENCED_PARAMETER(how);
	UNREFERENCED_PARAMETER(set);
	UNREFERENCED_PARAMETER(oset);

	// HACK - not implemented
	return 0;
}


int kill(pid_t pid, int sig)
{
	UNREFERENCED_PARAMETER(pid);
	UNREFERENCED_PARAMETER(sig);

	// HACK - implementation
	return -1;
}
