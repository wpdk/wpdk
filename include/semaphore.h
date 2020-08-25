#ifndef _SEMAPHORE_H_
#define	_SEMAPHORE_H_

#include <sys/platform.h>
#include <time.h>

_CRT_BEGIN_C_HEADER

typedef struct { HANDLE h; } sem_t;

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_destroy(sem_t *sem);
int sem_post(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

#include "../src/semaphore.c"

_CRT_END_C_HEADER

#endif /* !_SEMAPHORE_H_ */