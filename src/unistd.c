#include <sys/platform.h>
#include <stdbool.h>
#include <unistd.h>

pid_t getpid()
{
    return GetCurrentProcessId();
}

int truncate(const char *path, off_t length)
{
    // HACK
    return EINVAL;
}

int ftruncate(int fd, off_t length)
{
    // HACK
    return EINVAL;
}


int usleep(useconds_t useconds)
{
    // HACK - check
    // HACK - it's sleeping way too long - null it out for now
    //SleepEx((DWORD)(useconds / 1000), TRUE);
    return 0;
}


char *ttyname(int fildes)
{
    static char tty[] = "/dev/tty";
    // HACK - implementation
    return tty;
}


int isatty(int fildes)
{
    // HACk - implementation
    return true;
}


unsigned sleep(unsigned seconds)
{
    // HACK - check
    SleepEx((DWORD)(seconds * 1000), TRUE);
    // HACK - return value is unslept time
    return 0;
}


long sysconf(int name)
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


pid_t fork()
{
    // HACK - implement
    return -1;
}


int daemon(int nochdir, int noclose)
{
    // HACK - implement
    return -1;
}


ssize_t read(int fildes, void *buf, size_t nbyte)
{
    // HACK - off_t is 32 bits
    return _read(fildes, buf, nbyte);
}


ssize_t write(int fildes, const void *buf, size_t nbyte)
{
    // HACK - off_t is 32 bits ???
    return _write(fildes, buf, nbyte);
}


off_t lseek(int fildes, off_t offset, int whence)
{
    // HACK - off_t is 32 bits
    return _lseek(fildes, offset, whence);
}


int __real_unlink(const char *path)
{
    return _unlink(path);
}


int access(const char *pathname, int mode)
{
    return _access(pathname, mode);
}


int close(int fildes)
{
    return _close(fildes);
}
