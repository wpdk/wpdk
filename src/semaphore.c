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

#include <wpdklib.h>
#include <limits.h>
#include <semaphore.h>


int wpdk_sem_init(sem_t *sem, int pshared, unsigned int value)
{
	// HACK - sem_init pshared
	UNREFERENCED_PARAMETER(pshared);

	if (!sem) return EINVAL;

	sem->h = CreateSemaphore(NULL, value, INT_MAX, NULL);

	if (sem->h == NULL)	{
		// HACK - improve error handling
		return EINVAL;
	}

	return 0;
}

int wpdk_sem_destroy(sem_t *sem)
{
	if (!sem || !sem->h)
		return EINVAL;

	CloseHandle(sem->h);

	sem->h = NULL;
	return 0;
}

int wpdk_sem_post(sem_t *sem)
{
	if (!sem || !sem->h)
		return EINVAL;

	if (ReleaseSemaphore(sem->h, 1, NULL) == 0) {
		// HACK - error
		return EINVAL;
	}

	return 0;
}

int wpdk_sem_wait(sem_t *sem)
{
	if (!sem || !sem->h)
		return EINVAL;

	if (WaitForSingleObject(sem->h, INFINITE) != WAIT_OBJECT_0) {
		// HACK - error
		return EINVAL;
	}

	return 0;
}

int wpdk_sem_trywait(sem_t *sem)
{
	if (!sem || !sem->h)
		return EINVAL;

	if (WaitForSingleObject(sem->h, INFINITE) != WAIT_OBJECT_0) {
		// HACK - error
		return EINVAL;
	}

	return 0;
}

int wpdk_sem_timedwait(sem_t *sem, const struct timespec *abs_timeout)
{
	// HACK - handle timed wait
	UNREFERENCED_PARAMETER(abs_timeout);
	return wpdk_sem_wait(sem);
}
