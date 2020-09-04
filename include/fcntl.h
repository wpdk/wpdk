#ifndef _WPDK_FCNTL_H_
#define	_WPDK_FCNTL_H_

#include <sys/platform.h>
#include <../ucrt/fcntl.h>
#include <stdio.h>
#include <io.h>

_WPDK_BEGIN_C_HEADER

#define O_RDONLY	_O_RDONLY
#define O_WRONLY	_O_WRONLY
#define O_RDWR		_O_RDWR
#define O_APPEND	_O_APPEND
#define O_CREAT		_O_CREAT
#define O_TRUNC		_O_TRUNC
#define O_EXCL		_O_EXCL

#define F_GETFD		1
#define F_SETFD		2
#define F_GETFL		3
#define F_SETFL		4
#define F_GETLK		5
#define F_SETLK		6

#define LOCK_SH		1
#define LOCK_EX		2
#define LOCK_NB		4
#define LOCK_UN		8

#define O_NONBLOCK	0x0004

int fcntl(int fildes, int cmd, ...);

#ifndef _WPDK_BUILD_LIB_
#define open(path,oflag,...)	_open(path,oflag,__VA_ARGS__)
#endif

_WPDK_END_C_HEADER

#endif /* !_WPDK_FCNTL_H_ */