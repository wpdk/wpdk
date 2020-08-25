#ifndef _STRINGS_H_
#define	_STRINGS_H_

#include <sys/platform.h>
#include <string.h>

_CRT_BEGIN_C_HEADER

int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);

int strerror_r(int errnum, char *buf, size_t buflen);
char *strerror_r_gnu(int errnum, char *buf, size_t buflen);

char *strcasestr(const char *haystack, const char *needle);

#include "../src/strings.c"

#if defined(__USE_GNU)
#define strerror_r strerror_r_gnu
#endif

_CRT_END_C_HEADER
#endif /* !_STRINGS_H_ */