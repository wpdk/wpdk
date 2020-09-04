#ifndef _WPDK_SYS_EPOLL_H_
#define _WPDK_SYS_EPOLL_H_

#include <sys/platform.h>

_WPDK_BEGIN_C_HEADER

typedef union epoll_data {
	void		*ptr;
	int			fd;
	uint32_t	u32;
	uint64_t	u64;
} epoll_data_t;

struct epoll_event {
	uint32_t		events;
	epoll_data_t	data;
};

int epoll_create1(int flags);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

#define EPOLL_CTL_ADD	1
#define EPOLL_CTL_DEL	2

#define EPOLLIN			1
#define EPOLLERR		2

_WPDK_END_C_HEADER

#endif /* !_WPDK_SYS_EPOLL_H_ */