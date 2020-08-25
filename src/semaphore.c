#include <sys/platform.h>
#include <semaphore.h>


inline static int sem_init(sem_t *sem, int pshared, unsigned int value)
{
    if (!sem) return EINVAL;

    sem->h = CreateSemaphore(NULL, value, INT_MAX, NULL);

    if (sem->h == NULL) {
        // HACK - improve error handling
        return EINVAL;
    }

    return 0;
}


inline static int sem_destroy(sem_t *sem)
{
    if (!sem || !sem->h)
        return EINVAL;

    CloseHandle(sem->h);

    sem->h = NULL;
    return 0;
}


inline static int sem_post(sem_t *sem)
{
    if (!sem || !sem->h)
        return EINVAL;

    if (ReleaseSemaphore(sem->h, 1, NULL) == 0) {
        // HACK - error
        return EINVAL;
    }

    return 0;
}


inline static int sem_wait(sem_t *sem)
{
    if (!sem || !sem->h)
        return EINVAL;

    if (WaitForSingleObject(sem->h, INFINITE) != WAIT_OBJECT_0) {
        // HACK - error
        return EINVAL;
    }

    return 0;
}


inline static int sem_trywait(sem_t *sem)
{
    if (!sem || !sem->h)
        return EINVAL;
        
    if (WaitForSingleObject(sem->h, INFINITE) != WAIT_OBJECT_0) {
        // HACK - error
        return EINVAL;
    }

    return 0;    
}


inline static int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout)
{
    // HACK - handle timed wait
    return sem_wait(sem);
}