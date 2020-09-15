#include <wpdklib.h>
#include <syslog.h>

/* HACK - null definitions */

void openlog(const char *ident, int option, int facility)
{
	UNREFERENCED_PARAMETER(ident);
	UNREFERENCED_PARAMETER(option);
	UNREFERENCED_PARAMETER(facility);
	WPDK_UNIMPLEMENTED();
}


void syslog(int priority, const char *format, ...)
{
	UNREFERENCED_PARAMETER(priority);
	UNREFERENCED_PARAMETER(format);
	WPDK_UNIMPLEMENTED();
}


void closelog()
{
	WPDK_UNIMPLEMENTED();
}
