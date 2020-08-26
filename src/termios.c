#include <sys/platform.h>
#include <termios.h>


static inline int tcsetattr(int fildes, int optional_actions, const struct termios *termios_p)
{
    // HACK - implementation
    return -1;
}


static inline int tcgetattr(int fildes, struct termios *termios_p)
{
    // HACK - implementation
    return -1;
}