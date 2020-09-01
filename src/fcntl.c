#include <sys/platform.h>
#include <fcntl.h>

int fcntl(int fildes, int cmd, ...)
{
    // HACK - fcntl is used on sockets to set nonblock
    return EINVAL;
}