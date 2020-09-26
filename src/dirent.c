/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2020, MayaData Inc. All rights reserved.
 *  Copyright (c) 2020, DataCore Software Corporation. All rights reserved.
 * 
 *  POSIX details are based on the Open Group Base Specification Issue 7,
 *  2018 edition at https://pubs.opengroup.org/onlinepubs/9699919799/
 * 
 *  Details about Linux extensions are based on the Linux man-pages project
 *  at https://www.kernel.org/doc/man-pages/
 * 
 *  The use of _findfirst to implement dirent is based upon ideas found at
 *  http://www.two-sdg.demon.co.uk/curbralan/code/dirent/dirent.html
 */

#include <wpdk/internal.h>
#include <stdlib.h>
#include <dirent.h>
#include <io.h>


struct _DIR {
	intptr_t h;
	struct _finddata_t info;
	struct dirent entry;
	char spec[PATH_MAX];
};


DIR *
wpdk_opendir(const char *dirname)
{
	DIR *dirp;
	int error;
	char *cp;

	wpdk_set_invalid_handler();

	if (!dirname || !*dirname)
		return wpdk_posix_nullptr(EINVAL);

	if (strlen(dirname) + 2 >= MAX_PATH)
		return wpdk_posix_nullptr(ENAMETOOLONG);

	if ((dirp = calloc(1, sizeof(struct _DIR))) == NULL)
		return wpdk_posix_nullptr(ENOMEM);

	if (wpdk_get_path(dirname, dirp->spec,
			sizeof(dirp->spec) - 2) == dirname)
		strncpy(dirp->spec, dirname, sizeof(dirp->spec) - 2);

	cp = &dirp->spec[strlen(dirp->spec)-1];
	strcat(cp, (*cp == '/' || *cp == '\\') ? "*" : "\\*");

	if ((dirp->h = _findfirst(dirp->spec, &dirp->info)) == -1) {
		error = errno;
		free(dirp);
		return wpdk_posix_nullptr(error);
	}

	return dirp;
}


int
wpdk_closedir(DIR *dirp)
{
	if (!dirp)
		return wpdk_posix_error(EBADF);

	if (dirp->h != -1)
		_findclose(dirp->h);

	free(dirp);
	return 0;
}


struct dirent *
wpdk_readdir(DIR *dirp)
{
	int error = errno;

	wpdk_set_invalid_handler();

	if (!dirp || dirp->h == -1)
		return wpdk_posix_nullptr(EBADF);

	if (_findnext(dirp->h, &dirp->info) == -1) {
		if (errno == ENOENT) _set_errno(error);
		return NULL;
	}

	strncpy(dirp->entry.d_name, dirp->info.name,
			sizeof(dirp->entry.d_name));

	dirp->entry.d_type =
		((dirp->info.attrib & _A_SUBDIR) != 0) ? DT_DIR : DT_REG;

	dirp->entry.d_ino = 0;
	return &dirp->entry;
}


void
wpdk_rewinddir(DIR *dirp)
{
	wpdk_set_invalid_handler();
	
	if (dirp) {
		if (dirp->h != -1)
			_findclose(dirp->h);

		dirp->h = _findfirst(dirp->spec, &dirp->info);
	}
}
