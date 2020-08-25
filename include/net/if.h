#ifndef _NET_IF_H_
#define	_NET_IF_H_

#include <sys/platform.h>

_CRT_BEGIN_C_HEADER

#define IFNAMSIZ 16

struct	ifreq {
    char ifr_name[IFNAMSIZ];
    short ifr_flags;
};

#define IFF_UP          1
#define IFF_LOOPBACK    2

_CRT_END_C_HEADER

#endif /* !_NET_IF_H_ */