#ifndef _SYS_MMAN_H_
#define	_SYS_MMAN_H_

#include <sys/platform.h>

_CRT_BEGIN_C_HEADER

#define PROT_NONE   0
#define PROT_READ   1
#define PROT_EXEC   2
#define PROT_WRITE  4

#define MAP_SHARED      0x01
#define MAP_PRIVATE     0x02
#define MAP_FIXED       0x04
#define MAP_ANONYMOUS   0x08

#define MAP_FAILED ((void *) -1)

void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
int munmap(void *addr, size_t len);
int shm_open(const char *name, int oflag, mode_t mode);
int shm_unlink(const char *name);

#include "../src/mman.c"

_CRT_END_C_HEADER

#endif /* !_SYS_MMAN_H_ */