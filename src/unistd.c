#include <wpdklib.h>
#include <stdbool.h>
#include <unistd.h>


#define wpdk_unlink __real_unlink


pid_t getpid()
{
	return GetCurrentProcessId();
}


int truncate(const char *path, off_t length)
{
	// HACK - truncate
	UNREFERENCED_PARAMETER(path);
	UNREFERENCED_PARAMETER(length);
	WPDK_UNIMPLEMENTED();
	return EINVAL;
}


int ftruncate(int fd, off_t length)
{
	return (_chsize_s(fd, length) == 0) ? 0 : -1;
}


int usleep(useconds_t useconds)
{
	static LARGE_INTEGER freq;
	LARGE_INTEGER now, end;

	if (useconds >= 10000) {
		SleepEx((DWORD)(useconds / 1000), TRUE);
		return 0;
	}

	if (freq.QuadPart == 0)
		QueryPerformanceFrequency(&freq);
	
	QueryPerformanceCounter(&now);
	end.QuadPart = now.QuadPart + (useconds * freq.QuadPart) / 1000000;

	while (now.QuadPart < end.QuadPart) {
		if (SwitchToThread() == 0)
			YieldProcessor();

		QueryPerformanceCounter(&now);
	}

	return 0;
}


char *ttyname(int fildes)
{
	static char tty[] = "/dev/tty";
	// HACK - implementation
	UNREFERENCED_PARAMETER(fildes);
	WPDK_UNIMPLEMENTED();
	return tty;
}


int isatty(int fildes)
{
	// HACK - implementation
	UNREFERENCED_PARAMETER(fildes);
	WPDK_UNIMPLEMENTED();
	return false;
}


unsigned sleep(unsigned seconds)
{
	// HACK - check
	SleepEx((DWORD)(seconds * 1000), TRUE);
	// HACK - return value is unslept time
	return 0;
}


long sysconf(int name)
{
	// HACK - implement

	WPDK_UNIMPLEMENTED();

	if (name == _SC_NPROCESSORS_CONF)
		return 4;

	if (name == _SC_NPROCESSORS_ONLN)
		return 4;

	if (name == _SC_PAGESIZE)
		return 4096;

	return -1;
}


pid_t fork()
{
	// HACK - implement
	WPDK_UNIMPLEMENTED();
	return (pid_t)-1;
}


int daemon(int nochdir, int noclose)
{
	// HACK - implement
	UNREFERENCED_PARAMETER(nochdir);
	UNREFERENCED_PARAMETER(noclose);
	WPDK_UNIMPLEMENTED();
	return -1;
}


ssize_t read(int fildes, void *buf, size_t nbyte)
{
	// HACK - off_t is 32 bits
	// HACK - check nbyte is less than max uint
	return _read(fildes, buf, (unsigned int)nbyte);
}


ssize_t write(int fildes, const void *buf, size_t nbyte)
{
	// HACK - off_t is 32 bits ???
	// HACK - check nbyte is less than max uint
	return _write(fildes, buf, (unsigned int)nbyte);
}


off_t lseek(int fildes, off_t offset, int whence)
{
	// HACK - off_t is 32 bits
	// HACK - check size is less than max long
	return _lseek(fildes, (long)offset, whence);
}


int wpdk_unlink(const char *path)
{
	char buf[MAX_PATH];
	return _unlink(wpdk_get_path(path, buf, sizeof(buf)));
}


int access(const char *pathname, int mode)
{
	char buf[MAX_PATH];
	return _access(wpdk_get_path(pathname, buf, sizeof(buf)), mode);
}


int close(int fildes)
{
	if (wpdk_is_epoll(fildes))
		return wpdk_close_epoll(fildes);

	if (wpdk_is_socket(fildes))
		return wpdk_close_socket(fildes);

	return _close(fildes);
}


int fsync(int fildes)
{
	// HACK - not implemented
	UNREFERENCED_PARAMETER(fildes);
	WPDK_UNIMPLEMENTED();
	return 0;
}
