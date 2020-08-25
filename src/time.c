#include <sys/platform.h>
#include <sys/time.h>

/* HACK - null definitions */

inline static int clock_gettime(clockid_t clk_id, struct timespec *tp) {
    tp->tv_sec  = 0;
    tp->tv_nsec = 0;
    return 0;
}
