#include <wpdklib.h>
#include <sys/wait.h>


pid_t wait(int *stat_loc)
{
	// HACK - implement
	UNREFERENCED_PARAMETER(stat_loc);
	return (pid_t)-1;
}


pid_t waitpid(pid_t pid, int *stat_loc, int options)
{
	// HACK - implement
	UNREFERENCED_PARAMETER(pid);
	UNREFERENCED_PARAMETER(stat_loc);
	UNREFERENCED_PARAMETER(options);
	return (pid_t)-1;
}
