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


struct gai_error {
	int ecode;
	const char *message;
};

static struct gai_error gai_errors[] = {
	{ ERROR_SUCCESS, 0 },
	{ EAI_AGAIN, 0 },
	{ EAI_BADFLAGS, 0 },
	{ EAI_FAIL, 0 },
	{ EAI_FAMILY, 0 },
	{ EAI_MEMORY, 0 },
	{ EAI_NONAME, 0 },
	{ EAI_SERVICE, 0 },
	{ EAI_SOCKTYPE, 0 },
	{ EAI_NOSECURENAME, 0 },
	{ EAI_IPSECPOLICY, 0 },
	{ -1, "Unknown GAI error" }
};

static SRWLOCK gai_lock = SRWLOCK_INIT;


int wpdk_getaddrinfo(const char *node, const char *service,
					 const struct addrinfo *hints, struct addrinfo **res)
{
	if (!wpdk_socket_startup()) {
		wpdk_last_wsa_error();
		return (errno == ENOMEM) ? EAI_MEMORY : EAI_FAIL;
	}

	return getaddrinfo(node, service, hints, res);
}


void
wpdk_freeaddrinfo(struct addrinfo *ai)
{
	freeaddrinfo(ai);
}


const char *wpdk_gai_strerror(int ecode)
{
	struct gai_error *gai;

	/*
	 *  gai_strerror is not thread safe, so return a copy
	 *  of the message and cache it for future use.
	 */
	for (gai = gai_errors; gai->ecode != -1; gai++)
		if (gai->ecode == ecode) break;

	if (gai->message)
		return gai->message;

	AcquireSRWLockExclusive(&gai_lock);

	if (!gai->message)
		gai->message = _strdup(gai_strerror(ecode));

	ReleaseSRWLockExclusive(&gai_lock);

	if (!gai->message)
		return "Unknown GAI error";

	return gai->message;
}
