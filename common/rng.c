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
	RAND_bytes(x, xlen);

	return ERR_get_error();
}
