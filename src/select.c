#include <wpdklib.h>
#include <sys/select.h>


int wpdk_select(int nfds, fd_set *readfds, fd_set *writefds,
		fd_set *exceptfds, struct timeval *timeout)
{
	// HACK - TODO - handle error code
	return select(nfds, readfds, writefds, exceptfds, timeout);
}
