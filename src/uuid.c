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
#include <uuid/uuid.h>
#include <guiddef.h>
#include <cguid.h>
#include <rpc.h>

#undef uuid_t


void uuid_clear(uuid_t uu)
{
	memcpy(uu, &GUID_NULL, sizeof(uuid_t));
}


void uuid_copy(uuid_t dst, const uuid_t src)
{
	memcpy(dst, src, sizeof(uuid_t));
}


int uuid_is_null(const uuid_t uu)
{
	GUID id;
	memcpy(&id, uu, sizeof(uuid_t));
	return IsEqualGUID(&id, &GUID_NULL);
}


void uuid_generate(uuid_t out)
{
	UUID id;

	if (UuidCreate(&id) != RPC_S_OK) {
		// HACK - unclear what to do if this fails as well
		UuidCreateSequential(&id);
	}

	memcpy(out, &id, sizeof(uuid_t));
}


int uuid_parse(const char *in, uuid_t uu)
{
	UUID id;

	if (UuidFromString((unsigned char *)in, &id) == RPC_S_OK) {
		memcpy(uu, &id, sizeof(uuid_t));
		return 0;
	}

	return -1;
}


int uuid_compare(const uuid_t uu1, const uuid_t uu2)
{
	RPC_STATUS status;
	UUID id1, id2;

	memcpy(&id1, uu1, sizeof(uuid_t));
	memcpy(&id2, uu2, sizeof(uuid_t));

	// HACK - unclear what to do with status;
	return UuidCompare(&id1, &id2, &status);
}


#define uuid_string_len 37

void uuid_unparse(const uuid_t uu, char *out)
{
	static const char invalid[] = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx";
	RPC_STATUS rc;
	RPC_CSTR str;
	UUID id;

	memcpy(&id, uu, sizeof(uuid_t));
	rc = UuidToString(&id, &str);

	if (rc != RPC_S_OK) {
		strcpy_s(out, sizeof(invalid), invalid);
		return;
	}

	strcpy_s(out, uuid_string_len, (const char *)str);
	RpcStringFree(&str);
}


void uuid_unparse_upper(const uuid_t uu, char *out)
{
	uuid_unparse(uu, out);
	_strupr_s(out, uuid_string_len);
}


void uuid_unparse_lower(const uuid_t uu, char *out)
{
	uuid_unparse(uu, out);
	_strlwr_s(out, uuid_string_len);
}
