#include <wpdklib.h>
#include <sys/ioctl.h>


int ioctl(int fd, unsigned long request, ...)
{
	// HACK - not implemented
	return EINVAL;
}