#include <sys/platform.h>
#include <poll.h>


inline static int poll(struct pollfd fds[], nfds_t nfds, int timeout)
{
    // HACK - needs to call WSAPoll
    return EINVAL;
}
