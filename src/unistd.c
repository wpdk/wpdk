#include <sys/platform.h>
#include <stdbool.h>
#include <unistd.h>


#define wpdk_unlink __real_unlink

int wpdk_is_socket(int fd);
int wpdk_close_socket(int socket);
int wpdk_is_epoll(int fd);
int wpdk_close_epoll(int fd);
const char *wpdk_get_path(const char *);


pid_t getpid()
{
	return GetCurrentProcessId();
}


int truncate(const char *path, off_t length)
{
	// HACK - truncate
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
	return tty;
}


int isatty(int fildes)
{
	// HACK - implementation
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
	return -1;
}


int daemon(int nochdir, int noclose)
{
	// HACK - implement
	return -1;
}


ssize_t read(int fildes, void *buf, size_t nbyte)
{
	// HACK - off_t is 32 bits
	return _read(fildes, buf, nbyte);
}


ssize_t write(int fildes, const void *buf, size_t nbyte)
{
	// HACK - off_t is 32 bits ???
	return _write(fildes, buf, nbyte);
}


off_t lseek(int fildes, off_t offset, int whence)
{
	// HACK - off_t is 32 bits
	return _lseek(fildes, offset, whence);
}


int wpdk_unlink(const char *path)
{
	return _unlink(wpdk_get_path(path));
}


int access(const char *pathname, int mode)
{
	return _access(wpdk_get_path(pathname), mode);
}


int close(int fildes)
{
	if (wpdk_is_epoll(fildes))
		return wpdk_close_epoll(fildes);

	if (wpdk_is_socket(fildes))
		return wpdk_close_socket(fildes);

	return _close(fildes);
}
