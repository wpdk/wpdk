#include <sys/platform.h>
#include <sys/ioctl.h>


inline static int ioctl(int fd, unsigned long request, ...)
{
    // HACK - not implemented
    return EINVAL;
}