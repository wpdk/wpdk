#include <sys/platform.h>
#include <pthread.h>


inline static int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    if (!attr) return EINVAL;

    attr->type = PTHREAD_MUTEX_DEFAULT;
    attr->robust = PTHREAD_MUTEX_STALLED;
    attr->pshared = PTHREAD_PROCESS_PRIVATE;
    return 0;
}


inline static int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    if (!attr) return EINVAL;

    return 0;
}


inline static int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type)
{
    if (!attr || !type)
        return EINVAL;

    *type = attr->type;
    return 0;
}


inline static int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    if (!attr || type < PTHREAD_MUTEX_NORMAL || type > PTHREAD_MUTEX_RECURSIVE)
        return EINVAL;

    attr->type = type;
    return 0;
}


inline static int pthread_mutexattr_getrobust(const pthread_mutexattr_t *attr, int *robust)
{
    if (!attr || !robust)
        return EINVAL;

    *robust = attr->robust;
    return 0;
}


inline static int pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust)
{
    if (!attr || robust < PTHREAD_MUTEX_STALLED || robust > PTHREAD_MUTEX_ROBUST)
        return EINVAL;

    attr->robust = robust;
    return 0;
}


inline static int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr, int *pshared)
{
    if (!attr || !pshared)
        return EINVAL;

    *pshared = attr->pshared;
    return 0;
}


inline static int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared)
{
    if (!attr || pshared < PTHREAD_PROCESS_PRIVATE || pshared > PTHREAD_PROCESS_SHARED)
        return EINVAL;

    attr->pshared = pshared;
    return 0;
}


inline static int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr)
{
    if (!mutex || !mutexattr || mutexattr->type != PTHREAD_MUTEX_NORMAL)
        return EINVAL;

    // HACK - and spin count!
    InitializeCriticalSection(&mutex->lock);
    return 0;
}

inline static int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    if (!mutex) return EINVAL;

    DeleteCriticalSection(&mutex->lock);
    return 0;
}


inline static int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    if (!mutex) return EINVAL;

    EnterCriticalSection(&mutex->lock);
    return 0;
}


inline static int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    if (!mutex) return EINVAL;

    if (TryEnterCriticalSection(&mutex->lock) == 0)
        return EBUSY;

    return 0;
}


inline static int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    if (!mutex) return EINVAL;

    LeaveCriticalSection(&mutex->lock);
    return 0;
}


inline static int pthread_mutex_consistent(pthread_mutex_t *mutex)
{
    //HACK - not implemented
    return 0;
}


inline static int pthread_spin_init(pthread_spinlock_t *lock, int pshared)
{
    if (!lock || pshared < PTHREAD_PROCESS_PRIVATE || pshared > PTHREAD_PROCESS_SHARED)
        return EINVAL;

    // HACK - will eventually block
    InitializeCriticalSectionAndSpinCount(&lock->lock, 32000);
    return 0;
}


inline static int pthread_spin_destroy(pthread_spinlock_t *lock)
{
    if (!lock) return EINVAL;

    DeleteCriticalSection(&lock->lock);
    return 0;
}


inline static int pthread_spin_lock(pthread_spinlock_t *lock)
{
    if (!lock) return EINVAL;

    EnterCriticalSection(&lock->lock);
    return 0;
}


inline static int pthread_spin_trylock(pthread_spinlock_t *lock)
{
    if (!lock) return EINVAL;

    if (TryEnterCriticalSection(&lock->lock) == 0)
        return EBUSY;

    return 0;
}


inline static int pthread_spin_unlock(pthread_spinlock_t *lock)
{
    if (!lock) return EINVAL;

    LeaveCriticalSection(&lock->lock);
    return 0;
}


inline static int pthread_attr_init(pthread_attr_t *attr)
{
    if (!attr) return EINVAL;
    return 0;
}


inline static int pthread_attr_destroy(pthread_attr_t *attr)
{
    if (!attr) return EINVAL;
    return 0;
}


inline static pthread_t pthread_self()
{
    return GetCurrentThreadId();
}


inline static int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg)
{
    // HACK - check implementation
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, (LPDWORD)thread);

	if (hThread == NULL) {
        // HACK - error code
        return EINVAL;
    }

    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
    return 0;
}


inline static int pthread_join(pthread_t thread, void **value_ptr)
{
    // HACK - unimplemented
    return EINVAL;
}


inline static int pthread_detach(pthread_t thread)
{
    // HACK - unimplemented
    return EINVAL;
}


inline static int pthread_cancel(pthread_t thread)
{
    // HACK - unimplemented
    return EINVAL;
}


inline static void pthread_exit(void *value_ptr)
{
    // HACK - unimplemented
}