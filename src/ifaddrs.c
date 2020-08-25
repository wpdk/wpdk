#include <sys/platform.h>
#include <ifaddrs.h>


inline static int getifaddrs(struct ifaddrs **ifap)
{
    // HACK - not implemented
    return EINVAL;
}


inline static void freeifaddrs(struct ifaddrs *ifa)
{
    // HACK - not implemented
}