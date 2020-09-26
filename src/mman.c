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
#include <sys/mman.h>
#include <io.h>


int wpdk_shm_open(const char *name, int oflag, mode_t mode)
{
	char buf[MAX_PATH];
	return _open(wpdk_get_path(name, buf, sizeof(buf)), oflag, mode);
}


int wpdk_shm_unlink(const char *name)
{
	char buf[MAX_PATH];
	// HACK - shm_unlink - check handling with open fds	
	return _unlink(wpdk_get_path(name, buf, sizeof(buf)));
}


void *wpdk_mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
{
	LARGE_INTEGER start, end, size;
	DWORD granularity;
	SYSTEM_INFO info;
	HANDLE h, hMap;
	void *pMap;

	UNREFERENCED_PARAMETER(flags);
	UNREFERENCED_PARAMETER(prot);
	
	// HACK - fixed address not implemented yet
	if (addr) {
		WPDK_UNIMPLEMENTED();
		_set_errno(EINVAL);
		return NULL;
	}

	h = (HANDLE)_get_osfhandle(fildes);

	if (h == INVALID_HANDLE_VALUE) {
		_set_errno(EBADF);
		return NULL;
	}

	GetSystemInfo(&info);
	granularity = info.dwAllocationGranularity;

	start.QuadPart = (off / granularity) * granularity;
	end.QuadPart = off + len;
	size.QuadPart = end.QuadPart - start.QuadPart;

	if (size.HighPart != 0) {
		_set_errno(EINVAL);
		return NULL;
	}

	// HACK - mmap ignores prot
	hMap = CreateFileMapping(h, NULL, PAGE_READWRITE, end.HighPart, end.LowPart, NULL);

	if (h == INVALID_HANDLE_VALUE) {
		_set_errno(EBADF);
		return NULL;
	}

	pMap = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, start.HighPart, start.LowPart, size.LowPart);

	CloseHandle(hMap);

	if (pMap == NULL) {
		_set_errno(ENOMEM);
		return NULL;
	}

	return (char *)pMap + (off - start.QuadPart);
}


int wpdk_munmap(void *addr, size_t len)
{
	SYSTEM_INFO info;
	DWORD granularity;

	UNREFERENCED_PARAMETER(len);

	GetSystemInfo(&info);
	granularity = info.dwAllocationGranularity;

	// HACK - munmap error handling
	return (UnmapViewOfFile((void *)(((ULONG_PTR)addr / granularity) * granularity)) != 0) ? 0 : -1;
}
