/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2020, MayaData Inc. All rights reserved.
 *  Copyright (c) 2020, DataCore Software Corporation. All rights reserved.
 * 
 *  POSIX details are based on the Open Group Base Specification Issue 7,
 *  2018 edition at https://pubs.opengroup.org/onlinepubs/9699919799/
 * 
 *  Details about Linux extensions are based on the Linux man-pages project
 *  at https://www.kernel.org/doc/man-pages/
 */

#include <wpdk/internal.h>
#include <sys/socket.h>
#include <arpa/inet.h>


uint32_t
wpdk_htonl(uint32_t hostlong)
{
	return htonl(hostlong);
}


uint16_t
wpdk_htons(uint16_t hostshort)
{
	return htons(hostshort);
}


uint32_t
wpdk_ntohl(uint32_t netlong)
{
	return ntohl(netlong);
}


uint16_t
wpdk_ntohs(uint16_t netshort)
{
	return ntohs(netshort);
}


in_addr_t
wpdk_inet_addr(const char *cp)
{
	return inet_addr(cp);
}


char *
wpdk_inet_ntoa(struct in_addr in)
{
	return inet_ntoa(in);
}


const char *
wpdk_inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
	return inet_ntop(af, (void *)src, dst, size);
}


int wpdk_inet_pton(int af, const char *src, void *dst)
{
	return inet_pton(af, src, dst);
}
