#ifndef	_IFADDRS_H_
#define	_IFADDRS_H_

#include <sys/platform.h>

_CRT_BEGIN_C_HEADER

struct ifaddrs {
    struct ifaddrs *ifa_next;
    char *ifa_name;
    unsigned int ifa_flags;
	struct sockaddr	*ifa_addr;    
};

int getifaddrs(struct ifaddrs **ifap);
void freeifaddrs(struct ifaddrs *ifa);

#include "../src/ifaddrs.c"

_CRT_END_C_HEADER

#endif /* !_IFADDRS_H_ */