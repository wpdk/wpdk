#ifndef _WPDK_NET_IF_H_
#define	_WPDK_NET_IF_H_

#include <sys/platform.h>

_WPDK_BEGIN_C_HEADER

#define IFNAMSIZ 16

struct	ifreq {
	char ifr_name[IFNAMSIZ];
	short ifr_flags;
};

_WPDK_END_C_HEADER

#endif /* !_WPDK_NET_IF_H_ */