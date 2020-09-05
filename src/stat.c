#include <wpdklib.h>
#include <sys/stat.h>


int mknod(const char *path, mode_t mode, dev_t dev)
{
	// HACK - implementation
	return EINVAL;
}


int stat(const char *path, struct stat *buf)
{
	// HACK - implementation
	return EINVAL;
}


int fstat(int fildes, struct stat *buf)
{
	// HACK - implementation
	return EINVAL;
}