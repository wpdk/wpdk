#ifndef _WPDK_UUID_UUID_H_
#define	_WPDK_UUID_UUID_H_

#include <sys/platform.h>

_WPDK_BEGIN_C_HEADER

typedef unsigned char uuid_t[16];

void uuid_clear(uuid_t uu);
void uuid_generate(uuid_t out);
void uuid_copy(uuid_t dst, const uuid_t src);
int uuid_compare(const uuid_t uu1, const uuid_t uu2);
int uuid_is_null(const uuid_t uu);
int uuid_parse(const char *in, uuid_t uu);
void uuid_unparse(const uuid_t uu, char *out);
void uuid_unparse_upper(const uuid_t uu, char *out);
void uuid_unparse_lower(const uuid_t uu, char *out);

_WPDK_END_C_HEADER

#endif /* _WPDK_UUID_UUID_H_ */
