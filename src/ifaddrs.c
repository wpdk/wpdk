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
#include <sys/un.h>
#include <ifaddrs.h>
#include <iphlpapi.h>
#include <stdlib.h>
#include <net/if.h>


int wpdk_getifaddrs(struct ifaddrs **ifap)
{
	IP_ADAPTER_ADDRESSES *ipa, *buf = NULL;
	struct ifaddrs *ifa, *iflist = NULL;
	IP_ADAPTER_UNICAST_ADDRESS *addr;
	ADDRESS_FAMILY family;
	size_t len = 0;
	DWORD nbytes;
	int index;
	ULONG rc;

	if (!ifap) return EINVAL;

	wpdk_set_invalid_handler();

	while ((rc = GetAdaptersAddresses(AF_UNSPEC,
				GAA_FLAG_SKIP_ANYCAST|GAA_FLAG_SKIP_MULTICAST|GAA_FLAG_SKIP_DNS_SERVER/*|
				GAA_FLAG_SKIP_FRIENDLY_NAME*/, NULL, buf, &nbytes)) != ERROR_SUCCESS) {

		if (rc != ERROR_BUFFER_OVERFLOW) {
			free(buf);
			return wpdk_windows_error(rc);
		}

		ipa = buf;
		len = nbytes;
	
		if ((buf = realloc(buf, len)) == NULL) {
			free(ipa);
			return wpdk_posix_error(ENOMEM);
		}
	}

	for (ipa = buf; ipa; ipa = ipa->Next) {
		for (addr = ipa->FirstUnicastAddress; addr; addr = addr->Next) {
			family = addr->Address.lpSockaddr->sa_family;
			index = (family == AF_INET6) ? ipa->Ipv6IfIndex : ipa->IfIndex;

			if (family != AF_INET && family != AF_INET6)
				continue;

			len = sizeof(struct ifaddrs) + addr->Address.iSockaddrLength + IF_NAMESIZE;
			
			if ((ifa = calloc(1, len)) == NULL) {
				wpdk_freeifaddrs(iflist);
				free(buf);
				return wpdk_posix_error(ENOMEM);
			}

			ifa->ifa_addr = (struct sockaddr *)(ifa + 1);
			ifa->ifa_name = (char *)ifa->ifa_addr + addr->Address.iSockaddrLength;

			if (ipa->OperStatus == IfOperStatusUp) ifa->ifa_flags |= IFF_UP;
			if (ipa->IfType == IF_TYPE_SOFTWARE_LOOPBACK) ifa->ifa_flags |= IFF_LOOPBACK;

			memcpy(ifa->ifa_addr, addr->Address.lpSockaddr, addr->Address.iSockaddrLength);

			sprintf_s(ifa->ifa_name, IF_NAMESIZE, "%s%d",
				(ifa->ifa_flags & IFF_LOOPBACK) ? "lo" : "en", index);

			ifa->ifa_next = iflist;
			iflist = ifa;
		}
	}

	free(buf);

	*ifap = iflist;
	return 0;
}


void wpdk_freeifaddrs(struct ifaddrs *ifa)
{
	struct ifaddrs *next;

	for (; ifa; ifa = next) {
		next = ifa->ifa_next;
		free(ifa);
	}
}
