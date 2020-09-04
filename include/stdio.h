#ifndef _WPDK_STDIO_H_
#define _WPDK_STDIO_H_

#include <sys/platform.h>
#include <../ucrt/stdio.h>

_WPDK_BEGIN_C_HEADER

ssize_t getline(char **lineptr, size_t *n, FILE *stream);

#define fdopen(fildes,mode)		_fdopen(fildes,mode)

_WPDK_END_C_HEADER

#endif /* !_WPDK_STDIO_H_ */