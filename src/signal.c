#include <sys/platform.h>
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

    set->bits = ~0;
    return 0;
}


int sigaddset(sigset_t *set, int signo)
{
    // HACK - set errno
    if (!set || signo < 1 || signo >= NSIG) return -1;

    set->bits |= (1 << (signo - 1));
    return 0;
}


int sigdelset(sigset_t *set, int signo)
{
    // HACK - set errno
    if (!set || signo < 1 || signo >= NSIG) return -1;

    set->bits &= ~(1 << (signo - 1));
    return 0;
}


int sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
    // HACK - not implemented
    return -1;
}


int pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
{
    // HACK - not implemented
    return -1;
}


int kill(pid_t pid, int sig)
{
    // HACK - implementation
    return -1;
}
