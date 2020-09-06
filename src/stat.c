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


int wpdk_chmod(const char *filename, int pmode)
{
	char buf[MAX_PATH];
	int mode = 0;

	if (pmode & S_IREAD) mode |= _S_IREAD;
	if (pmode & S_IWRITE) mode |= _S_IWRITE;

	return _chmod(wpdk_get_path(filename, buf, sizeof(buf)), mode);
}
