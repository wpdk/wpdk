#include <wpdklib.h>
#include <sys/resource.h>


int getrlimit(int resource, struct rlimit *rlp)
{
	if (!rlp || resource != RLIMIT_CORE)
		return EINVAL;

	// HACK - not implemented
	memset(rlp, 0, sizeof(struct rlimit));
	return 0;
}


int setrlimit(int resource, const struct rlimit *rlp)
{
	if (!rlp || resource != RLIMIT_CORE)
		return EINVAL;

	// HACK - not implemented
	return 0;
}


int getrusage(int who, struct rusage *usage)
{
	if (!usage || who != RUSAGE_THREAD)
		return EINVAL;

	// HACK - not implemented
	memset(usage, 0, sizeof(struct rusage));
	return 0;
}