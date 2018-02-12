/**
 * rng.c
 *
 * OpenSSL wrapper of randombytes method
 **/

#include "rng.h"
#include <openssl/rand.h>
#include <openssl/err.h>

int
randombytes(unsigned char *x, unsigned long long xlen)
{
	int error_code = 0;

	RAND_bytes(x, xlen);

	error_code = (int)ERR_get_error();
	ERR_free_strings();

	return error_code;
}
