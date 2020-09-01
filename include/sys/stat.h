#ifndef _SYS_STAT_H_
#define	_SYS_STAT_H_

#include <sys/platform.h>
#include <../ucrt/sys/stat.h>
#include <io.h>

_CRT_BEGIN_C_HEADER

#define S_IFMT      _S_IFMT
#define S_IFDIR     _S_IFDIR
#define S_IFCHR     _S_IFCHR
#define S_IFREG     _S_IFREG
#define S_IREAD     _S_IREAD
#define S_IWRITE    _S_IWRITE
#define S_IEXEC     _S_IEXEC

#define	S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define	S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define	S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define	S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)

#define	S_ISBLK(m)	(false)
#define	S_ISLNK(m)	(false)
#define	S_ISSOCK(m)	(false)

#define S_IRWXU     (S_IREAD|S_IWRITE|S_IEXEC)
#define S_IRUSR     S_IREAD
#define S_IWUSR     S_IWRITE
#define S_IXUSR     S_IEXEC

#define S_IRWXG     (S_IREAD|S_IWRITE|S_IEXEC)
#define S_IRGRP     S_IREAD
#define S_IWGRP     S_IWRITE
#define S_IXGRP     S_IEXEC

#define S_IRWXO     (S_IREAD|S_IWRITE|S_IEXEC)
#define S_IROTH     S_IREAD
#define S_IWOTH     S_IWRITE
#define S_IXOTH     S_IEXEC

struct stat {
    dev_t   st_dev;
    ino_t   st_ino;
    mode_t  st_mode;
    nlink_t st_nlink;
    uid_t   st_uid;
    gid_t   st_gid;
    off_t   st_size;
    time_t  st_atime;
    time_t  st_mtime;
    time_t  st_ctime;
};

int mknod(const char *path, mode_t mode, dev_t dev);
int stat(const char *path, struct stat *buf);
int fstat(int fildes, struct stat *buf);

#define chmod(path,mode)    _chmod(path,mode)

_CRT_END_C_HEADER

#endif /* !_SYS_STAT_H_ */

