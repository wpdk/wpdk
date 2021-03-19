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
	fprintf(stderr, "Usage: wpdk_terminate <pid>\n");
	exit(1);
}


int
main(int argc, char **argv)
{
	int pid = (argc > 1) ? atoi(argv[1]) : 0;
	HANDLE h;
	DWORD rc;
	int i;

	if (pid <= 0) usage();

	/*
	 * Terminate the process. On Linux this invokes the SIGTERM handler,
	 * but on Windows TerminateProcess is a hard stop. Provoke a SIGTERM
	 * callback if possible and wait to see if the process exits before
	 * calling TerminateProcess.
	 */
	h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE,
			FALSE, (DWORD)pid);

	if (h == NULL) {
		fprintf(stderr, "wpdk_terminate: (%d) - No such process\n", pid);
		exit(1);
	}

	if (wpdk_kill(pid, SIGTERM) == 0) {
		for (i = 0; i < 250; i++, Sleep(20))
			if (GetExitCodeProcess(h, &rc) != STILL_ACTIVE) {
				CloseHandle(h);
				exit(0);
			}
		}

	TerminateProcess(h, 128 + SIGTERM);
	CloseHandle(h);
	exit(0);
}
