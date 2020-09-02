#include <sys/platform.h>
#include <sys/socket.h>


void wpdk_seterrno_wsa (int wsaerr)
{
	switch (wsaerr) {
		case WSATRY_AGAIN:
			_set_errno(EAGAIN);
			return;

		case WSAEWOULDBLOCK:
			_set_errno(EWOULDBLOCK);
			return;
	}

	_set_errno(EINVAL);
}