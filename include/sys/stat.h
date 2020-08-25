#ifndef _SYS_STAT_H_
#define	_SYS_STAT_H_

#include <sys/platform.h>
#include <../ucrt/sys/stat.h>
#include <io.h>

_CRT_BEGIN_C_HEADER

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

#include "../src/stat.c"

_CRT_END_C_HEADER

#endif /* !_SYS_STAT_H_ */

