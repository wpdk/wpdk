#include <wpdklib.h>
#include <sys/stat.h>


int mknod(const char *path, mode_t mode, dev_t dev)
{
	// HACK - implementation
	return EINVAL;
}


// HACK - check implementation
int stat(const char *path, struct stat *buf)
{
	struct _stat64 stat;
	int rc = _stat64(path, &stat);

	if (rc < 0) return rc;

	buf->st_dev = stat.st_dev;
	buf->st_ino = stat.st_ino;
	buf->st_mode = stat.st_mode;
	buf->st_nlink = stat.st_nlink;
	buf->st_uid = stat.st_uid;
	buf->st_gid	= stat.st_gid;
	buf->st_size = stat.st_size;
	buf->st_atime = stat.st_atime;
	buf->st_mtime = stat.st_mtime;
	buf->st_ctime = stat.st_ctime;

	return 0;
}


// HACK - check implementation
int fstat(int fildes, struct stat *buf)
{
	struct _stat64 stat;
	int rc = _fstat64(fildes, &stat);

	if (rc < 0) return rc;

	buf->st_dev = stat.st_dev;
	buf->st_ino = stat.st_ino;
	buf->st_mode = stat.st_mode;
	buf->st_nlink = stat.st_nlink;
	buf->st_uid = stat.st_uid;
	buf->st_gid	= stat.st_gid;
	buf->st_size = stat.st_size;
	buf->st_atime = stat.st_atime;
	buf->st_mtime = stat.st_mtime;
	buf->st_ctime = stat.st_ctime;
	return 0;
}


int wpdk_chmod(const char *filename, int pmode)
{
	char buf[MAX_PATH];
	int mode = 0;

	if (pmode & S_IREAD) mode |= _S_IREAD;
	if (pmode & S_IWRITE) mode |= _S_IWRITE;

	return _chmod(wpdk_get_path(filename, buf, sizeof(buf)), mode);
}
