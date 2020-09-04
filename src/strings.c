#include <sys/platform.h>
#include <strings.h>

#undef strerror_r


int strcasecmp(const char *s1, const char *s2)
{
	return _stricmp(s1, s2);
}


int strncasecmp(const char *s1, const char *s2, size_t n)
{
	return _strnicmp(s1, s2, n);
}


int strerror_r(int errnum, char *buf, size_t buflen)
{
	return strerror_s(buf, buflen, errnum);
}


char *strerror_r_gnu(int errnum, char *buf, size_t buflen)
{
	return strerror_s(buf, buflen, errnum) ? NULL : buf;
}


char *strcasestr(const char *haystack, const char *needle)
{
	// HACK - not case insensitive
	return (char *)strstr(haystack, needle);
}