#ifndef _WPDK_SIGNAL_H_
#define _WPDK_SIGNAL_H_

#include <sys/platform.h>
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


int sigaction(int sig, const struct sigaction *act, struct sigaction *oact);

int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signo);
int sigdelset(sigset_t *set, int signo);

int pthread_sigmask(int how, const sigset_t *set, sigset_t *oset);

int kill(pid_t pid, int sig);

_WPDK_END_C_HEADER

#endif /* _WPDK_SIGNAL_H_ */
