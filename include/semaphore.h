#ifndef _WPDK_SEMAPHORE_H_
#define	_WPDK_SEMAPHORE_H_

#include <sys/platform.h>
#include <time.h>

_WPDK_BEGIN_C_HEADER

typedef struct { HANDLE h; } sem_t;

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_destroy(sem_t *sem);
int sem_post(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

_WPDK_END_C_HEADER

#endif /* !_WPDK_SEMAPHORE_H_ */