#include <wpdklib.h>
#include <sys/mman.h>
#include <io.h>


int shm_open(const char *name, int oflag, mode_t mode)
{
	return _open(wpdk_get_path(name), oflag, mode);
}


int shm_unlink(const char *name)
{
	// HACK - shm_unlink - check handling with open fds	
	return _unlink(name);
}


void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
{
	LARGE_INTEGER start, end, size;
	DWORD granularity;
	SYSTEM_INFO info;
	HANDLE h, hMap;
	void *pMap;

	// HACK - fixed address not implemented yet
	if (addr) {
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


int munmap(void *addr, size_t len)
{
	SYSTEM_INFO info;
	DWORD granularity;

	GetSystemInfo(&info);
	granularity = info.dwAllocationGranularity;

	// HACK - munmap error handling
	return (UnmapViewOfFile((void *)(((ULONG_PTR)addr / granularity) * granularity)) != 0) ? 0 : -1;
}