#ifndef _WPDK_STDIO_H_
#define _WPDK_STDIO_H_

#include <sys/platform.h>
#include <../ucrt/stdio.h>

_WPDK_BEGIN_C_HEADER

ssize_t getline(char **lineptr, size_t *n, FILE *stream);
FILE *wpdk_fopen(const char *filename, const char *mode);

#ifndef _WPDK_BUILD_LIB_
#define fdopen(fildes,mode)		_fdopen(fildes,mode)
#define fopen(filename,mode)	wpdk_fopen(filename,mode)
#endif

_WPDK_END_C_HEADER

#endif /* !_WPDK_STDIO_H_ */