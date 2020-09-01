#ifndef _SYS_RESOURCE_H_
#define	_SYS_RESOURCE_H_

#include <sys/platform.h>
#include <sys/time.h>

_CRT_BEGIN_C_HEADER

struct rusage {
    struct timeval ru_utime;
    struct timeval ru_stime;
    long    ru_nvcsw;
    long    ru_nivcsw;
};

#define RUSAGE_THREAD   1

struct rlimit {
    rlim_t      rlim_cur; 
    rlim_t      rlim_max;
};

#define RLIMIT_CORE     1

int getrlimit(int resource, struct rlimit *rlp);
int setrlimit(int resource, const struct rlimit *rlp);
int getrusage(int who, struct rusage *usage);

_CRT_END_C_HEADER

#endif /* !_SYS_RESOURCE_H_ */