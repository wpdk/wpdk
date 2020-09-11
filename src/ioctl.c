#include <wpdklib.h>
#include <sys/ioctl.h>


int ioctl(int fd, unsigned long request, ...)
{
	UNREFERENCED_PARAMETER(fd);
	UNREFERENCED_PARAMETER(request);
	// HACK - not implemented
	return EINVAL;
}
