#include <wpdklib.h>
#include <sys/file.h>


int flock(int fd, int operation)
{
	UNREFERENCED_PARAMETER(fd);
	UNREFERENCED_PARAMETER(operation);
	// HACK - not implemented
	WPDK_UNIMPLEMENTED();
	return 0;
}
