#include <sys/platform.h>
#include <stdio.h>


const char *wpdk_get_path(const char *);



ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
	// HACK - unimplemented
	return 0;
}


FILE *wpdk_fopen(const char *filename, const char *mode)
{
	return fopen(wpdk_get_path(filename), mode);
}
