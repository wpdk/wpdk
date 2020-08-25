#ifndef _SYS_FILE_H_
#define	_SYS_FILE_H_

#include <sys/platform.h>

_CRT_BEGIN_C_HEADER

int flock(int fd, int operation);

#include "../src/file.c"

_CRT_END_C_HEADER

#endif /* !_SYS_FILE_H_ */
