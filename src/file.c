#include <sys/platform.h>
#include <sys/file.h>


int flock(int fd, int operation)
{
    // HACK - not implemented
    return EINVAL;
}
