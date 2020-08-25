#ifndef _SYS_SYSLOG_H_
#define _SYS_SYSLOG_H_

#include <sys/platform.h>

_CRT_BEGIN_C_HEADER

#define	LOG_EMERG	0
#define	LOG_ALERT	1
#define	LOG_CRIT	2
#define	LOG_ERR		3
#define	LOG_WARNING	4
#define	LOG_NOTICE	5
#define	LOG_INFO	6
#define	LOG_DEBUG	7

#define	LOG_LOCAL7	(1<<3)
#define	LOG_PID		1

void openlog(const char *ident, int option, int facility);
void syslog(int priority, const char *format, ...);
void closelog(void);

#include "../src/syslog.c"

_CRT_END_C_HEADER

#endif /* !_SYS_SYSLOG_H_ */