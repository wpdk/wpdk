#ifndef _TERMIOS_H_
#define _TERMIOS_H_

#include <sys/platform.h>

_CRT_BEGIN_C_HEADER

typedef unsigned char cc_t;
typedef unsigned int speed_t;
typedef unsigned int tcflag_t;

#define NCCS 16

struct termios {
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t c_cc[NCCS];
};

#define ECHO 1

#define TCSANOW     1
#define TCSADRAIN   2
#define TCSAFLUSH   4

int tcsetattr(int fildes, int optional_actions, const struct termios *termios_p);
int tcgetattr(int fildes, struct termios *termios_p);

#include "../src/termios.c"

_CRT_END_C_HEADER

#endif /* !_TERMIOS_H_ */