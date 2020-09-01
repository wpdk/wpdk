#include <sys/platform.h>
#include <sys/mman.h>


void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
{
    // HACK
    return NULL;
}


int munmap(void *addr, size_t len)
{
    // HACK
    return EINVAL;
}


int shm_open(const char *name, int oflag, mode_t mode)
{
    // HACK
    return EINVAL;
}


int shm_unlink(const char *name)
{
    // HACK
    return EINVAL;
}

