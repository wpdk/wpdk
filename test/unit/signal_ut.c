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
#include <stdlib.h>
#include <stdio.h>

#include <CUnit/Basic.h>


static int
null_init(void)
{
	return 0;
}


static int
null_clean(void)
{
	return 0;
}


static void
test_sigset(void)
{
	sigset_t set, full, empty;
	int i, rc;

	/* Check sigemptyset */
	rc = sigemptyset(&empty);
	CU_ASSERT(rc == 0);

	/* Check null sigemptyset */
	rc = sigemptyset(NULL);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check sigfillset */
	rc = sigfillset(&full);
	CU_ASSERT(rc == 0);

	/* Check null sigfillset */
	rc = sigfillset(NULL);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check null sigaddset */
	rc = sigaddset(NULL, SIGINT);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid sigaddset */
	rc = sigaddset(&set, 0);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid sigaddset */
	rc = sigaddset(&set, 123);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check all bits */
	rc = sigemptyset(&set);
	CU_ASSERT(rc == 0);
	for (i = 1; i < NSIG; i++) {
		rc = sigaddset(&set, i);
		CU_ASSERT(rc == 0);
	}
	CU_ASSERT(memcmp(&set, &full, sizeof(set)) == 0);

	/* Check null sigdelset */
	rc = sigdelset(NULL, SIGINT);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid sigdelset */
	rc = sigdelset(&set, 0);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid sigdelset */
	rc = sigdelset(&set, 123);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check all bits */
	rc = sigfillset(&set);
	CU_ASSERT(rc == 0);
	for (i = 1; i < NSIG; i++) {
		rc = sigdelset(&set, i);
		CU_ASSERT(rc == 0);
	}
	CU_ASSERT(memcmp(&set, &empty, sizeof(set)) == 0);

	/* Check sigismember */
	for (i = 1; i < NSIG; i++) {
		CU_ASSERT(sigismember(&full, i) == 1);
		CU_ASSERT(sigismember(&empty, i) == 0);
	}
}


static int test_signal_count[NSIG];

static void
test_signal_handler(int sig)
{
	test_signal_count[sig]++;
}


static void
test_signal(void)
{
	sighandler_t prev;
	int rc;

	/* Check default handler */
	prev = signal(SIGABRT, SIG_IGN);
	CU_ASSERT(prev == SIG_DFL);

	/* Check kill ignored */ 
	test_signal_count[SIGABRT] = 0;
	kill(0, SIGABRT);
	CU_ASSERT(test_signal_count[SIGABRT] == 0);

	/* Check custom handler */
	prev = signal(SIGABRT, test_signal_handler);
	CU_ASSERT(prev == SIG_IGN);

	/* Check kill */ 
	test_signal_count[SIGABRT] = 0;
	rc = kill(0, SIGABRT);
	CU_ASSERT(rc == 0);
	CU_ASSERT(test_signal_count[SIGABRT] == 1);

	/* Check handler reset */
	prev = signal(SIGABRT, SIG_DFL);
	CU_ASSERT(prev == SIG_DFL);

	/* Check zero signal */
	prev = signal(0, SIG_DFL);
	CU_ASSERT(prev == SIG_ERR && errno == EINVAL);

	/* Check invalid signal */
	prev = signal(NSIG, SIG_DFL);
	CU_ASSERT(prev == SIG_ERR && errno == EINVAL);
}


static void
test_sigaction_handler(int sig, siginfo_t *info, void *context)
{
	if (info && info->si_signo == sig && context)
		test_signal_count[sig]++;
}


static void
test_sigaction(void)
{
	struct sigaction act = { 0 };
	struct sigaction oact;
	int rc;

	/* Check default handler */
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	rc = sigaction(SIGABRT, &act, &oact);
	CU_ASSERT(rc == 0);
	CU_ASSERT(oact.sa_handler == SIG_DFL);

	/* Check kill ignored */ 
	test_signal_count[SIGABRT] = 0;
	kill(0, SIGABRT);
	CU_ASSERT(test_signal_count[SIGABRT] == 0);

	/* Check custom handler */
	act.sa_handler = test_signal_handler;
	act.sa_flags = 0;
	rc = sigaction(SIGABRT, &act, &oact);
	CU_ASSERT(rc == 0);
	CU_ASSERT(oact.sa_handler == SIG_IGN);

	/* Check kill */ 
	test_signal_count[SIGABRT] = 0;
	kill(0, SIGABRT);
	CU_ASSERT(test_signal_count[SIGABRT] == 1);

	/* Check handler not reset */
	act.sa_handler = SIG_DFL;
	act.sa_flags = 0;
	rc = sigaction(SIGABRT, &act, &oact);
	CU_ASSERT(rc == 0);
	CU_ASSERT(oact.sa_handler == test_signal_handler);

	/* Check sigaction handler */
	act.sa_sigaction = test_sigaction_handler;
	act.sa_flags = SA_SIGINFO;
	rc = sigaction(SIGABRT, &act, &oact);
	CU_ASSERT(rc == 0);
	CU_ASSERT(oact.sa_handler == SIG_DFL);

	/* Check kill */ 
	test_signal_count[SIGABRT] = 0;
	rc = kill(0, SIGABRT);
	CU_ASSERT(rc == 0);
	CU_ASSERT(test_signal_count[SIGABRT] == 1);

	/* Check handler not reset */
	act.sa_handler = SIG_DFL;
	act.sa_flags = 0;
	rc = sigaction(SIGABRT, &act, &oact);
	CU_ASSERT(rc == 0);
	CU_ASSERT(oact.sa_sigaction == test_sigaction_handler);
	CU_ASSERT((oact.sa_flags & SA_SIGINFO) != 0);

	/* Check zero signal */
	act.sa_handler = SIG_DFL;
	act.sa_flags = 0;
	rc = sigaction(0, &act, &oact);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid signal */
	act.sa_handler = SIG_DFL;
	act.sa_flags = 0;
	rc = sigaction(NSIG, &act, &oact);
	CU_ASSERT(rc == -1 && errno == EINVAL);
}


static void
test_kill(void)
{
	struct sigaction act = { 0 };
	struct sigaction oact;
	int rc;

	/* Check custom handler */
	act.sa_handler = test_signal_handler;
	act.sa_flags = 0;
	rc = sigaction(SIGABRT, &act, &oact);
	CU_ASSERT(rc == 0);
	CU_ASSERT(oact.sa_handler == SIG_DFL);

	/* Check zero kill */
	rc = kill(0, 0);
	CU_ASSERT(rc == 0);

	/* Check invalid kill */
	rc = kill(0, NSIG);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid process */
	rc = kill(GetCurrentProcessId() + 1, SIGABRT);
	CU_ASSERT(rc == -1 && errno == ENOSYS);

	/* Check kill */
	test_signal_count[SIGABRT] = 0;
	rc = kill(0, SIGABRT);
	CU_ASSERT(rc == 0);
	CU_ASSERT(test_signal_count[SIGABRT] == 1);

	/* Check kill process */
	test_signal_count[SIGABRT] = 0;
	rc = kill(GetCurrentProcessId(), SIGABRT);
	CU_ASSERT(rc == 0);
	CU_ASSERT(test_signal_count[SIGABRT] == 1);

	/* Check kill group */
	test_signal_count[SIGABRT] = 0;
	rc = kill(-(int)GetCurrentProcessId(), SIGABRT);
	CU_ASSERT(rc == 0);
	CU_ASSERT(test_signal_count[SIGABRT] == 1);

	/* Check kill all */
	test_signal_count[SIGABRT] = 0;
	rc = kill(-1, SIGABRT);
	CU_ASSERT(rc == 0);
	CU_ASSERT(test_signal_count[SIGABRT] == 1);

	/* Reset handler */
	act.sa_handler = SIG_DFL;
	act.sa_flags = 0;
	rc = sigaction(SIGABRT, &act, &oact);
	CU_ASSERT(rc == 0);
	CU_ASSERT(oact.sa_handler == test_signal_handler);
}


void add_signal_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("signal", null_init, null_clean);

	CU_ADD_TEST(suite, test_sigset);
	CU_ADD_TEST(suite, test_signal);
	CU_ADD_TEST(suite, test_sigaction);
	CU_ADD_TEST(suite, test_kill);
}
