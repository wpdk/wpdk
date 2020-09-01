#ifndef _UNISTD_H_
#define	_UNISTD_H_

#include <sys/platform.h>
#include <sys/time.h>
#include <stdio.h>
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

int access(const char *path, int amode);
int close(int fildes);
int daemon(int nochdir, int noclose);
pid_t fork(void);
int ftruncate(int fd, off_t length);
pid_t getpid(void);
int isatty(int fildes);
off_t lseek(int fildes, off_t offset, int whence);
ssize_t read(int fildes, void *buf, size_t nbyte);
unsigned sleep(unsigned seconds);
long sysconf(int name);
int truncate(const char *path, off_t length);
char *ttyname(int fildes);
int usleep(useconds_t useconds);
ssize_t write(int fildes, const void *buf, size_t nbyte);

int __wrap_unlink(const char *path);

#ifndef _WPDK_BUILD_LIB_
#define unlink _unlink
#define _unlink(path) __wrap_unlink(path)
#endif

_CRT_END_C_HEADER

#endif /* !_UNISTD_H_ */