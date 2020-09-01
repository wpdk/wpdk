#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <sys/platform.h>
#include <../ucrt/stdlib.h>

_CRT_BEGIN_C_HEADER

int mkstemp(char *path);
int posix_memalign(void **memptr, size_t alignment, size_t size);

long int random(void);
void srandom(unsigned int seed);
int rand_r(unsigned int *seedp);

#ifdef _WPDK_WRAP_
// HACK - temporary hack to handle mocking
void *__wrap_calloc(size_t nelem, size_t elsize);
#define calloc(nelem,elsize) __wrap_calloc(nelem,elsize)
#endif

_CRT_END_C_HEADER

#endif /* !_STDLIB_H_ **/