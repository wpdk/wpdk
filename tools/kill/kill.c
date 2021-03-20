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
	HANDLE h;
	int rc;

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
main(int argc, char **argv)
{
	int rc = 0, i = 1;
	int sig = SIGTERM;

	if (argc < 2) usage();

	if (argv[i][0] == '-') {
		if (strcmp(argv[i], "-SIGINT") == 0) sig = SIGINT;
		else if (strcmp(argv[i], "-INT") == 0) sig = SIGINT;
		else if (strcmp(argv[i], "-9") == 0) sig = SIGKILL;
		else if (strcmp(argv[i], "-0") == 0) sig = 0;
		else usage();
		i++;
	}

	for (; i < argc; i++) {
		if (!isdigit(argv[i][0])) usage();
		if (notify_process(atoi(argv[i]), sig) != 0)
			rc = 1;
	}

	exit(rc);
}
