#include <sys/platform.h>
#include <sys/time.h>


// HACK - clock is msec granularity 
int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
	if (clk_id != CLOCK_REALTIME) {
		_set_errno(EINVAL);
		return -1;
	}

	if (timespec_get(tp, TIME_UTC) != TIME_UTC) {
		_set_errno(EINVAL);
		return -1;
	}

	return 0;
}
