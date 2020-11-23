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

#ifndef _WPDK_ARPA_INET_H_
#define _WPDK_ARPA_INET_H_

#include <wpdk/header.h>
#include <sys/types.h>
#include <netinet/in.h>

_WPDK_BEGIN_C_HEADER

uint32_t wpdk_htonl(uint32_t hostlong);
uint16_t wpdk_htons(uint16_t hostshort);
uint32_t wpdk_ntohl(uint32_t netlong);
uint16_t wpdk_ntohs(uint16_t netshort);
in_addr_t wpdk_inet_addr(const char *cp);
int wpdk_inet_aton(const char *cp, struct in_addr *inp);
char *wpdk_inet_ntoa(struct in_addr in);
const char *wpdk_inet_ntop(int af, const void *src, char *dst, socklen_t size);
int wpdk_inet_pton(int af, const char *src, void *dst);

#ifndef _WPDK_BUILD_LIB_
#define htonl(hostlong) wpdk_htonl(hostlong)
#define htons(hostshort) wpdk_htons(hostshort)
#define ntohl(netlong) wpdk_ntohl(netlong)
#define ntohs(netshort) wpdk_ntohs(netshort)
#define inet_addr(cp) wpdk_inet_addr(cp)
#define inet_aton(cp,inp) wpdk_inet_aton(cp,inp)
#define inet_ntoa(in) wpdk_inet_ntoa(in)
#define inet_ntop(af,src,dst,size) wpdk_inet_ntop(af,src,dst,size)
#define inet_pton(af,src,dst) wpdk_inet_pton(af,src,dst)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_ARPA_INET_H_ */
