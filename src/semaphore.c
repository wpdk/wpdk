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
#include <sys/time.h>
#include <limits.h>
#include <semaphore.h>


int
wpdk_sem_init(sem_t *sem, int pshared, unsigned int value)
{
	if (!sem || value > INT_MAX)
		return wpdk_posix_error(EINVAL);

	if (pshared) {
		WPDK_UNIMPLEMENTED();
		return wpdk_posix_error(ENOSYS);
	}

	sem->h = CreateSemaphore(NULL, value, INT_MAX, NULL);

	if (sem->h == NULL)
		return wpdk_last_error();

	return 0;
}


int
wpdk_sem_destroy(sem_t *sem)
{
	if (!sem || !sem->h)
		return wpdk_posix_error(EINVAL);

	CloseHandle(sem->h);

	sem->h = NULL;
	return 0;
}


int
wpdk_sem_post(sem_t *sem)
{
	if (!sem || !sem->h)
		return wpdk_posix_error(EINVAL);

	if (ReleaseSemaphore(sem->h, 1, NULL) == 0)
		return wpdk_last_error();

	return 0;
}


static int
wpdk_wait_sem(sem_t *sem, DWORD msecs)
{
	int rc;

	if (!sem || !sem->h)
		return wpdk_posix_error(EINVAL);

	rc = WaitForSingleObject(sem->h, msecs);

	switch (rc) {
		case WAIT_FAILED:
			return wpdk_last_error();

		case WAIT_ABANDONED:
			WPDK_WARNING("Semaphore abandoned");
			return 0;

		case WAIT_OBJECT_0:
			return 0;

		case WAIT_TIMEOUT:
			return wpdk_posix_error(EAGAIN);	
	}

	return wpdk_posix_error(EINVAL);
}


int
wpdk_sem_wait(sem_t *sem)
{
	return wpdk_wait_sem(sem, INFINITE);
}


int
wpdk_sem_trywait(sem_t *sem)
{
	return wpdk_wait_sem(sem, 0);
}


int
wpdk_sem_timedwait(sem_t *sem, const struct timespec *abs_timeout)
{
	DWORD msecs;

	if (wpdk_abstime_to_msecs(abs_timeout, &msecs) == -1)
		return -1;

	return wpdk_wait_sem(sem, msecs);
}
