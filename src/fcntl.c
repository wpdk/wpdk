#include <sys/platform.h>
#include <fcntl.h>

inline static int fcntl(int fildes, int cmd, ...)
{
    // HACK - fcntl is used on sockets to set nonblock
    return EINVAL;
}