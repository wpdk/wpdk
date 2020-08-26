#ifndef _STDIO_H_
#define _STDIO_H_

#include <sys/platform.h>
#include <../ucrt/stdio.h>

_CRT_BEGIN_C_HEADER

ssize_t getline(char **lineptr, size_t *n, FILE *stream);

#define fdopen(fildes,mode)     _fdopen(fildes,mode)

#include "../src/stdio.c"

_CRT_END_C_HEADER

#endif /* !_STDIO_H_ */