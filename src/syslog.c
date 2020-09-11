#include <wpdklib.h>
#include <syslog.h>

/* HACK - null definitions */

void openlog(const char *ident, int option, int facility)
{
	UNREFERENCED_PARAMETER(ident);
	UNREFERENCED_PARAMETER(option);
	UNREFERENCED_PARAMETER(facility);
}


void syslog(int priority, const char *format, ...)
{
	UNREFERENCED_PARAMETER(priority);
	UNREFERENCED_PARAMETER(format);
}


void closelog()
{

}
