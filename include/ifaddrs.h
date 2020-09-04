#ifndef	_WPDK_IFADDRS_H_
#define	_WPDK_IFADDRS_H_

#include <sys/platform.h>

_WPDK_BEGIN_C_HEADER

struct ifaddrs {
	struct ifaddrs *ifa_next;
	char *ifa_name;
	unsigned int ifa_flags;
	struct sockaddr	*ifa_addr;    
};

int getifaddrs(struct ifaddrs **ifap);
void freeifaddrs(struct ifaddrs *ifa);

_WPDK_END_C_HEADER

#endif /* !_WPDK_IFADDRS_H_ */