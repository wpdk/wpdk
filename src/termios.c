#include <wpdklib.h>
#include <termios.h>


int tcsetattr(int fildes, int optional_actions, const struct termios *termios_p)
{
	// HACK - implementation
	return -1;
}


int tcgetattr(int fildes, struct termios *termios_p)
{
	// HACK - implementation
	return -1;
}