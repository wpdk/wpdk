#include <wpdklib.h>
#include <termios.h>


int tcsetattr(int fildes, int optional_actions, const struct termios *termios_p)
{
	// HACK - implementation
	UNREFERENCED_PARAMETER(fildes);
	UNREFERENCED_PARAMETER(optional_actions);
	UNREFERENCED_PARAMETER(termios_p);
	return -1;
}


int tcgetattr(int fildes, struct termios *termios_p)
{
	// HACK - implementation
	UNREFERENCED_PARAMETER(fildes);
	UNREFERENCED_PARAMETER(termios_p);
	return -1;
}
