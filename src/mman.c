#include <sys/platform.h>
#include <sys/mman.h>


inline static void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
{
    // HACK
    return EINVAL;
}


inline static int munmap(void *addr, size_t)
{
    // HACK
    return EINVAL;
}


inline static int shm_open(const char *name, int oflag, mode_t mode)
{
    // HACK
    return EINVAL;
}


inline static int shm_unlink(const char *name)
{
    // HACK
    return EINVAL;
}

