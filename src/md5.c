#include <wpdklib.h>
#include <openssl/md5.h>


int MD5_Init(MD5_CTX *c)
{
	UNREFERENCED_PARAMETER(c);
	// HACK - implementation
	return -1;
}


int MD5_Update(MD5_CTX *c, const void *data, size_t len)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(len);
	// HACK - implementation
	return -1;
}


int MD5_Final(unsigned char *md, MD5_CTX *c)
{
	UNREFERENCED_PARAMETER(md);
	UNREFERENCED_PARAMETER(c);
	// HACK - implementation
	return -1;
}


unsigned char *MD5(const unsigned char *d, size_t n, unsigned char *md)
{
	UNREFERENCED_PARAMETER(d);
	UNREFERENCED_PARAMETER(n);
	UNREFERENCED_PARAMETER(md);
	// HACK - implementation
	return NULL;
}


void MD5_Transform(MD5_CTX *c, const unsigned char *b)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(b);
	// HACK - implementation
}
