#include <sys/platform.h>
#include <sys/stat.h>


inline static int mknod(const char *path, mode_t mode, dev_t dev)
{
    // HACK - implementation
    return EINVAL;
}

inline static int stat(const char *path, struct stat *buf)
{
    // HACK - implementation
    return EINVAL;
}


inline static int fstat(int fildes, struct stat *buf)
{
    // HACK - implementation
    return EINVAL;
}