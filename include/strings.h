#ifndef _WPDK_STRINGS_H_
#define	_WPDK_STRINGS_H_

#include <sys/platform.h>
#include <string.h>

_WPDK_BEGIN_C_HEADER

int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);

int strerror_r(int errnum, char *buf, size_t buflen);
char *strerror_r_gnu(int errnum, char *buf, size_t buflen);

char *strcasestr(const char *haystack, const char *needle);

#ifndef _WPDK_BUILD_LIB_
#if defined(__USE_GNU)
#define strerror_r strerror_r_gnu
#endif
#define strdup(str) _strdup(str)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_STRINGS_H_ */
