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
 * 
 *  Details about the variants and versions of UUIDs are based on the article
 *  https://en.wikipedia.org/wiki/Universally_unique_identifier
 */

#include <wpdk/internal.h>
#include <uuid/uuid.h>
#include <stdio.h>
#include <guiddef.h>
#include <cguid.h>
#include <rpc.h>


/*
 *  Libuuid at https://github.com/karelzak/util-linux/tree/master/libuuid
 *  will build for Windows, but some of the logic in uuid_generate is stubbed
 *  out, so provide the functionality using Microsoft's implementation.
 */

#undef uuid_t


static inline void
put_uint32(uint32_t v, unsigned char *cp, int little_endian)
{
	if (little_endian) {
		cp[3] = (unsigned char)(v >> 24);
		cp[2] = (unsigned char)(v >> 16);
		cp[1] = (unsigned char)(v >> 8);
		cp[0] = (unsigned char)v;
	} else {
		cp[0] = (unsigned char)(v >> 24);
		cp[1] = (unsigned char)(v >> 16);
		cp[2] = (unsigned char)(v >> 8);
		cp[3] = (unsigned char)v;
	}
}


static inline void
put_uint16(uint16_t v, unsigned char *cp, int little_endian)
{
	if (little_endian) {
		cp[1] = (unsigned char)(v >> 8);
		cp[0] = (unsigned char)v;
	} else {
		cp[0] = (unsigned char)(v >> 8);
		cp[1] = (unsigned char)v;
	}
}


static inline uint32_t
get_uint32(const unsigned char *cp, int little_endian)
{
	return (little_endian) ?
		(((uint32_t)cp[3] << 24) | ((uint32_t)cp[2] << 16) | ((uint32_t)cp[1] << 8) | cp[0]) :
		(((uint32_t)cp[0] << 24) | ((uint32_t)cp[1] << 16) | ((uint32_t)cp[2] << 8) | cp[3]);
}


static inline uint16_t
get_uint16(const unsigned char *cp, int little_endian)
{
	return (little_endian) ?
		(((uint16_t)cp[1] << 8) | cp[0]) :
		(((uint16_t)cp[0] << 8) | cp[1]);
}


static inline int
is_little_endian (unsigned char v)
{
	/* Variant 2 is little endian */
	return ((v & 0xe0) == 0xc0);
}


static void
uuid_to_bytes(const UUID *src, uuid_t dest)
{
	int little_endian = is_little_endian(src->Data4[0]);

	put_uint32(src->Data1, &dest[0], little_endian);
	put_uint16(src->Data2, &dest[4], little_endian);
	put_uint16(src->Data3, &dest[6], little_endian);
	memcpy(&dest[8], src->Data4, sizeof(src->Data4));
}


static void
bytes_to_uuid(const uuid_t src, UUID *dest)
{
	int little_endian = is_little_endian(src[8]);

	dest->Data1 = get_uint32(&src[0], little_endian);
	dest->Data2 = get_uint16(&src[4], little_endian);
	dest->Data3 = get_uint16(&src[6], little_endian);
	memcpy(&dest->Data4, &src[8], sizeof(dest->Data4));
}


void
uuid_clear(uuid_t uu)
{
	uuid_to_bytes(&GUID_NULL, uu);
}


void
uuid_copy(uuid_t dst, const uuid_t src)
{
	memcpy(dst, src, sizeof(uuid_t));
}


int
uuid_is_null(const uuid_t uu)
{
	GUID id;
	bytes_to_uuid(uu, &id);
	return IsEqualGUID(&id, &GUID_NULL);
}


void
uuid_generate(uuid_t out)
{
	RPC_STATUS rc;
	UUID id;

	rc = UuidCreate(&id);
	
	if (rc != RPC_S_OK && rc != RPC_S_UUID_LOCAL_ONLY) {
		/*
		 *  If RCP_S_UUID_LOCAL_ONLY is returned, the uuid is only guaranteed
		 *  to be unique on this computer, which is probably ok. Otherwise,
		 *  treat it as a fatal error.
		 */
		WPDK_FATAL_ERROR("Unable to create uuid");
	}

	uuid_to_bytes(&id, out);
}


int
uuid_parse(const char *in, uuid_t uu)
{
	UUID id;

	if (UuidFromString((unsigned char *)in, &id) == RPC_S_OK) {
		uuid_to_bytes(&id, uu);
		return 0;
	}

	return -1;
}


int
uuid_compare(const uuid_t uu1, const uuid_t uu2)
{
	UUID id1, id2;

	bytes_to_uuid(uu1, &id1);
	bytes_to_uuid(uu2, &id2);

	/*
	 * UuidCompare says it can return a status code, which there is
	 * no way to handle, so perform the compare directly.
	 */
	if (id1.Data1 != id2.Data1)	return (id1.Data1 < id2.Data1) ? -1 : 1;
	if (id1.Data2 != id2.Data2)	return (id1.Data2 < id2.Data2) ? -1 : 1;
	if (id1.Data3 != id2.Data3)	return (id1.Data3 < id2.Data3) ? -1 : 1;

	return memcmp(&id1.Data4, &id2.Data4, sizeof(id1.Data4));
}


void
uuid_unparse(const uuid_t uu, char *out)
{
	uuid_unparse_upper(uu, out);
}


#define XXXX "%02hhX%02hhX"

void
uuid_unparse_upper(const uuid_t uu, char *out)
{
	UUID u;
	bytes_to_uuid(uu, &u);

	sprintf(out, "%08lX-%04hX-%04hX-" XXXX "-" XXXX XXXX XXXX,
		u.Data1, u.Data2, u.Data3, u.Data4[0], u.Data4[1], u.Data4[2],
		u.Data4[3], u.Data4[4], u.Data4[5], u.Data4[6], u.Data4[7]);
}


#define xxxx "%02hhx%02hhx"

void
uuid_unparse_lower(const uuid_t uu, char *out)
{
	UUID u;
	bytes_to_uuid(uu, &u);

	sprintf(out, "%08lx-%04hx-%04hx-" xxxx "-" xxxx xxxx xxxx,
		u.Data1, u.Data2, u.Data3, u.Data4[0], u.Data4[1], u.Data4[2],
		u.Data4[3], u.Data4[4], u.Data4[5], u.Data4[6], u.Data4[7]);
}
