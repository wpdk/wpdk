#include <sys/platform.h>
#include <sys/wait.h>


inline static pid_t wait(int *stat_loc)
{
    // HACK - implement
    return -1;
}


inline static pid_t waitpid(pid_t pid, int *stat_loc, int options)
{
    // HACK - implement
    return -1;
}