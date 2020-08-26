#include <sys/platform.h>
#include <unistd.h>

inline static pid_t getpid()
{
    return GetCurrentProcessId();
}

inline static int truncate(const char *path, off_t length)
{
    // HACK
    return EINVAL;
}

inline static int ftruncate(int fd, off_t length)
{
    // HACK
    return EINVAL;
}


inline static int usleep(useconds_t useconds)
{
    // HACK - check
    SleepEx((DWORD)(useconds / 1000), TRUE);
    return 0;
}


inline static char *ttyname(int fildes)
{
    static char tty[] = "/dev/tty";
    // HACK - implementation
    return tty;
}


inline static int isatty(int fildes)
{
    // HACk - implementation
    return true;
}


inline static unsigned sleep(unsigned seconds)
{
    // HACK - check
    SleepEx((DWORD)(seconds * 1000), TRUE);
    // HACK - return value is unslept time
    return 0;
}


inline static long sysconf(int name)
{
    // HACK - implement

    if (name == _SC_NPROCESSORS_CONF)
        return 4;

    if (name == _SC_NPROCESSORS_ONLN)
        return 4;

    if (name == _SC_PAGESIZE)
        return 4096;

    return -1;
}


inline static pid_t fork()
{
    // HACK - implement
    return -1;
}
