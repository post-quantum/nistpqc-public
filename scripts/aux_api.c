#include "api.h"
#include "aux_api.h"

size_t crypto_get_shared_secret_size(void)
{
	return CRYPTO_BYTES;
}

size_t crypto_get_ciphertext_size(void)
{
	return CRYPTO_CIPHERTEXTBYTES;
}

size_t crypto_get_public_key_size(void)
{
	return CRYPTO_PUBLICKEYBYTES;
}

size_t crypto_get_private_key_size(void)
{
	return CRYPTO_SECRETKEYBYTES;
}

const char* crypto_get_algorithm_name(void)
{
	return CRYPTO_ALGNAME;
}
