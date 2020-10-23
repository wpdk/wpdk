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
#include <stdio.h>
#include <stdbool.h>


static IP_ADAPTER_ADDRESSES *
wpdk_get_adapters()
{
	IP_ADAPTER_ADDRESSES *ipa, *buf = NULL;
	DWORD len = 0;
	ULONG rc;

	while ((rc = GetAdaptersAddresses(AF_UNSPEC,
				GAA_FLAG_SKIP_ANYCAST|GAA_FLAG_SKIP_MULTICAST|GAA_FLAG_SKIP_DNS_SERVER,
				NULL, buf, &len)) != ERROR_SUCCESS) {

		if (rc != ERROR_BUFFER_OVERFLOW) {
			free(buf);
			wpdk_windows_error(rc);
			return NULL;
		}

		ipa = buf;
		if ((buf = realloc(buf, len)) == NULL) {
			free(ipa);
			wpdk_posix_error(ENOMEM);
			return NULL;
		}
	}

	return buf;
}


static bool
wpdk_get_if_info(IP_ADAPTER_ADDRESSES *ipa,
		ADDRESS_FAMILY family, char *name, short *pflags)
{
	int index;

	if (family != AF_INET && family != AF_INET6)
		return false;

	*pflags = 0;
	if (ipa->OperStatus == IfOperStatusUp) *pflags |= IFF_UP;
	if (ipa->IfType == IF_TYPE_SOFTWARE_LOOPBACK) *pflags |= IFF_LOOPBACK;

 	index = (family == AF_INET6) ? ipa->Ipv6IfIndex : ipa->IfIndex;

	sprintf_s(name, IF_NAMESIZE, "%s%d",
		(*pflags & IFF_LOOPBACK) ? "lo" : "en", index);

	return true;
}


int
wpdk_getifaddrs(struct ifaddrs **ifap)
{
	struct ifaddrs *ifa, *iflist = NULL;
	IP_ADAPTER_UNICAST_ADDRESS *addr;
	IP_ADAPTER_ADDRESSES *ipa, *buf;
	char name[IF_NAMESIZE];
	short flags;
	size_t len;

	if (!ifap) return EINVAL;

	wpdk_set_invalid_handler();

	if ((buf = wpdk_get_adapters()) == NULL)
		return -1;

	for (ipa = buf; ipa; ipa = ipa->Next) {
		for (addr = ipa->FirstUnicastAddress; addr; addr = addr->Next) {
			if (!wpdk_get_if_info(ipa, addr->Address.lpSockaddr->sa_family,
					name, &flags))
				continue;

			len = sizeof(struct ifaddrs) + addr->Address.iSockaddrLength + IF_NAMESIZE;
			
			if ((ifa = calloc(1, len)) == NULL) {
				wpdk_freeifaddrs(iflist);
				free(buf);
				return wpdk_posix_error(ENOMEM);
			}

			ifa->ifa_addr = (struct sockaddr *)(ifa + 1);
			ifa->ifa_name = (char *)ifa->ifa_addr + addr->Address.iSockaddrLength;
			ifa->ifa_flags = flags;

			memcpy(ifa->ifa_addr, addr->Address.lpSockaddr, addr->Address.iSockaddrLength);
			strcpy(ifa->ifa_name, name);

			ifa->ifa_next = iflist;
			iflist = ifa;
		}
	}

	free(buf);

	*ifap = iflist;
	return 0;
}


int
wpdk_getifflags(struct ifreq *ifr)
{
	IP_ADAPTER_UNICAST_ADDRESS *addr;
	IP_ADAPTER_ADDRESSES *ipa, *buf;
	char name[IF_NAMESIZE];
	short flags;

	wpdk_set_invalid_handler();

	if ((buf = wpdk_get_adapters()) == NULL)
		return -1;

	for (ipa = buf; ipa; ipa = ipa->Next) {
		for (addr = ipa->FirstUnicastAddress; addr; addr = addr->Next) {
			if (!wpdk_get_if_info(ipa, addr->Address.lpSockaddr->sa_family,
					name, &flags))
				continue;

			if (strcmp(ifr->ifr_name, name) == 0) {
				ifr->ifr_flags = flags;
				free(buf);
				return 0;
			}
		}
	}

	free(buf);
	return wpdk_posix_error(EINVAL);
}


void
wpdk_freeifaddrs(struct ifaddrs *ifa)
{
	struct ifaddrs *next;

	for (; ifa; ifa = next) {
		next = ifa->ifa_next;
		free(ifa);
	}
}
