/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2021, MayaData Inc. All rights reserved.
 *  Copyright (c) 2021, DataCore Software Corporation. All rights reserved.
 * 
 *  POSIX details are based on the Open Group Base Specification Issue 7,
 *  2018 edition at https://pubs.opengroup.org/onlinepubs/9699919799/
 * 
 *  Details about Linux extensions are based on the Linux man-pages project
 *  at https://www.kernel.org/doc/man-pages/
 */
#include <windows.h>
#include <signal.h>
#include <stdio.h>


struct siglist_s {
	char *name;
	int num;
};

static struct siglist_s siglist[] = {
	{ "SIGTERM", SIGTERM },
	{ "SIGKILL", SIGKILL },
	{ "SIGINT", SIGINT },
};


void
usage()
{
	fprintf(stderr, "Usage: wpdk_kill.exe [-<sig>] <pid>\n");
	exit(1);
}


int
process_exited(HANDLE h)
{
	DWORD rc;
	int i;

	for (i = 0; i < 250; i++, Sleep(20))
		if (GetExitCodeProcess(h, &rc) != STILL_ACTIVE)
			return 1;

	return 0;
}


int
notify_process(int pid, int sig)
{
	int rc = 1;
	HANDLE h;

	h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE,
			FALSE, (DWORD)pid);

	if (h == NULL) {
		fprintf(stderr, "wpdk_kill.exe: (%d) - No such process\n", pid);
		return 1;
	}

	switch (sig) {
	case SIGTERM:
		/*
		* On Linux SIGTERM invokes a signal handler, but on Windows
		* TerminateProcess is a hard stop, so send the signal and wait
		* to see if the process exits before calling TerminateProcess.
		*/
		rc = wpdk_kill(pid, sig);

		if (rc != 0 || !process_exited(h))
			rc = (TerminateProcess(h, 128 + sig) != 0);
		break;

	case SIGKILL:
		rc = (TerminateProcess(h, 128 + sig) != 0);
		break;

	case SIGINT:
		rc = wpdk_kill(pid, sig);
		break;

	case 0:
		rc = 0;
		break;
	}

	CloseHandle(h);
	return rc;
}


int
arg_to_signum(char *arg)
{
	size_t i;

	if (isdigit(arg[0])) return atoi(&arg[0]);

	for (i = 0; i < sizeof(siglist) / sizeof(siglist[0]); i++)
		if (_stricmp(arg, siglist[i].name) == 0 ||
				_stricmp(arg, &siglist[i].name[3]) == 0)
			return siglist[i].num;

	usage();
	return 0;
}


int
main(int argc, char **argv)
{
	int rc = 1, i, expectpid = 0;
	int sig = SIGTERM;

	if (argc < 2) usage();

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-' && !expectpid) {
			if (strcmp(argv[i], "-s") != 0)
				sig = arg_to_signum(&argv[i][1]);
			else if (i + 1 < argc)
				sig = arg_to_signum(argv[++i]);
			else usage();
		}
		else if (isdigit(argv[i][0])) {
			if (notify_process(atoi(argv[i]), sig) == 0)
				rc = 0;
			expectpid = 1;
		}
		else usage();
	}

	exit(rc);
}
