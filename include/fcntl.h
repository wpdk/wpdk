#ifndef _SYS_FCNTL_H_
#define	_SYS_FCNTL_H_

#include <sys/platform.h>
#include <../ucrt/fcntl.h>
#include <io.h>

_CRT_BEGIN_C_HEADER

#define F_GETFD     1
#define F_SETFD     2
#define F_GETFL     3
#define F_SETFL     4
#define F_GETLK     5
#define F_SETLK     6

#define LOCK_SH     1
#define LOCK_EX     2
#define LOCK_NB     4
#define LOCK_UN     8

#define O_NONBLOCK  0x0004

int fcntl(int fildes, int cmd, ...);

#include "../src/fcntl.c"

_CRT_END_C_HEADER

#endif /* !_SYS_FCNTL_H_ */