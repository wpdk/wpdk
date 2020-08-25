#include <sys/platform.h>
#include <syslog.h>

/* HACK - null definitions */

inline static void openlog(const char *ident, int option, int facility) { }
inline static void syslog(int priority, const char *format, ...) { }
inline static void closelog() { }
