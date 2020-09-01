#include <sys/platform.h>
#include <syslog.h>

/* HACK - null definitions */

void openlog(const char *ident, int option, int facility) { }
void syslog(int priority, const char *format, ...) { }
void closelog() { }
