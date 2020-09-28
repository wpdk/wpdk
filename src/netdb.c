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
#include <netdb.h>


int wpdk_getaddrinfo(const char *node, const char *service,
					 const struct addrinfo *hints, struct addrinfo **res)
{
	// HACK - check return code and map it
	if (!wpdk_socket_startup())
		return EAI_MEMORY;

	return getaddrinfo(node, service, hints, res);
}


void
wpdk_freeaddrinfo(struct addrinfo *ai)
{
	freeaddrinfo(ai);
}


const char *wpdk_gai_strerror(int ecode)
{
	// HACK - gai_strerror is not thread safe
	return gai_strerror(ecode);
}
