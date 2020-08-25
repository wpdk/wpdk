#ifndef _UNISTD_H_
#define	_UNISTD_H_

#include <sys/platform.h>
#include <sys/time.h>
#include <io.h>

_CRT_BEGIN_C_HEADER

#define F_OK    1
#define X_OK    2
#define W_OK    4
#define R_OK    8

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

#define _SC_NPROCESSORS_CONF    1
#define _SC_NPROCESSORS_ONLN    2
#define _SC_PAGESIZE            3

pid_t getpid(void);
int truncate(const char *path, off_t length);
int ftruncate(int fd, off_t length);
unsigned sleep(unsigned seconds);
int usleep(useconds_t useconds);
long sysconf(int name);

int isatty(int fildes);
char *ttyname(int fildes);

#include "../src/unistd.c"

_CRT_END_C_HEADER
#endif /* !_UNISTD_H_ */

