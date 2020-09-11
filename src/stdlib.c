#include <wpdklib.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>


int mkstemp(char *path)
{
	if (_mktemp(path) == NULL) return -1;

	// HACK - loop on failure
	return _open(path, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
}


int posix_memalign(void **memptr, size_t alignment, size_t size)
{
	// HACK - not aligned - just basic malloc for now
	UNREFERENCED_PARAMETER(alignment);

	if (!memptr || !size)
		return EINVAL;

	*memptr = malloc(size);

	if (!*memptr)
		return ENOMEM;
	return 0;
}


void srandom(unsigned int seed)
{
	srand(seed);
}


long int random(void)
{
	// HACK 16 bits instead of 32-bits
	return rand();
}


int rand_r(unsigned int *seedp)
{
	// HACK - incomplete implementation
	UNREFERENCED_PARAMETER(seedp);
	return rand();
}


void *__real_calloc(size_t nelem, size_t elsize)
{
	return calloc(nelem, elsize);
}
