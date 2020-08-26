#include <sys/platform.h>
#include <sys/stat.h>


inline static int mknod(const char *path, mode_t mode, dev_t dev)
{
    // HACK - implementation
    return EINVAL;
}
