#ifndef _WPDK_STDLIB_H_
#define _WPDK_STDLIB_H_

#include <sys/platform.h>
#include <../ucrt/stdlib.h>

_WPDK_BEGIN_C_HEADER

int mkstemp(char *path);
int posix_memalign(void **memptr, size_t alignment, size_t size);

long int random(void);
void srandom(unsigned int seed);
int rand_r(unsigned int *seedp);

void *__wrap_calloc(size_t nelem, size_t elsize);

#ifndef _WPDK_BUILD_LIB_
#define calloc(nelem,elsize) __wrap_calloc(nelem, elsize);
#endif

_WPDK_END_C_HEADER

#endif /* !_WPDK_STDLIB_H_ **/