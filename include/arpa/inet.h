#ifndef _ARPA_INET_H_
#define _ARPA_INET_H_

#include <sys/platform.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>

_CRT_BEGIN_C_HEADER

uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);

in_addr_t inet_addr(const char *cp);
char *inet_ntoa(struct in_addr in);

_CRT_END_C_HEADER

#endif /* !_ARPA_INET_H_ */