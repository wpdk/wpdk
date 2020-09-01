#include <sys/platform.h>
#include <ifaddrs.h>


int getifaddrs(struct ifaddrs **ifap)
{
    // HACK - not implemented
    // HACK https://www.google.co.uk/search?ei=wOdNX5CuEJLHxgOhkZv4Bg&q=windows+socket+getifaddrs&oq=windows+socket+getifaddrs&gs_lcp=CgZwc3ktYWIQAzoGCAAQBxAeOggIABAIEAcQHjoCCAA6BggAEAgQHlDYV1i4Z2CybWgAcAB4AIABRogBxAaSAQIxNJgBAKABAaoBB2d3cy13aXrAAQE&sclient=psy-ab&ved=0ahUKEwiQjvbrp8frAhWSo3EKHaHIBm8Q4dUDCA0&uact=5
    if (ifap) *ifap = NULL;
    return EINVAL;
}


void freeifaddrs(struct ifaddrs *ifa)
{
    // HACK - not implemented
}